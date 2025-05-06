#ifndef GBC_BINARY_H
#define GBC_BINARY_H

#include <vector> //std::vector
#include <string> //std::string
#include <cstdint> //Fixed lenght variables
#include "addressable_memory.h" //AddressableMemory
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

namespace mygbc{

    /// @brief Provides a accesible presentation of the GBC binary.
    /// @details Parses binary bytes and extracts the available information from it, providing accesible presentation of the GBC binary.
    class GBCBinary: public AddressableMemory{
        public:
            /// @brief Data structure for the binary headerdata.
            /// @details Struct for the headerdata (0x134 => 0x14F). Aligned for uint16_t.
            ///         based on https://github.com/icecr4ck/bnGB/blob/master/README.md, https://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt 
            ///         and https://gbdev.gg8.se/wiki/articles/The_Cartridge_Header. 
            struct alignas(uint16_t) GBCBinaryHeaderData{
                std::string title; //0x134 => 0x142 | 0x134 => 0x144 ; byte[16] | byte [15] byte binary title.
                //std::string manufacturing_code; TODO: WIKI. 0x13F=>0x142 potentially a manufacturing code
                uint8_t gameboy_type; //0x143, byte[1] 0x00 gameboy, 0x80 gameboy color
                uint8_t licencee_new; //0x144=>0x145, byte[2] licencee code new. Interpeted as value between 00 - 99!
                uint8_t sgb_compatability; //0x146, byte[1] Super GameBoy Compatability flag. Value either 0x00 or 0x03.
                uint8_t cartridge_type; //0x147, byte[1] Cartridge type. (0x00 ROM, 0x01 MBC1 ...) Mem extends.
                uint8_t rom_size; //0x148, byte[1] Rom size. (Num of mem banks. 2-96)
                uint8_t ram_size; //0x149, byte[1] Ram size. (Num of mem banks. 0-16)
                uint8_t japanese_code; //0x14A, byte[1] Japanese code.
                uint8_t licencee_old; //0x14B, byte[1] licencee code old.
                uint8_t mask_rom_version; //0x14C, byte[1] Rom version mask.
                uint8_t header_checksum; //0x14D, byte[1] Complement check. Checked, from wiki: x=0:FOR i=0134h TO 014C h:x=x-MEM[i]-1:NEXT
                uint16_t global_checksum; //0x14E-0x14F, byte[2] headerdata checksum. Not valitated according to the wiki.

                ///@brief Empty intializer
                ///@details Initializes empty data structure
                GBCBinaryHeaderData();

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
                GBCBinaryHeaderData(
                    const std::string & t, uint8_t g_type, uint8_t licencee_n, uint8_t sgb_comp,
                    uint8_t cart_type, uint8_t rom_s, uint8_t ram_s, uint8_t jap_code, uint8_t licencee_o,
                    uint8_t rom_ver_mask, uint8_t head_check, uint16_t glob_check
                );

                /// @brief Comparison operator for the data type
                /// @param other 
                /// @return are the structs a match data wise?
                bool operator==(const GBCBinaryHeaderData& other) const noexcept;
            };

            /// @brief Static function that parses the given byte buffer as a GBCBinary.
            /// @details Extracts header data and validates the logo from the given byte array. Returns info in the form of GBCBinary object.
            /// @param byte_buffer std::vector buffer containing the binary bytes.
            /// @return Parsed GBCBinary ready to be used or error Status.
            static StatusOr<GBCBinary> parse_bytes(const std::vector<uint8_t>& byte_buffer) noexcept;

            /// @brief Initializes empty GBCBinary.
            /// @details Initializes empty GBCBinary.
            GBCBinary();

            /// @brief Initializes GBCBinary with values.
            /// @details Initializes GBCBinary with values.
            /// @param header Parsed header data of the binary.
            /// @param valid_logo Logo status of the binary.
            /// @param valid_header Was header validated succesfull using the checksum?
            /// @param byte_buffer Bytes of the binary.
            GBCBinary(const GBCBinary::GBCBinaryHeaderData& header, const bool valid_logo, const bool valid_header, const std::vector<uint8_t>& byte_buffer);

            /// @brief Initializes GBCBinary with values.
            /// @details Initializes GBCBinary with values.
            /// @param header Parsed header data of the binary.
            /// @param valid_logo Logo status of the binary.
            /// @param valid_header Was header validated succesfull using the checksum?
            /// @param byte_buffer Bytes of the binary.
            GBCBinary(const GBCBinary::GBCBinaryHeaderData&& header, const bool&& valid_logo, const bool&& valid_header, const std::vector<uint8_t>& byte_buffer);


            /// @brief Getter for the binary headerdata variable (see struct `GBCBinaryHeaderData`).
            /// @details Returns the headerdata available for the binary.
            /// @return headerdata available for the binary (see struct `GBCBinaryHeaderData`).
            const GBCBinary::GBCBinaryHeaderData& get_header_data() const noexcept;

            /// @brief Does the binary have a valid logo?
            /// @details Does the 0x104 => 0x133 section represent a valid logo?
            /// @return Is the logo valid?
            const bool& has_valid_logo() const noexcept;

            /// @brief Does the binary have a valid header?
            /// @details Does the 0x134 => 0x14C section match the checksum at 0x14D?
            /// @return Is the header valid?
            const bool& has_valid_header() const noexcept;

            /// @brief Gets the logo status and header data as a string representation.
            /// @brief Gets the logo status and header data as a string representation. Does not include byte contents of binary.
            /// @return Binary header and logo status represented as string.
            std::string to_string();
        private:

            /// @brief Checks if the logo is correct in byte_buffer.
            /// @details Checks wheter the bytes 0x104=>0x133 are a valid logo.
            /// @param byte_buffer std::vector buffer containing the binary bytes.
            /// @return Were the bytes 0x104=>0x133 present and presented a valid logo or error Status.
            static StatusOr<bool> check_logo_validity(const std::vector<uint8_t>& byte_buffer) noexcept;

            /// @brief Checks if the header checksum is correct in byte_buffer.
            /// @details Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D.
            /// @param byte_buffer std::vector buffer containing the binary bytes.
            /// @return Checks wheter the header bytes 0x134=>0x14C are a valid using the checksum at 0x14D or error Status.
            static StatusOr<bool> check_header_checksum_validity(const std::vector<uint8_t>& byte_buffer) noexcept;

            /// @brief Extracts binary headerdata.
            /// @details Extracts all the binary headerdata available.
            /// @param byte_buffer std::vector buffer containing the binary bytes.
            /// @return headerdata available at 0x134 to 0x14F or error Status.
            static StatusOr<GBCBinary::GBCBinaryHeaderData> extract_header_data(const std::vector<uint8_t>& byte_buffer) noexcept;

            //headerdata extracted from the binary
            const GBCBinary::GBCBinaryHeaderData binary_header_data_;

            //Was header validated succesfull using the checksum?
            const bool has_valid_header_;

            //Is the logo valid (0x104 => 0x133)
            const bool has_valid_logo_;
    };

}//namespace_mygbc

#endif