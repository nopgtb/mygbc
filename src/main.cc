#include <iostream> //Console output
#include "util/binary_reader.h" //BinaryReader
#include "memory/gbc_binary.h" //GBCBinary

int main(int argc, char* argv[]){
    if(argc > 1){
        try{
            const std::string file_path = argv[1];
            std::cout << "Reading " << file_path << " as a binary!" << "\n";
            std::vector<uint8_t> buffer = BinaryReader::read_as_bytes(file_path);
            std::cout << "Read " << buffer.size() << " bytes from " << file_path << "!\n";

            try{
                GBCBinary gbc_binary = GBCBinary::parse_bytes(buffer);
                std::cout << "Parsed binary successfully!\n\n" << gbc_binary.to_string() << "\n";
            }
            catch(const std::out_of_range& e){
                std::cout << "Failed to parse as GBCBinary: " <<  e.what() << "\n";
            }
        }
        catch(const std::ios_base::failure& e){
            std::cout << "Error: " << e.what() << "\n";
        }
    }
    else{
        std::cout << "Provide binary path!\n";
    }
    return 0;
}