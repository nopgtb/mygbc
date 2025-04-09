#include "addressable_memory.h"
#include "../util/util.h"
#include <stdexcept> //std::out_of_range
#include <cstring> //std::memcpy

///@brief Default constructor
///@details Default constructor, empty memory, sets read only flag to false.
AddressableMemory::AddressableMemory():read_only_memory_(false){
}

///@brief Setter constructor, sets read only flag to false.
///@details Setter constructor, sets read only flag to false.
///@param memory Contents of the addressable memory
///@param read_only Is the memory read only?
AddressableMemory::AddressableMemory(const std::vector<uint8_t> & memory, const bool read_only):memory_(memory), read_only_memory_(read_only){
}

///@brief Default deconstructor
///@details Calls free as default behaviour
AddressableMemory::~AddressableMemory(){
    free();
}

///@brief Flips the read only memory flag value.
///@details Flips the read only memory flag value.
void AddressableMemory::flip_read_only_flag(){
    read_only_memory_ = !read_only_memory_;
}

///@brief Returns the read only memory flag.
///@details Returns the read only memory flag.
///@return The read only memory flag.
bool AddressableMemory::get_read_only_flag() const{
    return read_only_memory_;
}

///@brief Returns the byte located at the given address.
///@details Returns the byte located at the given address. Address is zero-based indexed. 
///@param addr Zero based address.
///@return byte value located at the given address.
///@throw std::out_of_range if given address is not valid address. 
uint8_t AddressableMemory::get_byte(const uint16_t addr) const {
    if(memory_.size() <= addr){
        return memory_[addr];
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

///@brief Returns the word located at the given address.
///@details Returns the word located at the given address. Address is zero-based indexed. 
///@param addr Zero based address.
///@return Word value located at the given address.
///@throw std::out_of_range if given address is not valid address. 
uint16_t AddressableMemory::get_word(const uint16_t addr) const{
    if(memory_.size() <= addr){
        uint16_t val = 0;
        std::memcpy(&val, &memory_[addr], sizeof(uint16_t));
        val = Util::nthos16_t(val);
        return val;
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

///@brief Allows read only access to the whole memory.
///@details Returns const reference to the memory.
///@return Const reference to the memory.
const std::vector<uint8_t>& AddressableMemory::get_memory() const{
    return memory_;
}

///@brief Sets the byte located at the given address to the given value.
///@details Set the byte located at the given address to the given value. Address is zero-based indexed. 
///@param addr Zero based address.
///@param value Byte, New value.
///@throw std::out_of_range if given address is not valid address. 
void AddressableMemory::set(const uint16_t addr, const uint8_t value){
    if(memory_.size() <= addr){
        if(!get_read_only_flag()){
            memory_[addr] = value;
        }
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

///@brief Sets the word located at the given address to the given value.
///@details Set the word located at the given address to the given value. Address is zero-based indexed. 
///@param addr Zero based address.
///@param value Word, New value.
///@throw std::out_of_range if given address is not valid address. 
void AddressableMemory::set(const uint16_t addr, const uint16_t value){
    const uint16_t first_byte_addr = addr;
    const uint16_t second_byte_addr = addr + 1;
    if(memory_.size() <= first_byte_addr && memory_.size() <= second_byte_addr){
        //Copy each byte at time
        const uint8_t * byte_ptr = reinterpret_cast<const uint8_t*>(&value);
        memory_[first_byte_addr] = byte_ptr[0];
        memory_[second_byte_addr] = byte_ptr[1];
    }
    else{
        throw std::out_of_range("Invalid address given! Can't access memory at address.");
    }
}

///@brief Sets the contents of the memory to the given value. Observes read_only flag.
///@details Sets the contents of the memory to the given value. Observes read_only flag.
///@param contents new contents of the memory
///@throw std::logic_error if read_only flag is set. 
void AddressableMemory::set_memory(const std::vector<uint8_t>& contents){
    if(!read_only_memory_){
        memory_ = contents;
    }
    throw std::logic_error("Tried to set memory that is protected by a read only flag!");
}

///@brief Frees the memory assosiated with the memory object.
///@details Frees the memory assosiated with the memory object.
void AddressableMemory::free(){
    memory_.clear();
    memory_.shrink_to_fit();
    read_only_memory_ = false;
}