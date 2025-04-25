#include "util.h" //Util
#include <stdexcept> //std::out_of_range
#include <algorithm> //std::find_if

/// @brief Converts a 16-bit value to network byte order (big-endian) if the system is little-endian.
/// @details Checks the endianness of the system. If the system is little-endian, it swaps the bytes of the given 16-bit value to convert it to big-endian format. If the system is already big-endian, the value is returned as-is.
/// @param val A 16-bit unsigned integer that needs to be converted to network byte order.
/// @return A 16-bit unsigned integer in network byte order (big-endian).
uint16_t Util::nthos16_t(const uint16_t val){
    uint16_t test = 0x0102;
    uint8_t* ptr = reinterpret_cast<uint8_t*>(&test);
    if (ptr[0] == 0x02){ //Little endian system?
        return (val >> 8) | (val << 8);
    }
    return val;
}

/// @brief Converts the two bytes into their equilevant number in the ASCII - representation. Combines the two chars and returns it as uint8_t number.
/// @details New licensee format described here https://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt. Fullfills the described handling of the two bytes.
/// @param first_byte first byte of the two bytes
/// @param second_byte second byte of the two bytes
/// @return Combined ASCII number valuation of the two bytes or error Status.
StatusOr<uint8_t> Util::combined_char_based_value(const uint8_t first_byte, const uint8_t second_byte){
    //Are we a number in ASCII?
    if(
        (first_byte >= 0x30 && first_byte <= 0x39) &&
        (second_byte >= 0x30 && second_byte <= 0x39)
    )
    {
        return (((uint8_t)(first_byte - 0x30)) * 0xA) + ((uint8_t)(second_byte - 0x30)); 
    }
    Status::invalid_input_error("Can't interpet given bytes as ASCII numbers (values must be between 0x30 - 0x39)!");
}

/// @brief Given a string, trims all trailing 0x00 bytes off the string.
/// @details Given a string, trims all trailing 0x00 bytes off the string.
/// @param str string to be trimmed.
/// @return the remaining string without the null bytes
std::string Util::trim_trailing_null_bytes(const std::string& str){
    std::string trim_result = str;
    trim_result.erase(
        std::find_if(
            trim_result.rbegin(),
            trim_result.rend(),
            [](unsigned char l){return l != 0x00;}
        ).base(),
        trim_result.end()
    );
    return trim_result;
}