#include <iostream> //std::cout
#include "util/io/binary_reader.h" //BinaryReader
#include "memory/gbc_binary.h" //GBCBinary

int main(int argc, char* argv[]){
    if(argc > 1){
            const std::string file_path = argv[1];
            std::cout << "Reading " << file_path << " as a binary!" << "\n";
            StatusOr<std::vector<uint8_t>> buffer = BinaryReader::read_as_bytes(file_path);
            if(buffer.ok()){ // No error
                std::cout << "Read " << buffer.value().size() << " bytes from " << file_path << "!\n";
                StatusOr<GBCBinary> gbc_binary = GBCBinary::parse_bytes(buffer.value());
                if(gbc_binary.ok()){
                    std::cout << "Parsed binary successfully!\n\n" << gbc_binary.value().to_string() << "\n";
                }
                else{
                    std::cout << "Failed to parse as GBCBinary! \n";
                }
            }
            else{
                std::cout << "Could not read binary!\n";
            }
    }
    else{
        std::cout << "Provide binary path!\n";
    }
    return 0;
}