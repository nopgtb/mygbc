#include "addressable_memory.h" //AddressableMemory
#include "../util/util.h" //Util

namespace mygbc{

    /// @brief Default constructor
    /// @details Default constructor, empty memory, sets read only flag to false.
    AddressableMemory::AddressableMemory():read_only_memory_(false) {
    }

    /// @brief Setter constructor, sets read only flag to false.
    /// @details Setter constructor, sets read only flag to false.
    /// @param memory Contents of the addressable memory
    /// @param read_only Is the memory read only?
    AddressableMemory::AddressableMemory(const std::vector<uint8_t> & memory, const bool read_only)
    :memory_(memory), read_only_memory_(read_only), memory_mutex_(std::make_shared<std::shared_mutex>()){
    }

    /// @brief Returns the read only memory flag.
    /// @details Returns the read only memory flag.
    /// @return The read only memory flag.
    bool AddressableMemory::is_read_only() const noexcept{
        return read_only_memory_;
    }

    /// @brief Returns the byte located at the given address.
    /// @details Returns the byte located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return byte value located at the given address or error Status.
    StatusOr<uint8_t> AddressableMemory::get_byte(const uint16_t addr) noexcept{
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
        if(addr < memory_.size()){
            return memory_[addr];
        }
        return Status::invalid_index_error(
            "Invalid address given for byte read! Can't access memory at address(Addr: " + 
            std::to_string(addr) + "/ Limit: " + std::to_string(memory_.size()) + ")."
        );
    }

    /// @brief Returns the word located at the given address.
    /// @details Returns the word located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return Word value located at the given address or error Status.
    StatusOr<uint16_t> AddressableMemory::get_word(const uint16_t addr) noexcept{
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
        const uint16_t byte_one_addr = addr;
        const uint16_t byte_two_addr = addr + 1;
        if(byte_two_addr < memory_.size()){
            uint16_t val = (static_cast<uint16_t>(memory_[byte_two_addr]) << 8) | static_cast<uint16_t>(memory_[byte_one_addr]);
            return Util::nthos16_t(val);
        }
        return Status::invalid_index_error(
            "Invalid address given for word read! Can't access memory at address(Addr: " + 
            std::to_string(addr) + "/ Limit: " + std::to_string(memory_.size()) + ")."
        );
    }

    /// @brief Allows access to the whole memory.
    /// @details Returns copy of the memory.
    /// @return copy of the memory.
    std::vector<uint8_t> AddressableMemory::get_memory(){
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
        return memory_;
    }

    /// @brief Returns the current size of the memory in bytes
    /// @return Size of the memory in bytes.
    std::size_t AddressableMemory::get_memory_size(){
        std::shared_lock<std::shared_mutex> read_lock(*memory_mutex_);
        return memory_.size();
    }

    /// @brief Sets the byte located at the given address to the given value.
    /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Byte, New value.
    /// @return Returns status of the set
    Status AddressableMemory::set_byte(const uint16_t addr, const uint8_t value) noexcept{
        std::unique_lock<std::shared_mutex> write_lock(*memory_mutex_);
        if(addr < memory_.size()){
            if(!is_read_only()){
                memory_[addr] = value;
                return Status::ok_status();
            }
            return Status::protected_memory_set_error("Tried to set memory when is_read_only flag was set!");
        }
        return Status::invalid_index_error(
            "Invalid address given for byte write! Can't access memory at address(Addr: " + 
            std::to_string(addr) + "/ Limit: " + std::to_string(memory_.size()) + ")."
        );
    }

    /// @brief Sets the word located at the given address to the given value.
    /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Word, New value.
    /// @return Returns status of the set
    Status AddressableMemory::set_word(const uint16_t addr, const uint16_t value) noexcept{
        std::unique_lock<std::shared_mutex> write_lock(*memory_mutex_);
        const uint16_t first_byte_addr = addr;
        const uint16_t second_byte_addr = addr + 1;
        if(second_byte_addr < memory_.size()){
            if(!is_read_only()){
                //Copy each byte at time
                const uint8_t * byte_ptr = reinterpret_cast<const uint8_t*>(&value);
                memory_[first_byte_addr] = byte_ptr[1];
                memory_[second_byte_addr] = byte_ptr[0];
                return Status::ok_status();
            }
            return Status::protected_memory_set_error("Tried to set memory when is_read_only flag was set!");
        }
        return Status::invalid_index_error(
            "Invalid address given for word write! Can't access memory at address(Addr: " + 
            std::to_string(addr) + "/ Limit: " + std::to_string(memory_.size()) + ")."
        );
    }

    /// @brief Sets the contents of the memory to the given value. Observes read_only flag.
    /// @details Sets the contents of the memory to the given value. Observes read_only flag.
    /// @param contents new contents of the memory
    /// @return Returns status of the set
    Status AddressableMemory::set_memory(const std::vector<uint8_t>& contents) noexcept{
        std::unique_lock<std::shared_mutex> write_lock(*memory_mutex_);
        if(!is_read_only()){
            memory_ = contents;
            return Status::ok_status();
        }
        return Status::protected_memory_set_error("Tried to set total memory when is_read_only flag was set!");
    }

    /// @brief Frees the memory assosiated with the memory object.
    /// @details Frees the memory assosiated with the memory object.
    void AddressableMemory::free(){
        std::unique_lock<std::shared_mutex> write_lock(*memory_mutex_);
        memory_.clear();
        memory_.shrink_to_fit();
    }

}//namespace_mygbc