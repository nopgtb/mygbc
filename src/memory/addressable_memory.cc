#include "addressable_memory.h" //AddressableMemory
#include "../util/util.h" //Util
#include <stdexcept> //std::out_of_range
#include <cstring> //std::memcpy

/// @brief Default constructor
/// @details Default constructor, empty memory, sets read only flag to false.
AddressableMemory::AddressableMemory():read_only_memory_(false){
}

/// @brief Setter constructor, sets read only flag to false.
/// @details Setter constructor, sets read only flag to false.
/// @param memory Contents of the addressable memory
/// @param read_only Is the memory read only?
AddressableMemory::AddressableMemory(const std::vector<uint8_t> & memory, const bool read_only):memory_(memory), read_only_memory_(read_only){
}

/// @brief Default deconstructor
/// @details Calls free as default behaviour
AddressableMemory::~AddressableMemory(){
    free();
}

/// @brief Flips the read only memory flag value.
/// @param flag New read only flag value.
/// @details Flips the read only memory flag value.
void AddressableMemory::set_read_only_flag(bool flag){
    read_only_memory_ = flag;
}

/// @brief Returns the read only memory flag.
/// @details Returns the read only memory flag.
/// @return The read only memory flag.
bool AddressableMemory::is_read_only() const{
    return read_only_memory_;
}

/// @brief Returns the byte located at the given address.
/// @details Returns the byte located at the given address. Address is zero-based indexed. 
/// @param addr Zero based address.
/// @return byte value located at the given address.
/// @throw std::out_of_range if given address is not valid address. 
uint8_t AddressableMemory::get_byte(const uint16_t addr) const {
    if(addr < memory_.size()){
        return memory_[addr];
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

/// @brief Returns the word located at the given address.
/// @details Returns the word located at the given address. Address is zero-based indexed. 
/// @param addr Zero based address.
/// @return Word value located at the given address.
/// @throw std::out_of_range if given address is not valid address. 
uint16_t AddressableMemory::get_word(const uint16_t addr) const{
    const uint16_t byte_one_addr = addr;
    const uint16_t byte_two_addr = addr + 1;
    if(byte_two_addr < memory_.size()){
        uint16_t val = (static_cast<uint16_t>(memory_[byte_two_addr]) << 8) | static_cast<uint16_t>(memory_[byte_one_addr]);
        return Util::nthos16_t(val);
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

/// @brief Allows read only access to the whole memory.
/// @details Returns const reference to the memory.
/// @return Const reference to the memory.
const std::vector<uint8_t>& AddressableMemory::get_memory() const{
    return memory_;
}

/// @brief Sets the byte located at the given address to the given value.
/// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
/// @param addr Zero based address.
/// @param value Byte, New value.
/// @throw std::out_of_range if given address is not valid address. 
/// @throw std::logic_error if protected memory flag is set
void AddressableMemory::set(const uint16_t addr, const uint8_t value){
    if(addr < memory_.size()){
        if(!is_read_only()){
            memory_[addr] = value;
        }
        else{
            throw std::logic_error("Can't write on protected memory!");
        }
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

/// @brief Sets the word located at the given address to the given value.
/// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
/// @param addr Zero based address.
/// @param value Word, New value.
/// @throw std::out_of_range if given address is not valid address.
/// @throw std::logic_error if protected memory flag is set
void AddressableMemory::set(const uint16_t addr, const uint16_t value){
    const uint16_t first_byte_addr = addr;
    const uint16_t second_byte_addr = addr + 1;
    if(second_byte_addr < memory_.size()){
        if(!is_read_only()){
            //Copy each byte at time
            const uint8_t * byte_ptr = reinterpret_cast<const uint8_t*>(&value);
            memory_[first_byte_addr] = byte_ptr[1];
            memory_[second_byte_addr] = byte_ptr[0];
        }
        else{
            throw std::logic_error("Can't write on protected memory!");
        }
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

/// @brief Sets the contents of the memory to the given value. Observes read_only flag.
/// @details Sets the contents of the memory to the given value. Observes read_only flag.
/// @param contents new contents of the memory
/// @throw std::logic_error if read_only flag is set. 
void AddressableMemory::set_memory(const std::vector<uint8_t>& contents){
    if(!is_read_only()){
        memory_ = contents;
    }
    else{
        throw std::logic_error("Tried to set memory that is protected by a read only flag!");
    }
}

/// @brief Frees the memory assosiated with the memory object.
/// @details Frees the memory assosiated with the memory object.
void AddressableMemory::free(){
    memory_.clear();
    memory_.shrink_to_fit();
    read_only_memory_ = false;
}