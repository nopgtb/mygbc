#ifndef INSTRUCTION_EXECUTOR_LR35902_H
#define INSTRUCTION_EXECUTOR_LR35902_H

#include "instruction_lr35902.h"
#include "../util/status/status_or.h"
#include "../components/lr35902_register_file.h"
#include "../components/memory_controller.h"
#include <unordered_map> //std::unordered_map
#include <functional> //std::function
#include <string> //std::string

namespace mygbc{

    class InstructionExecutorLR35902{
        public:

        /// @brief Initializes the executor and fetches the jump table.
        /// @details Fetches the jump table to avoid link issues.
        InstructionExecutorLR35902();

        /// @brief Executes the instruction if valid instruction. 
        /// @param instruction Instruction to execute.
        /// @param register_file Registers of the cpu.
        /// @param memory_controller Memory controller.
        /// @return Execution time in ticks or Status if can't execute. 
        StatusOr<uint8_t> execute_instruction(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller) const;
        
        private:

        /// @brief Returns a built jump table containing executor functions for instructions
        /// @details Each function is keyd by the short mnemonic of the instruction
        /// @return jump table for instruction execution functions
        std::unordered_map<std::string, std::function<StatusOr<uint8_t>(const InstructionLR35902&, LR35902RegisterFile&, MemoryController&)>> get_jump_table() const;

        //Mnemonic => Execute function jump table
        std::unordered_map<std::string, std::function<StatusOr<uint8_t>(const InstructionLR35902&, LR35902RegisterFile&, MemoryController&)>> jump_map_;

        /// @brief Checks the common flag conditionals: Z, C, NZ, NC
        /// @param instruction Instruction info
        /// @param register_file Cpu register file
        /// @return Is the flag condition met that is specified by the instruction?
        static StatusOr<bool> helper_common_flag_check(const InstructionLR35902& instruction, LR35902RegisterFile& register_file);

        /// @brief Executor for all of the JP instructions
        /// @details Handles and executes all of the absolute jump variations
        /// @param instruction JP variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_jp(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

        /// @brief Executor for all of the JR instructions
        /// @details Handles and executes all of the relative jump variations
        /// @param instruction JR variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_jr(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

        /// @brief Executor for all of the CALL instructions
        /// @details Handles and executes all of the subroutine calls
        /// @param instruction CALL variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_call(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

        /// @brief Executor for all of the RET instructions
        /// @details Handles and executes all of the returns from subroutines
        /// @param instruction RET variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_ret(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

        /// @brief Executor for all of the RETI instructions
        /// @details Handles and executes of the return from subroutine while enabling interupts
        /// @param instruction RETI variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_reti(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

        /// @brief Executor for all of the LD instructions
        /// @details Handles and executes of load instructions
        /// @param instruction LD variation
        /// @param register_file CPU register file
        /// @param memory_controller Memory access
        /// @return Execution time in ticks or Status if can't execute.
        static StatusOr<uint8_t> exec_ld(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller);

    };
}//namespace_mygbc

#endif