#include "memory_controller.h"

namespace mygbc{

    /// @brief Default constructor
    MemoryController::MemoryController():memory_banks_mutex_(std::make_shared<std::shared_mutex>()){
    }

    /// @brief Returns the byte located at the given address.
    /// @details Returns the byte located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return byte value located at the given address or error Status.
    StatusOr<uint8_t> MemoryController::get_byte(const uint16_t addr) noexcept{
        StatusOr<MemoryBank*> memory_bank_fetch = get_addr_memory_bank(addr);
        if(memory_bank_fetch.ok()){
            const uint16_t translated_addr = memory_bank_fetch.value()->translate_address(addr);
            return memory_bank_fetch.value()->memory_bank_->get_byte(translated_addr);
        }
        return memory_bank_fetch.status();
    }

    /// @brief Returns the word located at the given address.
    /// @details Returns the word located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return Word value located at the given address or error Status.
    StatusOr<uint16_t> MemoryController::get_word(const uint16_t addr) noexcept{
        StatusOr<MemoryBank*> memory_bank_fetch = get_addr_memory_bank(addr);
        if(memory_bank_fetch.ok()){
            const uint16_t translated_addr = memory_bank_fetch.value()->translate_address(addr);
            return memory_bank_fetch.value()->memory_bank_->get_word(translated_addr);
        }
        return memory_bank_fetch.status();
    }

    /// @brief Returns the current size of the memory in bytes
    /// @return Size of the memory in bytes.
    std::size_t MemoryController::get_memory_size(){
        std::shared_lock<std::shared_mutex> read_lock(*memory_banks_mutex_);
        uint16_t size = 0;
        for(auto& memory_bank : memory_banks_){
            size += (memory_bank.second.range_end - memory_bank.second.range_start);
        }
        return size;
    }

    /// @brief Sets the byte located at the given address to the given value.
    /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Byte, New value.
    /// @return Returns status of the set
    Status MemoryController::set_byte(const uint16_t addr, const uint8_t value) noexcept{
        StatusOr<MemoryBank*> memory_bank_fetch = get_addr_memory_bank(addr);
        if(memory_bank_fetch.ok()){
            const uint16_t translated_addr = memory_bank_fetch.value()->translate_address(addr);
            return memory_bank_fetch.value()->memory_bank_->set_byte(translated_addr, value);
        }
        return memory_bank_fetch.status();
    }

    /// @brief Sets the word located at the given address to the given value.
    /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Word, New value.
    /// @return Returns status of the set
    Status MemoryController::set_word(const uint16_t addr, const uint16_t value) noexcept{
        StatusOr<MemoryBank*> memory_bank_fetch = get_addr_memory_bank(addr);
        if(memory_bank_fetch.ok()){
            const uint16_t translated_addr = memory_bank_fetch.value()->translate_address(addr);
            return memory_bank_fetch.value()->memory_bank_->set_word(translated_addr, value);
        }
        return memory_bank_fetch.status();
    }

    /// @brief Frees the memory assosiated with the memory object.
    /// @details Frees the memory assosiated with the memory object.
    void MemoryController::free(){
        std::unique_lock<std::shared_mutex> write_lock(*memory_banks_mutex_);
        memory_banks_.clear();
    }

    /// @brief Tries to mount memory starting at given address.
    /// @param memory_start_addr Mount range start address
    /// @param memory shared ptr to the memory object
    /// @return Status of the mount try
    Status MemoryController::mount_memory(uint16_t memory_start_addr, std::shared_ptr<SystemMemoryInterface> memory){
        std::unique_lock<std::shared_mutex> write_lock(*memory_banks_mutex_);
        //Check range availability
        if(memory_range_is_free(memory_start_addr, (memory_start_addr + memory->get_memory_size()))){
            memory_banks_.emplace( 
                memory_start_addr,
                MemoryBank(memory_start_addr, (memory_start_addr + memory->get_memory_size()), true, memory)
            );
            return Status::ok_status();
        }
        return Status::invalid_memory_range_error(
            "Memory range is occupied currently! (" + std::to_string(memory_start_addr) + " - " + std::to_string((memory_start_addr + memory->get_memory_size())) + ")"
        );
    }

    /// @brief Unmounts the range corresponding to the given range start address
    /// @param range_start Range start address
    /// @return Unmount status
    Status MemoryController::unmount_range(const uint16_t range_start){
        std::unique_lock<std::shared_mutex> write_lock(*memory_banks_mutex_);
        auto memory_bank = memory_banks_.find(range_start);
        if(memory_bank != memory_banks_.end()){
            memory_banks_.erase(memory_bank);
            return Status::ok_status();
        }
        return Status::invalid_memory_range_error("Given start address is not associated with any memory range!");
    }

    /// @brief Checks wheter the given memory range is unocupied.
    /// @param range_start Start of the range
    /// @param range_end End of the range
    /// @return given memory range is unocupied?
    bool MemoryController::memory_range_is_free(const uint16_t range_start, const uint16_t range_end){
        auto map_range = memory_banks_.lower_bound(range_start); // Get first above or equal to start
        //Start >= End of all ranges || range_end is not in range of greater or equal range
        return (map_range == memory_banks_.end() || !map_range->second.in_range(range_end))  &&
               (map_range == memory_banks_.begin() || !std::prev(map_range)->second.in_range(range_start)); 
        //We are at begining or prev doesnt have range_start in range.
    }

    /// @brief Gets the mounted memorybank that controls the address
    /// @param address Memory address
    /// @return mounted memorybank that controls the address
    StatusOr<MemoryController::MemoryBank*> MemoryController::get_addr_memory_bank(const uint16_t address){
        std::shared_lock<std::shared_mutex> read_lock(*memory_banks_mutex_);
        auto nearest_low = memory_banks_.lower_bound(address);
        //No memory_bank found
        if(nearest_low != memory_banks_.end()){
            if(nearest_low != memory_banks_.begin() && nearest_low->first > address){
                //Current memory bank starts too high
                nearest_low = std::prev(nearest_low);
            }
            //Are we in range of the bank?
            if(nearest_low->second.in_range(address)){
                return &(nearest_low->second);
            }
        }
        return Status::invalid_memory_range_error(
            "Given address falls beyond the addressed memory range!"
        );
    }

    /// @brief Memorybank container
    MemoryController::MemoryBank::MemoryBank(const uint16_t memory_range_start, const uint16_t memory_range_end, bool enabled, std::shared_ptr<SystemMemoryInterface> memory)
    :range_start(memory_range_start), range_end(memory_range_end), mount_enabled(enabled), memory_bank_(memory){
    }

    /// @brief Checks if the external address falls in the range of the memory memory bank
    /// @param address external address
    /// @return Does the external address fall in to the range of the memory bank
    inline bool MemoryController::MemoryBank::in_range(const uint16_t address){
        return address >= range_start && address <= range_end;
    }

    /// @brief Translates the address into a internal memory address
    /// @param external_address Address to translate
    /// @return Translated address
    inline uint16_t MemoryController::MemoryBank::translate_address(const uint16_t external_address){
        return external_address - range_start;
    }
}