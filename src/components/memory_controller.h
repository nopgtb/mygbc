#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H

#include <shared_mutex> //std::shared_mutex
#include <memory> //std::shared_ptr
#include <mutex> //std::unique_lock
#include <map> //std::map
#include "../memory/system_memory_interface.h" //MemoryInterface

namespace mygbc{
    class MemoryController{
        public: 

            /// @brief Default constructor
            MemoryController();

            /// @brief Returns the byte located at the given address.
            /// @details Returns the byte located at the given address. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @return byte value located at the given address or error Status.
            StatusOr<uint8_t> get_byte(const uint16_t addr) noexcept;

            /// @brief Returns the word located at the given address.
            /// @details Returns the word located at the given address. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @return Word value located at the given address or error Status.
            StatusOr<uint16_t> get_word(const uint16_t addr) noexcept;

            /// @brief Returns the current size of the memory managed in bytes
            /// @return Size of the managed memory in bytes.
            std::size_t get_memory_size();

            /// @brief Sets the byte located at the given address to the given value.
            /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @param value Byte, New value.
            /// @return Returns status of the set
            Status set_byte(const uint16_t addr, const uint8_t value) noexcept;

            /// @brief Sets the word located at the given address to the given value.
            /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @param value Word, New value.
            /// @return Returns status of the set
            Status set_word(const uint16_t addr, const uint16_t value) noexcept;

            /// @brief Clears the memory banks array.
            /// @details Clears the memory banks array.
            void free();

            /// @brief Tries to mount memory starting at given address.
            /// @param memory_start_addr Mount range start address
            /// @param memory shared ptr to the memory object
            /// @return Status of the mount try
            Status mount_memory(uint16_t memory_start_addr, std::shared_ptr<SystemMemoryInterface> memory);

            /// @brief Unmounts the range corresponding to the given range start address
            /// @param range_start Range start address
            /// @return Unmount status
            Status unmount_range(const uint16_t range_start);

            private:
            
            /// @brief Checks wheter the given memory range is unocupied.
            /// @param range_start Start of the range
            /// @param range_end End of the range
            /// @return given memory range is unocupied?
            bool memory_range_is_free(const uint16_t range_start, const uint16_t range_end);

            class MemoryBank{
                public:
                    /// @brief Memorybank container
                    MemoryBank(const uint16_t memory_range_start, const uint16_t memory_range_end, bool enabled, std::shared_ptr<SystemMemoryInterface> memory);

                    /// @brief Checks if the external address falls in the range of the memory memory bank
                    /// @param address external address
                    /// @return Does the external address fall in to the range of the memory bank
                    inline bool in_range(const uint16_t address);

                    /// @brief Translates the address into a internal memory address
                    /// @param external_address Address to translate
                    /// @return Translated address
                    inline uint16_t translate_address(const uint16_t external_address);

                    uint16_t range_start;
                    uint16_t range_end;
                    bool mount_enabled;
                    std::shared_ptr<SystemMemoryInterface> memory_bank_;
            };

            /// @brief Gets the mounted memorybank that controls the address
            /// @param address Memory address
            /// @return mounted memorybank that controls the address
            StatusOr<MemoryBank*> get_addr_memory_bank(const uint16_t address);

            //Memory banks container
            std::map<uint16_t, MemoryBank> memory_banks_;

            //Read/Write mutex. shared_ptr so its movable
            std::shared_ptr<std::shared_mutex> memory_banks_mutex_;

    };
}

#endif