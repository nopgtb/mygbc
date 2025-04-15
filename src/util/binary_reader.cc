#include "binary_reader.h" //BinaryReader
#include <fstream> //std::ifstream

/// @brief Reads the file and returns its contents as std::vector
/// @details Reads the file at the given path and returns the contents as std::vector<uint8_t>. If the file cannot be read throws std::ifstream::failure.
/// @param file_path The file path to the file.
/// @throws std::ifstream::failure If the file cannot be read (e.g. if the file does not exists).
/// @return A vector of uint8_t representing the file contents in binary format.
std::vector<uint8_t> BinaryReader::read_as_bytes(const std::string& file_path){
    std::ifstream binary_file(file_path, std::ios::binary);
    //Is file accesible?
    if(!binary_file){
        throw std::ifstream::failure("File at the given path was not accesible!");
    }
    //Read and return file contents
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(binary_file), {});
    return buffer;
}