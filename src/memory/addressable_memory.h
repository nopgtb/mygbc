#ifndef ADDRESSABLE_MEMORY_H
#define ADDRESSABLE_MEMORY_H

#include <vector> //std::vector
#include <cstdint> //Fixed lenght variables

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
    /// @return byte value located at the given address.
    /// @throw std::out_of_range if given address is not valid address. 
    virtual uint8_t get_byte(const uint16_t addr) const;

    /// @brief Returns the word located at the given address.
    /// @details Returns the word located at the given address. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @return Word value located at the given address.
    /// @throw std::out_of_range if given address is not valid address. 
    virtual uint16_t get_word(const uint16_t addr) const;

    /// @brief Allows read only access to the whole memory.
    /// @details Returns const reference to the memory.
    /// @return Const reference to the memory.
    virtual const std::vector<uint8_t>& get_memory() const;

    /// @brief Sets the byte located at the given address to the given value.
    /// @details Set the byte located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Byte, New value.
    /// @throw std::out_of_range if given address is not valid address.
    /// @throw std::logic_error if protected memory flag is set
    virtual void set(const uint16_t addr, const uint8_t value);

    /// @brief Sets the word located at the given address to the given value.
    /// @details Set the word located at the given address to the given value. Address is zero-based indexed. 
    /// @param addr Zero based address.
    /// @param value Word, New value.
    /// @throw std::out_of_range if given address is not valid address. 
    /// @throw std::logic_error if protected memory flag is set
    virtual void set(const uint16_t addr, const uint16_t value);

    /// @brief Sets the contents of the memory to the given value. Observes read_only flag.
    /// @details Sets the contents of the memory to the given value. Observes read_only flag.
    /// @param contents new contents of the memory
    /// @throw std::logic_error if read_only flag is set. 
    virtual void set_memory(const std::vector<uint8_t>& contents);

    /// @brief Frees the memory assosiated with the memory object.
    /// @details Frees the memory assosiated with the memory object.
    virtual void free();

    /// @brief Flips the read only memory flag value.
    /// @param flag New read only flag value.
    /// @details Flips the read only memory flag value.
    void set_read_only_flag(bool flag);

    /// @brief Returns the read only memory flag.
    /// @details Returns the read only memory flag.
    /// @return The read only memory flag.
    bool is_read_only() const;

    protected:
        //Binary bytes
        std::vector<uint8_t> memory_;

        //Read only memory flag (ROM/RAM)
        bool read_only_memory_;
};

#endif