#ifndef REGISTER_16BIT_H
#define REGISTER_16BIT_H

#include "addressable_memory.h" //AdderessableMemory
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

namespace mygbc{

    /// @brief Represents a 16-bit wide register.
    class Register16Bit : public AddressableMemory{
        public:
            /// @brief Sets up a 16-bit register (8-byte pair)
            /// @details Sets up a 16-bit register (8-byte pair)
            Register16Bit();

            /// @brief Reads and returns the value of the bit at the given byte and bit index
            /// @param byte_index Index of the byte in which the bit is in
            /// @param bit_index Index of the bit from right (0-7 value)
            /// @return bit at the given byte and bit index or error Status
            StatusOr<bool> get_bit(const uint8_t byte_index, const uint8_t bit_index) noexcept;

            /// @brief Sets the value of the bit at the given byte and bit index
            /// @param byte_index Index of the byte in which the bit is in
            /// @param bit_index Index of the bit from right (0-7 value)
            /// @param bit_value New value of the bit
            /// @return Status of the set.
            Status set_bit(const uint8_t byte_index, const uint8_t bit_index, const bool bit_value) noexcept;

            /// @brief Returns the contents of the registry as a word.
            /// @return Word value.
            uint16_t get_word() noexcept;

            /// @brief Sets the value of the register.
            /// @param value Word, New value.
            void set_word(const uint16_t value) noexcept;
    };

}//namespace_mygbc
#endif