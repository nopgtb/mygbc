#ifndef ADDRESSABLE_MEMORY_H
#define ADDRESSABLE_MEMORY_H

#include <vector> //std::vector
#include <mutex> //std::unique_lock
#include <memory> //std::shared_ptr
#include <cstdint> //Fixed lenght variables
#include <shared_mutex> //std::shared_mutex
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

namespace mygbc{

    /// @brief Interface class for all addressable memory.
    /// @details Interface class for all addressable memory. Contains functionality for read only memory.
    class AddressableMemory{
        public:

        /// @brief Default constructor
        /// @details Default constructor, empty memory, sets read only flag to false.
        AddressableMemory();

        /// @brief Setter constructor, sets read only flag to false.
        /// @details Setter constructor, sets read only flag to false.
        /// @param memory Contents of the addressable memory
        /// @param read_only Is the memory read only?
        AddressableMemory(const std::vector<uint8_t> & memory, const bool read_only);
        
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

        /// @brief Allows access to the whole memory.
        /// @details Returns copy of the memory.
        /// @return copy of the memory.
        std::vector<uint8_t> get_memory();

        /// @brief Returns the current size of the memory in bytes
        /// @return Size of the memory in bytes.
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

        /// @brief Sets the contents of the memory to the given value. Observes read_only flag.
        /// @details Sets the contents of the memory to the given value. Observes read_only flag.
        /// @param contents new contents of the memory
        /// @return Returns status of the set
        Status set_memory(const std::vector<uint8_t>& contents) noexcept;

        /// @brief Frees the memory assosiated with the memory object.
        /// @details Frees the memory assosiated with the memory object.
        void free();

        /// @brief Returns the read only memory flag.
        /// @details Returns the read only memory flag.
        /// @return The read only memory flag.
        bool is_read_only() const noexcept;

        protected:
            //Binary bytes
            std::vector<uint8_t> memory_;

            //Read only memory flag (ROM/RAM)
            const bool read_only_memory_;

            //Read/Write mutex. shared_ptr so its movable
            std::shared_ptr<std::shared_mutex> memory_mutex_;
    };

}//namespace_mygbc

#endif