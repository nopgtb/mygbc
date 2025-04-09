#ifndef GBC_BINARY_H
#define GBC_BINARY_H

#include "addressable_memory.h" //AddressableMemory
#include <vector> //std::vector
#include <string> //std::string
#include <cstdint> //Fixed lenght variables

///@brief Provides a accesible presentation of the GBC binary.
///@details Parses binary bytes and extracts the available information from it, providing accesible presentation of the GBC binary.
class GBCBinary: public AddressableMemory{
    public:
        ///@brief Data structure for the binary headerdata.
        ///@details Struct for the headerdata (0x134 => 0x14F). Aligned for uint16_t.
        ///         based on https://github.com/icecr4ck/bnGB/blob/master/README.md, https://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt 
        ///         and https://gbdev.gg8.se/wiki/articles/The_Cartridge_Header. 
        struct alignas(uint16_t) gbc_binary_headerdata{
            std::string title; //0x134 => 0x142; byte[14] binary title.
            uint8_t gameboy_type; //0x143, byte[1] 0x00 gameboy, 0x80 gameboy color
            uint8_t licencee_new; //0x144=>0x145, byte[2] licencee code new. Interpeted as value between 00 - 99!
            uint8_t sgb_compatability; //0x146, byte[1] Super GameBoy Compatability flag. Value either 0x00 or 0x03.
            uint8_t cartridge_type; //0x147, byte[1] Cartridge type. (0x00 ROM, 0x01 MBC1 ...) Mem extends.
            uint8_t rom_size; //0x148, byte[1] Rom size. (Num of mem banks. 2-96)
            uint8_t ram_size; //0x149, byte[1] Ram size. (Num of mem banks. 0-16)
            uint8_t japanese_code; //0x14A, byte[1] Japanese code.
            uint8_t licencee_old; //0x14B, byte[1] licencee code old.
            uint8_t mask_rom_version; //0x14C, byte[1] Rom version mask.
            uint8_t complement_check; //0x14D, byte[1] Complement check. Checked, from wiki: x=0:FOR i=0134h TO 014C h:x=x-MEM[i]-1:NEXT
            uint16_t checksum; //0x14E-0x14F, byte[2] headerdata checksum. Not valitated according to the wiki.
        };

        ///@brief Static function that parses the given byte buffer as a GBCBinary.
        ///@details Extracts header data and validates the nintendo logo from the given byte array. Returns info in the form of GBCBinary object.
        ///@param byte_buffer std::vector buffer containing the binary bytes.
        ///@return Parsed GBCBinary ready to be used.
        ///@throw std::out_of_range If given array is too small to be valid GBCBinary.
        static GBCBinary parse_bytes(const std::vector<uint8_t>& byte_buffer);

        ///@brief Initializes empty GBCBinary.
        ///@details Initializes empty GBCBinary.
        GBCBinary();

        ///@brief Initializes GBCBinary with values.
        ///@param header Parsed header data of the binary.
        ///@param valid_logo Logo status of the binary.
        ///@param valid_header Was header validated succesfull using the checksum?
        ///@param byte_buffer Bytes of the binary.
        ///@details Initializes GBCBinary with values.
        GBCBinary(const GBCBinary::gbc_binary_headerdata& header, const bool valid_logo, const bool valid_header, const std::vector<uint8_t>& byte_buffer);

        ///@brief Getter for the binary headerdata variable (see struct `gbc_binary_headerdata`).
        ///@details Returns the headerdata available for the binary.
        ///@return headerdata available for the binary (see struct `gbc_binary_headerdata`).
        const GBCBinary::gbc_binary_headerdata& get_header_data() const;

        ///@brief Does the binary have a valid nintendo logo?
        ///@details Does the 0x104 => 0x133 segtion represent a valid nintendo logo?
        ///@return Is the nintendo logo valid?
        const bool& is_valid_nintendo_logo() const;

        ///@brief Gets the logo status and header data as a string representation.
        ///@brief Gets the logo status and header data as a string representation. Does not include byte contents of binary.
        ///@return Binary header and logo status represented as string.
        std::string to_string() const;
    private:

        ///@brief Checks if the nintendo logo is correct in byte_buffer.
        ///@details Checks wheter the bytes 0x104=>0x133 are a valid nintendo logo.
        ///@param byte_buffer std::vector buffer containing the binary bytes.
        ///@return Were the bytes 0x104=>0x133 present and presented a valid nintendo logo?
        ///@throw std::out_of_range If given array is too small to contain logo.
        static bool valid_nintendo_logo(const std::vector<uint8_t>& byte_buffer);

        ///@brief Checks if the header checksum is correct in byte_buffer.
        ///@details Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D.
        ///@param byte_buffer std::vector buffer containing the binary bytes.
        ///@return Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D.
        ///@throw std::out_of_range If given array is too small to contain header.
        static bool valid_header_checksum(const std::vector<uint8_t>& byte_buffer);

        ///@brief Extracts binary headerdata.
        ///@details Extracts all the binary headerdata available.
        ///@param byte_buffer std::vector buffer containing the binary bytes.
        ///@return headerdata available at 0x134 to 0x14F
        ///@throw std::out_of_range If given array is too small to contain header data.
        static GBCBinary::gbc_binary_headerdata extract_header_data(const std::vector<uint8_t>& byte_buffer);

        //headerdata extracted from the binary
        GBCBinary::gbc_binary_headerdata binary_header_data_;

        //Was header validated succesfull using the checksum?
        bool header_is_valid_;

        //Is the nintendo logo valid (0x104 => 0x133)
        bool has_valid_nintendo_logo_;
};

#endif