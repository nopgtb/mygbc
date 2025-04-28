#include "gbc_binary.h" //GBCBinary
#include "../util/util.h" //Util
#include <algorithm> //std::equal
#include <cstring> //std::memcpy
#include <sstream> //std::ostringstream
#include <iomanip> //std::hex, std::setw, std::setfill
#include <map> //std::map
#include <iostream>

namespace mygbc{

    /// @brief Empty intializer
    /// @details Initializes empty data structure
    GBCBinary::GBCBinaryHeaderData::GBCBinaryHeaderData()
    :title(""), gameboy_type(0), licencee_new(0), sgb_compatability(0), cartridge_type(0), rom_size(0), 
    ram_size(0), japanese_code(0), licencee_old(0), mask_rom_version(0), header_checksum(0), global_checksum(0)
    {
    }

    /// @brief Value initializer
    /// @param t title
    /// @param g_type gameboy_type
    /// @param licencee_n licencee_new
    /// @param sgb_comp super gameboy compatability 
    /// @param cart_type cartridge type
    /// @param rom_s rom size
    /// @param ram_s ram size
    /// @param jap_code japanese code
    /// @param licencee_o licencee old
    /// @param rom_ver_mask rom version mask
    /// @param head_check header checksum
    /// @param glob_check global checksum
    GBCBinary::GBCBinaryHeaderData::GBCBinaryHeaderData(
        const std::string & t, uint8_t g_type, uint8_t licencee_n, uint8_t sgb_comp,
        uint8_t cart_type, uint8_t rom_s, uint8_t ram_s, uint8_t jap_code, uint8_t licencee_o,
        uint8_t rom_ver_mask, uint8_t head_check, uint16_t glob_check
    )
    :title(t), gameboy_type(g_type), licencee_new(licencee_n), sgb_compatability(sgb_comp), cartridge_type(cart_type), rom_size(rom_s), 
    ram_size(ram_s), japanese_code(jap_code), licencee_old(licencee_o), mask_rom_version(rom_ver_mask), header_checksum(head_check), global_checksum(glob_check)
    {
    }

    /// @brief Comparison operator for the data type
    /// @param other 
    /// @return are the structs a match data wise?
    bool GBCBinary::GBCBinaryHeaderData::operator==(const GBCBinary::GBCBinaryHeaderData& other) const noexcept{
        return title == other.title &&
        gameboy_type == other.gameboy_type &&
        licencee_new == other.licencee_new &&
        sgb_compatability == other.sgb_compatability &&
        cartridge_type == other.cartridge_type &&
        rom_size == other.rom_size &&
        ram_size == other.ram_size && 
        japanese_code == other.japanese_code &&
        licencee_old == other.licencee_old &&
        mask_rom_version == other.mask_rom_version &&
        header_checksum == other.header_checksum &&
        global_checksum == other.global_checksum;
    }

    /// @brief Static function that parses the given byte buffer as a GBCBinary.
    /// @details Extracts header data and validates the logo from the given byte array. Returns info in the form of GBCBinary object.
    /// @param byte_buffer std::vector buffer containing the binary bytes.
    /// @return Parsed GBCBinary ready to be used or error Status.
    StatusOr<GBCBinary> GBCBinary::parse_bytes(const std::vector<unsigned char>& byte_buffer) noexcept{
        //Get data and check for errors
        StatusOr<GBCBinary::GBCBinaryHeaderData> header_data = GBCBinary::extract_header_data(byte_buffer);
        StatusOr<bool> valid_logo = GBCBinary::check_logo_validity(byte_buffer);
        StatusOr<bool> valid_header = GBCBinary::check_header_checksum_validity(byte_buffer);
        if(
            header_data.ok() &&
            valid_logo.ok() &&
            valid_header.ok()
        ){
            return GBCBinary(
                header_data.value(),
                valid_logo.value(),
                valid_header.value(),
                byte_buffer
            );
        }
        return Status::invalid_binary_error("Could not parse the binary!");
    }

    /// @brief Checks if the logo is correct in byte_buffer.
    /// @details Checks wheter the bytes 0x104=>0x133 are a valid logo.
    /// @param byte_buffer std::vector buffer containing the binary bytes.
    /// @return Were the bytes 0x104=>0x133 present and presented a valid logo or error Status.
    StatusOr<bool> GBCBinary::check_logo_validity(const std::vector<uint8_t>& byte_buffer) noexcept{
        const std::vector<uint8_t> logo_bytes = {
            0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03,
            0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08,
            0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E,
            0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
            0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 
            0xB9, 0x33, 0x3E
        };
        //logo located at range 0x104 => 0x133
        const uint16_t logo_start_addr = 0x104;
        const uint16_t logo_end_addr = 0x134; //Exclusive in std::equal
        if(byte_buffer.size() >= logo_end_addr){
            if (
                std::equal(
                    (byte_buffer.begin() + logo_start_addr), 
                    (byte_buffer.begin() + logo_end_addr),
                    logo_bytes.begin()
                )
            ){
                return true;
            }
            return false;
        }
        //Not valid GBC binary
        return Status::invalid_binary_error("Given binary is not valid GBCBinary. Missing logo byte ranges 0x104 to 0x133.");
    }

