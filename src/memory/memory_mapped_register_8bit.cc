#include "memory_mapped_register_8bit.h" //MemoryMappedRegister8Bit

namespace mygbc{

    /// @brief Default constructor. Builds not mapped register.
    MemoryMappedRegister8Bit::MemoryMappedRegister8Bit():mapped_address_(0){
    }

    /// @brief Mapping constructor. Takes map address.
    /// @param map_addr Map Address.
    MemoryMappedRegister8Bit::MemoryMappedRegister8Bit(const uint16_t map_addr):mapped_address_(map_addr){
    }

    /// @brief Returns the currently mapped memory address.
    /// @return Address to which register is mapped to.
    uint16_t MemoryMappedRegister8Bit::get_mapping_address(){
        return mapped_address_;
    }

    /// @brief Sets the mapping address.
    /// @param new_map_addr New address register maps to.
    void MemoryMappedRegister8Bit::set_mapping_address(const uint16_t new_map_addr){
        mapped_address_ = new_map_addr;
    }

    /// @brief Returns the current size of the memory in bytes
    /// @return Size of the memory in bytes.
    std::size_t MemoryMappedRegister8Bit::get_memory_size(){
        return sizeof(uint8_t);
    }

    /// @brief Returns the byte located at the given address.
    /// @details Returns the byte located at the given address. Address is zero-based indexed. 
    /// @param memory MemoryController to access the mapped address.
    /// @return byte value located at the given address or error Status.
    StatusOr<uint8_t> MemoryMappedRegister8Bit::get_byte(MemoryController& memory) noexcept{
        return memory.get_byte(mapped_address_);
    }

    /// @brief Tries to set the byte located at the mapped address to the given value.
    /// @details Sets the mapped byte trough memorycontroller. 
    /// @param value Byte, New value.
    /// @param memory MemoryController to access the mapped address.
    /// @return Returns status of the set.
    Status MemoryMappedRegister8Bit::set_byte(const uint8_t value, MemoryController& memory) noexcept{
        return memory.set_byte(mapped_address_, value);
    }

    /// @brief Reads and returns the value of the bit at the given bit index.
    /// @param bit_index Index of the bit from right (0-7 value)
    /// @param memory MemoryController to access the mapped address.
    /// @return bit at the given byte and bit index or error Status
    StatusOr<bool> MemoryMappedRegister8Bit::get_bit(const uint8_t bit_index, MemoryController& memory) noexcept{
        StatusOr<uint8_t> register_value_fetch = get_byte(memory);
        if(register_value_fetch.ok()){
            const uint8_t mask = 0x01;
            //Shift the desired bit to extreme right. Using and with the mask zero rest.
            return static_cast<bool>(((register_value_fetch.value() >> bit_index) & mask));
        }
        return register_value_fetch.status();
    }

    /// @brief Sets the value of the bit at the given byte and bit index
    /// @param bit_index Index of the bit from right (0-7 value)
    /// @param bit_value New value of the bit
    /// @param memory MemoryController to access the mapped address.
    /// @return Status of the set.
    Status MemoryMappedRegister8Bit::set_bit(const uint8_t bit_index, const bool bit_value, MemoryController& memory) noexcept{
        StatusOr<uint8_t> register_value_fetch = get_byte(memory);
        if(register_value_fetch.ok()){
            if(bit_value){
                //Creates a mask like 000100. Or turns the 1 position to 1. 
                set_byte(register_value_fetch.value() | (1 << bit_index), memory);
            }
            else{
                //Creates a mask like 00100 and negate it => 11011. And turns the 0 position to 0.
                set_byte(register_value_fetch.value() & ~(1 << bit_index), memory);
            }
            return Status::ok_status();
        }
        return register_value_fetch.status();
    }

}