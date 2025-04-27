#ifndef ADDRESSABLE_MEMORY_H
#define ADDRESSABLE_MEMORY_H

#include <vector> //std::vector
#include <cstdint> //Fixed lenght variables
#include "../util/status/status.h" //Status
#include "../util/status/status_or.h" //StatusOr

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

    /// @brief Default deconstructor
    /// @details Calls free as default behaviour
    ~AddressableMemory();
    
    /// @brief Returns the byte located at the given address.
    /// @details Returns the byte located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return byte value located at the given address or error Status.
    StatusOr<uint8_t> get_byte(const uint16_t addr) const noexcept;

    /// @brief Returns the word located at the given address.
    /// @details Returns the word located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return Word value located at the given address or error Status.
    StatusOr<uint16_t> get_word(const uint16_t addr) const noexcept;

    /// @brief Allows read only access to the whole memory.
    /// @details Returns const reference to the memory.
    /// @return Const reference to the memory.
    const std::vector<uint8_t>& get_memory() const;

    /// @brief Sets the byte located at the given address to the given value.
    /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Byte, New value.
    /// @return Returns status of the set
    Status set(const uint16_t addr, const uint8_t value) noexcept;

    /// @brief Sets the word located at the given address to the given value.
    /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Word, New value.
    /// @return Returns status of the set
    Status set(const uint16_t addr, const uint16_t value) noexcept;

    /// @brief Sets the contents of the memory to the given value. Observes read_only flag.
    /// @details Sets the contents of the memory to the given value. Observes read_only flag.
    /// @param contents new contents of the memory
    /// @return Returns status of the set
    Status set_memory(const std::vector<uint8_t>& contents) noexcept;

    /// @brief Frees the memory assosiated with the memory object.
    /// @details Frees the memory assosiated with the memory object.
    void free();

    /// @brief Flips the read only memory flag value.
    /// @param flag New read only flag value.
    /// @details Flips the read only memory flag value.
    void set_read_only_flag(bool flag) noexcept;

    /// @brief Returns the read only memory flag.
    /// @details Returns the read only memory flag.
    /// @return The read only memory flag.
    bool is_read_only() const noexcept;

    protected:
        //Binary bytes
        std::vector<uint8_t> memory_;

        //Read only memory flag (ROM/RAM)
        bool read_only_memory_;
};

#endif