    /// @brief Checks if the header checksum is correct in byte_buffer.
    /// @details Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D.
    /// @param byte_buffer std::vector buffer containing the binary bytes.
    /// @return Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D or error Status.
    StatusOr<bool> GBCBinary::check_header_checksum_validity(const std::vector<uint8_t>& byte_buffer) noexcept{
        const uint16_t header_end_addr = 0x14F;
        if(byte_buffer.size() >= header_end_addr){
            //From wiki: x=0:FOR i=0134h TO 014C h:x=x-MEM[i]-1:NEXT
            const uint16_t header_check_start_addr = 0x134;
            const uint16_t header_check_end_addr = 0x14C;
            const uint16_t header_checksum_addr = 0x14D;
            uint8_t calculated_checksum = 0x0;
            for(uint16_t cur_header_flag = header_check_start_addr; cur_header_flag <= header_check_end_addr; ++cur_header_flag){
                calculated_checksum = calculated_checksum - byte_buffer[cur_header_flag] - 1;
            }
            if(calculated_checksum == byte_buffer[header_checksum_addr]){
                return true;
            }
            return false;
        }
        //Not valid GBC binary
        return Status::invalid_binary_error("Given binary is not valid GBCBinary. Missing header data at byte ranges 0x134 to 0x14F.");
    }

    /// @brief Extracts binary headerdata.
    /// @details Extracts all the binary headerdata available.
    /// @param byte_buffer std::vector buffer containing the binary bytes.
    /// @return headerdata available at 0x134 to 0x14F or error Status
    StatusOr<GBCBinary::GBCBinaryHeaderData> GBCBinary::extract_header_data(const std::vector<uint8_t>& byte_buffer) noexcept{
        const uint16_t header_end_addr = 0x14F;
        if(byte_buffer.size() >= header_end_addr){
            GBCBinary::GBCBinaryHeaderData header_data;
            const uint16_t header_title_start_addr = 0x134; //Inclusive
            const uint16_t header_title_end_addr_medium = 0x143; //Exclusive
            const uint16_t header_title_end_addr_long = 0x144; //Exclusive

            const std::map<std::string, uint16_t> header_flag_addr = {
                {"gameboy_type", 0x143}, //gameboy_type, 1 byte, Expected value 0x80 | 0xC0. If not, byte part of title (Wiki)
                {"licencee_new_byte_1", 0x144}, //licencee_new, 2 bytes, byte 1/2
                {"licencee_new_byte_2", 0x145}, //licencee_new, 2 bytes, byte 2/2
                {"sgb_compatability", 0x146}, //sgb_compatability, 1 byte
                {"cartridge_type", 0x147}, //cartridge_type, 1 byte
                {"rom_size", 0x148}, //rom_size, 1 byte
                {"ram_size", 0x149}, //ram_size, 1 byte
                {"japanese_code", 0x14A}, //japanese_code, 1 byte
                {"licencee_old", 0x14B}, //licencee_old, 1 byte
                {"mask_rom_version", 0x14C}, //mask_rom_version, 1 byte
                {"header_checksum", 0x14D}, //header_checksum, 1 byte
                {"global_checksum", 0x14E}, //global_checksum, 2 bytes
            };
            uint16_t header_title_end_addr = header_title_end_addr_medium;
            header_data.gameboy_type = byte_buffer[header_flag_addr.at("gameboy_type")];
            if(header_data.gameboy_type != 0x80 && header_data.gameboy_type != 0xC0){
                //Expected value 0x80 | 0xC0. If not, byte part of title (Wiki)
                header_title_end_addr = header_title_end_addr_long;
                header_data.gameboy_type = 0;
            }

            //Handle title as complex data type std::string
            header_data.title = std::string(
                reinterpret_cast<const char*>(
                    &byte_buffer[header_title_start_addr]
                ),
                (header_title_end_addr - header_title_start_addr)
            );
            header_data.title = Util::trim_trailing_null_bytes(header_data.title);
            //Copy header flags
            
            StatusOr<uint8_t> combine_value = Util::combined_char_based_value(
                byte_buffer[header_flag_addr.at("licencee_new_byte_1")],
                byte_buffer[header_flag_addr.at("licencee_new_byte_2")]
            );
            if(combine_value.ok()){
                header_data.licencee_new = combine_value.value();
            }
            else{
                //Ignore any interpetation errors and mark as 0 (None).
                header_data.licencee_new = 0;
            }
        
            header_data.sgb_compatability = byte_buffer[header_flag_addr.at("sgb_compatability")];
            header_data.cartridge_type = byte_buffer[header_flag_addr.at("cartridge_type")];
            header_data.rom_size = byte_buffer[header_flag_addr.at("rom_size")];
            header_data.ram_size = byte_buffer[header_flag_addr.at("ram_size")];
            header_data.japanese_code = byte_buffer[header_flag_addr.at("japanese_code")];
            header_data.licencee_old = byte_buffer[header_flag_addr.at("licencee_old")];
            header_data.mask_rom_version = byte_buffer[header_flag_addr.at("mask_rom_version")];
            header_data.header_checksum = byte_buffer[header_flag_addr.at("header_checksum")];
            std::memcpy(&header_data.global_checksum, &byte_buffer[header_flag_addr.at("global_checksum")], sizeof(uint16_t));
            header_data.global_checksum = Util::nthos16_t(header_data.global_checksum);
            return header_data;
        }
        //Not valid GBC binary
        return Status::invalid_binary_error("Given binary is not valid GBCBinary. Missing header data at byte ranges 0x134 to 0x14F.");
    }

