#ifndef BINARY_READER_H
#define BINARY_READER_H

#include "status/status_or.h" //StatusOr
#include <vector> //std::vector
#include <string> //std::string
#include <cstdint> //Fixed lenght variables

/// @brief Simple class enabling reading of binary files.
/// @details Provides functionality to read in binary files as data types.
class BinaryReader{
    public:
    
        /// @brief Reads the file and returns its contents as std::vector or error status.
        /// @details Reads the file at the given path and returns the contents as std::vector<uint8_t>. If the file cannot be read return error status.
        /// @param file_path The file path to the file.
        /// @return A vector of uint8_t representing the file contents in binary format or error status.
        static StatusOr<std::vector<uint8_t>> read_as_bytes(const std::string& file_path) noexcept;
};

#endif