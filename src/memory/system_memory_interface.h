#ifndef MEMORY_INTERFACE_H
#define MEMORY_INTERFACE_H

#include <concepts> //std::concept
#include <vector> //std::vector
#include <cstdint> //Fixed lenght variables
#include "../util/status/status_or.h"

namespace mygbc{
    
    template<typename T>
    ///@brief enforce memory interface trough compile time concept.
    concept SystemMemoryInterfaceConcept = requires(T t, uint16_t addr, uint8_t byte, uint16_t word, const std::vector<uint8_t>& contents) {
        { t.get_byte(addr) } -> std::same_as<StatusOr<uint8_t>>;
        { t.get_word(addr) } -> std::same_as<StatusOr<uint16_t>>;
        { t.get_memory() } -> std::same_as<std::vector<uint8_t>>;
        { t.get_memory_size() } -> std::same_as<std::size_t>;
        { t.set_byte(addr, byte) } -> std::same_as<Status>;
        { t.set_word(addr, word) } -> std::same_as<Status>;
        { t.set_memory(contents) } -> std::same_as<Status>;
        { t.free() } -> std::same_as<void>;
    };

    /// @brief Runtime interface
    class SystemMemoryInterface{

            public:
            /// @brief Returns the byte located at the given address.
            /// @details Returns the byte located at the given address. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @return byte value located at the given address or error Status.
            virtual StatusOr<uint8_t> get_byte(const uint16_t addr) noexcept = 0;

            /// @brief Returns the word located at the given address.
            /// @details Returns the word located at the given address. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @return Word value located at the given address or error Status.
            virtual StatusOr<uint16_t> get_word(const uint16_t addr) noexcept = 0;

            /// @brief Allows access to the whole memory.
            /// @details Returns copy of the memory.
            /// @return copy of the memory.
            virtual std::vector<uint8_t> get_memory() = 0;

            /// @brief Returns the current size of the memory in bytes
            /// @return Size of the memory in bytes.
            virtual std::size_t get_memory_size() = 0;

            /// @brief Sets the byte located at the given address to the given value.
            /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @param value Byte, New value.
            /// @return Returns status of the set
            virtual Status set_byte(const uint16_t addr, const uint8_t value) noexcept = 0;

            /// @brief Sets the word located at the given address to the given value.
            /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
            /// @param addr Zero based address.
            /// @param value Word, New value.
            /// @return Returns status of the set
            virtual Status set_word(const uint16_t addr, const uint16_t value) noexcept = 0;

            /// @brief Sets the contents of the memory to the given value. Observes read_only flag.
            /// @details Sets the contents of the memory to the given value. Observes read_only flag.
            /// @param contents new contents of the memory
            /// @return Returns status of the set
            virtual Status set_memory(const std::vector<uint8_t>& contents) noexcept = 0;

            /// @brief Frees the memory assosiated with the memory object.
            /// @details Frees the memory assosiated with the memory object.
            virtual void free() = 0;

    };

}//namespace_mygbc

#endif