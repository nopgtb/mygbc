#ifndef INSTRUCTION_DECODER_LR35902_H
#define INSTRUCTION_DECODER_LR35902_H

#include <cstdint> //Fixed lenght variables
#include <vector> //std::vector
#include <string> //std::string
#include <type_traits> //std::constructible_from
#include "../memory/system_memory_interface.h" //MemoryInterface
#include "../components/memory_controller.h" //MemoryController
#include "../util/status/status_or.h" //StatusOr
#include "instruction_set_lr35902.h" //InstructionSetLR35902

namespace mygbc{

    /// @brief Static decoder class. Decodes bytes in stream to instructions.
    class InstructionDecoderLR35902{
        public:
            /// @brief Tries to decode the instruction from the given address.
            /// @details If the given address is not valid instruction returns a status
            /// @tparam T typename implementing the SystemMemoryInterfaceConcept concept.
            /// @param memory AddressableMemory derived container
            /// @param address Address of the instruction
            /// @return Decoded instruction or error status
            template <typename T>
            requires (std::is_same_v<T, MemoryController> || SystemMemoryInterfaceConcept<T>)
            static StatusOr<InstructionLR35902> decode(T& memory, const uint16_t address, const InstructionSetLR35902& instruction_set) noexcept{
                StatusOr<InstructionLR35902> instruction_fetch = get_instruction_from_address(memory, address, instruction_set);
                if(instruction_fetch.ok()){
                    InstructionLR35902 instruction = instruction_fetch.value();
                    if(instruction.has_read_value){
                        //Determine the address of read by determining the size of the opcode (total size - value size)
                        const uint16_t value_address = (address + (instruction.size_in_bytes - instruction.read_value_size_in_bytes));
                        StatusOr<uint16_t> value_fetch = get_value_from_address(memory, value_address, instruction.read_value_size_in_bytes);
                        if(value_fetch.ok()){
                            instruction.read_value = value_fetch.value();
                        }
                        else{
                            return value_fetch.status();
                        }
                    }
                    return instruction;
                }
                return instruction_fetch.status();
            }

            private:

            /// @brief Fetches the instruction info matching to the opcode present at the given address.
            /// @details If memory fetches or opcode at the address is invalid returns error state.
            /// @tparam T T typename implementing the SystemMemoryInterfaceConcept concept.
            /// @param memory AddressableMemory derived container.
            /// @param address Address of the instruction.
            /// @return Instruction information or error status.
            template <typename T>
            requires (std::is_same_v<T, MemoryController> || SystemMemoryInterfaceConcept<T>)
            static StatusOr<InstructionLR35902> get_instruction_from_address(T& memory, const uint16_t address, const InstructionSetLR35902& instruction_set) noexcept{
                StatusOr<uint8_t> non_prefixed_opcode_fetch = memory.get_byte(address);
                if(non_prefixed_opcode_fetch.ok()){
                    uint16_t opcode = static_cast<uint16_t>(non_prefixed_opcode_fetch.value());
                    //If first byte is 0xCB, we need second byte to determine the opcode.
                    const uint8_t two_byte_opcode_prefix_ = 0xCB;
                    if(opcode == two_byte_opcode_prefix_){
                        StatusOr<uint16_t> prefixed_opcode_fetch = memory.get_word(address);
                        //If we could not read the word, return the status of the read
                        if(!prefixed_opcode_fetch.ok()){
                            return prefixed_opcode_fetch.status();
                        }
                        opcode = prefixed_opcode_fetch.value();
                    }
                    //Fetch details from the instruction set
                    return instruction_set.get_by_opcode(opcode);
                }
                return non_prefixed_opcode_fetch.status();
            }

            /// @brief Fetches the value present at the given address.
            /// @details If memory fetches at the address fails returns error state.
            /// @tparam T T typename implementing the SystemMemoryInterfaceConcept concept.
            /// @param memory AddressableMemory derived container.
            /// @param address Address of the value.
            /// @param value_size_in_bytes size of the read value.
            /// @return Value or error status.
            template <typename T>
            requires (std::is_same_v<T, MemoryController> || SystemMemoryInterfaceConcept<T>)
            static StatusOr<uint16_t> get_value_from_address(T& memory, const uint16_t address, const uint8_t value_size_in_bytes) noexcept{
                uint16_t value = 0;
                if(value_size_in_bytes > 1){
                    StatusOr<uint16_t> word_fetch_result = memory.get_word(address);
                    if(!word_fetch_result.ok()){
                        return word_fetch_result.status();
                    }
                    value = word_fetch_result.value();
                }
                else{
                    StatusOr<uint8_t> byte_fetch_result = memory.get_byte(address);
                    if(!byte_fetch_result.ok()){
                        return byte_fetch_result.status();
                    }
                    value = static_cast<uint8_t>(byte_fetch_result.value());
                }
                return value;
            }
    };

}//namespace_mygbc

#endif