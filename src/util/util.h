#ifndef UTIL_H
#define UTIL_H

#include <cstdint> //Fixed lenght variables
#include <string> //std::string
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

namespace mygbc{
    
    /// @brief Contains assorted static utility functions
    class Util{
    public:
        /// @brief Converts a 16-bit value to network byte order (big-endian) if the system is little-endian.
        /// @details Checks the endianness of the system. If the system is little-endian, it swaps the bytes of the given 16-bit value to convert it to big-endian format. If the system is already big-endian, the value is returned as-is.
        /// @param val A 16-bit unsigned integer that needs to be converted to network byte order.
        /// @return A 16-bit unsigned integer in network byte order (big-endian).
        static uint16_t nthos16_t(const uint16_t val);

        /// @brief Converts the two bytes into their equilevant number in the ASCII - representation. Combines the two chars and returns it as uint8_t number.
        /// @details New licensee format described here https://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt. Fullfills the described handling of the two bytes.
        /// @param first_byte first byte of the two bytes
        /// @param second_byte second byte of the two bytes
        /// @return Combined ASCII number valuation of the two bytes or error Status.
        static StatusOr<uint8_t> combined_char_based_value(const uint8_t first_byte, const uint8_t second_byte);

        /// @brief Given a string, trims all trailing 0x00 bytes off the string.
        /// @details Given a string, trims all trailing 0x00 bytes off the string.
        /// @param str string to be trimmed.
        /// @return the remaining string without the null bytes
        static std::string trim_trailing_null_bytes(const std::string& str);

        /// @brief Returns current unix timestamp in string format.
        /// @return current unix timestamp in string format.
        static std::string get_unix_timestamp();
    };
}//namespace_mygbc
#endif