#include <iostream> //std::cout
#include "util/io/binary_reader.h" //BinaryReader
#include "memory/gbc_binary.h" //GBCBinary
#include "instruction_set_lr35902/instruction_decoder_lr35902.h" //InstructionDecoderLR35902
#include "util/io/logger.h"//Logger

int main(int argc, char* argv[]){
    //Init log file
    std::shared_ptr<std::fstream> log_file = std::make_shared<std::fstream>("mygbc.log", std::ios::out | std::ios::app);
    if(log_file->is_open()){
        mygbc::Logger::init_stream(log_file);
    }

    if(argc > 1){
            const std::string file_path = argv[1];
            std::cout << "Reading " << file_path << " as a binary!" << "\n";
            mygbc::StatusOr<std::vector<uint8_t>> buffer_read = mygbc::BinaryReader::read_as_bytes(file_path);
            if(buffer_read.ok()){ // No error
                std::vector<uint8_t> buffer = std::move(buffer_read).value();
                std::cout << "Read " << buffer.size() << " bytes from " << file_path << "!\n";
                mygbc::StatusOr<mygbc::GBCBinary> gbc_binary_read = mygbc::GBCBinary::parse_bytes(buffer);
                if(gbc_binary_read.ok()){
                    mygbc::GBCBinary gbc_binary = std::move(gbc_binary_read).value();
                    std::cout << "Parsed binary successfully!\n\n" << gbc_binary.to_string() << "\n";
                    mygbc::InstructionSetLR35902 lr35902_instructions;
                    const uint16_t cartridge_entry_point = 0x100;
                    mygbc::StatusOr<mygbc::InstructionLR35902> instruction = mygbc::InstructionDecoderLR35902::decode(gbc_binary, cartridge_entry_point, lr35902_instructions);
                    if(instruction.ok()){
                        std::cout << "Decoded instruction " << instruction.value().full_mnemonic << " from address " << cartridge_entry_point;
                    }
                    else{
                        std::cout << "Could not decode instruction from address " << cartridge_entry_point << ", got status " << std::to_string(static_cast<int>(instruction.status().code()));
                    }
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