    /// @brief Initializes empty GBCBinary.
    /// @details Initializes empty GBCBinary.
    GBCBinary::GBCBinary():AddressableMemory(), binary_header_data_(), has_valid_header_(false), has_valid_logo_(false){
    }

    /// @brief Initializes GBCBinary with values.
    /// @param header Parsed header data of the binary.
    /// @param valid_logo Logo status of the binary.
    /// @param valid_header Was header validated succesfull using the checksum?
    /// @param byte_buffer Bytes of the binary.
    /// @details Initializes GBCBinary with values.
    GBCBinary::GBCBinary(const GBCBinary::GBCBinaryHeaderData& header, const bool valid_logo, const bool valid_header, const std::vector<uint8_t>& byte_buffer)
    :AddressableMemory(byte_buffer, false), binary_header_data_(header), has_valid_header_(valid_header), has_valid_logo_(valid_logo)
    {
    }

    /// @brief Getter for the binary headerdata variable (see struct `GBCBinaryHeaderData`).
    /// @details Returns the headerdata available for the binary.
    /// @return headerdata available for the binary (see struct `GBCBinaryHeaderData`).
    const GBCBinary::GBCBinaryHeaderData& GBCBinary::get_header_data() const noexcept{
        return binary_header_data_;
    }

    /// @brief Does the binary have a valid logo?
    /// @details Does the 0x104 => 0x133 section represent a valid logo?
    /// @return Is the logo valid?
    const bool& GBCBinary::has_valid_logo() const noexcept{
        return has_valid_logo_;
    }

    /// @brief Does the binary have a valid header?
    /// @details Does the 0x134 => 0x14C section match the checksum at 0x14D?
    /// @return Is the header valid?
    const bool& GBCBinary::has_valid_header() const noexcept{
        return has_valid_header_;
    }

    /// @brief Gets the logo status and header data as a string representation.
    /// @details Gets the logo status and header data as a string representation. Does not include byte contents of binary.
    /// @return Binary header and logo status represented as string.
    std::string GBCBinary::to_string() const{
        std::ostringstream str_builder;
        str_builder << "Binary size in bytes: " << get_memory().size() << "\n";
        str_builder << "Logo status: " << (has_valid_logo_ ? "valid" : "not valid") << "\n";
        str_builder << "Header status: " << (has_valid_header_ ? "valid" : "not valid") << "\n";
        str_builder << "Binary title: " << binary_header_data_.title << "\n";
        str_builder << std::hex;
        //Cast to int, uint8_t might be treated as a char otherwise
        str_builder << "Binary gameboy type: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.gameboy_type) << "\n";
        str_builder << "Binary licencee new: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.licencee_new) << "\n";
        str_builder << "Binary sgb compatability: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.sgb_compatability) << "\n";
        str_builder << "Binary cartridge type: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.cartridge_type) << "\n";
        str_builder << "Binary rom size: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.rom_size) << "\n";
        str_builder << "Binary ram size: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.ram_size) << "\n";
        str_builder << "Binary japanese code: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.japanese_code) << "\n";
        str_builder << "Binary licencee old: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.licencee_old) << "\n";
        str_builder << "Binary mask rom version: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.mask_rom_version) << "\n";
        str_builder << "Binary header checksum: " << std::setw(2) << std::setfill('0') << static_cast<int>(binary_header_data_.header_checksum) << "\n";
        str_builder << "Binary global checksum: " << std::setw(4) << std::setfill('0') << binary_header_data_.global_checksum << "\n";
        return str_builder.str();
    }

}//namespace_mygbc