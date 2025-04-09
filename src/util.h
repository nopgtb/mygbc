#ifndef UTIL_H
#define UTIL_H

#include <cstdint> //Fixed lenght variables

class Util{
public:
    ///@brief Converts a 16-bit value to network byte order (big-endian) if the system is little-endian.
    ///@details Checks the endianness of the system. If the system is little-endian, it swaps the bytes of the given 16-bit value to convert it to big-endian format. If the system is already big-endian, the value is returned as-is.
    ///@param val A 16-bit unsigned integer that needs to be converted to network byte order.
    ///@return A 16-bit unsigned integer in network byte order (big-endian).
    static uint16_t nthos16_t(const uint16_t val);

    ///@brief Converts the two bytes into their equilevant number in the ASCII - representation. Combines the two chars and returns it as uint8_t number.
    ///@details New licensee format described here https://www.zophar.net/fileuploads/2/10597teazh/gbrom.txt. Fullfills the described handling of the two bytes.
    ///@param first_byte first byte of the two bytes
    ///@param second_byte second byte of the two bytes
    ///@return Combined ASCII number valuation of the two bytes.
    ///@throw std::out_of_range. If either one of the bytes cant be interpeted as a number in ASCII (0x30-0x39). 
    static uint8_t combined_char_based_value(const uint8_t first_byte, const uint8_t second_byte);
};

#endif