#ifndef LR35902_H
#define LR35902_H

#include "lr35902_register_file.h" //LR35902RegisterFile
#include "memory_controller.h" //MemoryController
#include "../instruction_set_lr35902/instruction_set_lr35902.h" //InstructionSetLR35902
#include "../instruction_set_lr35902/instruction_executor_lr35902.h" //InstructionExecutorLR35902

namespace mygbc{

    ///@brief represents the functionality of the LR35902 CPU
    class LR35902{

        public:
        
        /// @brief Initializes the CPU for execution
        /// @details Sets pc pointing at 0x00 (BOOT start)
        LR35902();

        /// @brief Emulates one fetch-decode-execute cycle returning the costs of that cycle.
        /// @return Status or Cost of the fetch-decode-execute cycle.
        StatusOr<uint8_t> fetch_decode_execute(MemoryController& memory_controller);
        
        private:
        //Registers of the cpu
        LR35902RegisterFile register_file_;

        //Instructions available to the LR35902
        InstructionSetLR35902 instruction_set_;

        //Executor of the LR35902 instructions
        InstructionExecutorLR35902 instruction_executor_;
    };

}//namespace_mygbc

#endif