#include <fstream> //std::ifstream
#include "binary_reader.h" //BinaryReader

namespace mygbc{

    /// @brief Reads the file and returns its contents as std::vector
    /// @details Reads the file at the given path and returns the contents as std::vector<uint8_t>. If the file cannot be read throws std::ifstream::failure.
    /// @param file_path The file path to the file.
    /// @return A vector of uint8_t representing the file contents in binary format.
    StatusOr<std::vector<uint8_t>> BinaryReader::read_as_bytes(const std::string& file_path) noexcept{
        std::ifstream binary_file(file_path, std::ios::binary);
        //Is file accesible?
        if(!binary_file){
            return Status::io_error("File at the given path was not accesible!");
        }
        //Read and return file contents
        try{
            std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(binary_file), {});
            return buffer;
        }
        catch(const std::bad_alloc& e){
            return Status::io_error(std::string("Issues with allocating memory! ") + e.what());
        }
    }

}//namespace_mygbc