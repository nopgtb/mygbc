#ifndef MEMORY_MAPPED_REGISTER_8BIT_H
#define MEMORY_MAPPED_REGISTER_8BIT_H

#include "../components/memory_controller.h" //MemoryController
#include "../util/status/status_or.h" //StatusOr

namespace mygbc{

    /// @brief Represents a 8-bit "Register" mapped to a specific memory address.
    class MemoryMappedRegister8Bit{

        public:

            /// @brief Default constructor. Builds not mapped register.
            MemoryMappedRegister8Bit();

            /// @brief Mapping constructor. Takes map address.
            /// @param map_addr Map Address.
            MemoryMappedRegister8Bit(const uint16_t map_addr);

            /// @brief Returns the currently mapped memory address.
            /// @return Address to which register is mapped to.
            uint16_t get_mapping_address();

            /// @brief Sets the mapping address.
            /// @param new_map_addr New address register maps to.
            void set_mapping_address(const uint16_t new_map_addr);

            /// @brief Returns the current size of the memory in bytes
            /// @return Size of the memory in bytes.
            std::size_t get_memory_size();

            /// @brief Returns the byte located at the given address.
            /// @details Returns the byte located at the given address. Address is zero-based indexed. 
            /// @param memory MemoryController to access the mapped address.
            /// @return byte value located at the given address or error Status.
            StatusOr<uint8_t> get_byte(MemoryController& memory) noexcept;

            /// @brief Tries to set the byte located at the mapped address to the given value.
            /// @details Sets the mapped byte trough memorycontroller. 
            /// @param value Byte, New value.
            /// @param memory MemoryController to access the mapped address.
            /// @return Returns status of the set.
            Status set_byte(const uint8_t value, MemoryController& memory) noexcept;

            /// @brief Reads and returns the value of the bit at the given bit index.
            /// @param bit_index Index of the bit from right (0-7 value)
            /// @param memory MemoryController to access the mapped address.
            /// @return bit at the given byte and bit index or error Status
            StatusOr<bool> get_bit(const uint8_t bit_index, MemoryController& memory) noexcept;

            /// @brief Sets the value of the bit at the given byte and bit index
            /// @param bit_index Index of the bit from right (0-7 value)
            /// @param bit_value New value of the bit
            /// @param memory MemoryController to access the mapped address.
            /// @return Status of the set.
            Status set_bit(const uint8_t bit_index, const bool bit_value, MemoryController& memory) noexcept;

            private:
                //Byte we are mapped to
                uint16_t mapped_address_;
            

    };

}

#endif