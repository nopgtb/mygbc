#ifndef REGISTER_H
#define REGISTER_H

#include "addressable_memory.h"
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

class Register : public AddressableMemory{
    public:
        /// @brief Sets up a 16-bit register (8-byte pair)
        /// @details Sets up a 16-bit register (8-byte pair)
        Register();

        /// @brief Reads and returns the value of the bit at the given byte and bit index
        /// @param byte_index Index of the byte in which the bit is in
        /// @param bit_index Index of the bit from right (0-7 value)
        /// @return bit at the given byte and bit index or error Status
        StatusOr<bool> get_bit(uint8_t byte_index, uint8_t bit_index) const noexcept;

        /// @brief Sets the value of the bit at the given byte and bit index
        /// @param byte_index Index of the byte in which the bit is in
        /// @param bit_index Index of the bit from right (0-7 value)
        /// @param bit_value New value of the bit
        /// @return Status of the set.
        Status set_bit(uint8_t byte_index, uint8_t bit_index, bool bit_value) noexcept;
};

#endif