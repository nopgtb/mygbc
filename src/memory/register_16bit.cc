#include <stdexcept> //std::out_of_range
#include <vector> //std::vector
#include "register_16bit.h" //Register16Bit
#include "../util/util.h" //Util

namespace mygbc{

    /// @brief Sets up a 16-bit register (8-byte pair)
    /// @details Sets up a 16-bit register (8-byte pair)
    Register16Bit::Register16Bit():AddressableMemory(std::vector<uint8_t>(2, 0), false){
    }

    /// @brief Reads and returns the value of the bit at the given byte and bit index
    /// @param byte_index Index of the byte in which the bit is in
    /// @param bit_index Index of the bit from right (0-7 value)
    /// @return bit at the given byte and bit index or error Status
    StatusOr<bool> Register16Bit::get_bit(const uint8_t byte_index, const uint8_t bit_index) noexcept{
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
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
    Status Register16Bit::set_bit(const uint8_t byte_index, const uint8_t bit_index, const bool bit_value) noexcept{
        std::unique_lock<std::shared_mutex> read_lock(*memory_mutex_);
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

    /// @brief Returns the contents of the registry as a word.
    /// @return Word value.
    uint16_t Register16Bit::get_word() noexcept{
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
        const uint16_t byte_one_addr = 0;
        const uint16_t byte_two_addr = 1;
        uint16_t val = (static_cast<uint16_t>(memory_[byte_two_addr]) << 8) | static_cast<uint16_t>(memory_[byte_one_addr]);
        return Util::nthos16_t(val);
    }

    /// @brief Sets the value of the register.
    /// @param value Word, New value.
    void Register16Bit::set_word(const uint16_t value) noexcept{
        std::unique_lock<std::shared_mutex> write_lock(*memory_mutex_);
        const uint16_t first_byte_addr = 0;
        const uint16_t second_byte_addr = 1;
        
        const uint8_t * byte_ptr = reinterpret_cast<const uint8_t*>(&value);
        memory_[first_byte_addr] = byte_ptr[1];
        memory_[second_byte_addr] = byte_ptr[0];
    }

}//namespace_mygbc