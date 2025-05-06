#include "lr35902.h" //LR35902
#include "../instruction_set_lr35902/instruction_decoder_lr35902.h" //InstructionDecoderL35902

namespace mygbc{
    /// @brief Initializes the CPU for execution
    /// @details Sets pc pointing at 0x00 (BOOT start)
    LR35902::LR35902(){
        //Point the pc at the start of the boot rom
        register_file_.pc.set_word(0x00);
    }

    /// @brief Emulates one fetch-decode-execute cycle returning the costs of that cycle.
    /// @return Status or Cost of the fetch-decode-execute cycle.
    StatusOr<uint8_t> LR35902::fetch_decode_execute(MemoryController& memory_controller){
        //Fetch and decode
        StatusOr<InstructionLR35902> instruction_fetch = InstructionDecoderLR35902::decode(
            memory_controller, register_file_.pc.get_word(), instruction_set_
        );
        if(instruction_fetch.ok()){
            //Execute
            InstructionLR35902 instruction = std::move(instruction_fetch).value();
            return instruction_executor_.execute_instruction(instruction, register_file_, memory_controller);
        }
        return instruction_fetch.status();
    }

}