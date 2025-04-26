#include "register.h"
#include <stdexcept> //std::out_of_range
#include <vector> //std::vector

/// @brief Sets up a 16-bit register (8-byte pair)
/// @details Sets up a 16-bit register (8-byte pair)
Register::Register():AddressableMemory(std::vector<uint8_t>(2, 0), false){
}

/// @brief Reads and returns the value of the bit at the given byte and bit index
/// @param byte_index Index of the byte in which the bit is in
/// @param bit_index Index of the bit from right (0-7 value)
/// @return bit at the given byte and bit index or error Status
StatusOr<bool> Register::get_bit(uint8_t byte_index, uint8_t bit_index) const noexcept{
    const uint8_t max_bit_index = 0x07;
    if(
        byte_index < memory_.size() &&
        bit_index <= max_bit_index
    ){
        const uint8_t mask = 0x01;
        //Shift the desired bit to extreme right. Using and with the mask zero rest.
        return static_cast<bool>(((memory_[byte_index] >> bit_index) & mask));
    }
    return Status::invalid_index_error(
        "Invalid byte or invalid bit index given for bit fetch! (byte: " + 
        std::to_string(byte_index) + "/ Limit: " + std::to_string(memory_.size()) + ", bit: " +
        std::to_string(bit_index) + " / Limit: " + std::to_string(max_bit_index) + ")."
    );
}

/// @brief Sets the value of the bit at the given byte and bit index
/// @param byte_index Index of the byte in which the bit is in
/// @param bit_index Index of the bit from right (0-7 value)
/// @param bit_value New value of the bit
/// @return Status of the set.
Status Register::set_bit(uint8_t byte_index, uint8_t bit_index, bool bit_value) noexcept{
    const uint8_t max_bit_index = 0x07;
    if(
        byte_index < memory_.size() &&
        bit_index <= max_bit_index
    ){
        if(bit_value){
            //Creates a mask like 000100. Or turns the 1 position to 1. 
            memory_[byte_index] = memory_[byte_index] | (1 << bit_index);
        }
        else{
            //Creates a mask like 00100 and negate it => 11011. And turns the 0 position to 0.
            memory_[byte_index] = memory_[byte_index] & ~(1 << bit_index);
        }
        return Status::ok_status();
    }
    return Status::invalid_index_error(
        "Invalid byte or invalid bit index given for bit write! (byte: " + 
        std::to_string(byte_index) + "/ Limit: " + std::to_string(memory_.size()) + ", bit: " +
        std::to_string(bit_index) + " / Limit: " + std::to_string(max_bit_index) + ")."
    );
}