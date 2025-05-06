#include "instruction_executor_lr35902.h"
#include <utility>

namespace mygbc{

    /// @brief Initializes the executor and fetches the jump table.
    /// @details Fetches the jump table to avoid link issues.
    InstructionExecutorLR35902::InstructionExecutorLR35902()
    :jump_map_(std::move(get_jump_table())){
    }

    /// @brief Executes the instruction if valid instruction. 
    /// @param instruction Instruction to execute.
    /// @param register_file Registers of the cpu.
    /// @param memory_controller Memory controller.
    /// @return Execution time in ticks or Status if can't execute. 
    StatusOr<uint8_t> InstructionExecutorLR35902::execute_instruction(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller) const{
        //Check executor table
        if(jump_map_.find(instruction.short_mnemonic) != jump_map_.end()){
            return jump_map_.at(instruction.short_mnemonic)(instruction, register_file, memory_controller);
        }
        return Status::invalid_index_error(
            "Could not find a executor for instruction " + instruction.full_mnemonic
        );
    }

    /// @brief Returns a built jump table containing executor functions for instructions
    /// @details Each function is keyd by the short mnemonic of the instruction
    /// @return jump table for instruction execution functions
    std::unordered_map<std::string, std::function<StatusOr<uint8_t>(const InstructionLR35902&, LR35902RegisterFile&, MemoryController&)>> InstructionExecutorLR35902::get_jump_table() const{
        //Jump map for executes
        return std::unordered_map<std::string, std::function<StatusOr<uint8_t>(const InstructionLR35902&, LR35902RegisterFile&, MemoryController&)>>{
            {"JP", InstructionExecutorLR35902::exec_jp}
        };
    }

    /// @brief Executor for all of the JP instructions
    /// @details Handles and executes all of the absolute jump variations
    /// @param instruction JP variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_jp(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        bool jump_condition_satisfied = true;
        if(instruction.execution_condition != InstructionLR35902::ExecutionCondition::NONE){
            //JP exists with Z,C,Not Z, Not C conditionals
            StatusOr<bool> flag_fetch;
            bool condition_value = false;
            switch (instruction.execution_condition)
            {
            case InstructionLR35902::ExecutionCondition::ZERO_SET:
                flag_fetch = register_file.get_zero_flag(); 
                condition_value = true;
                break;
            case InstructionLR35902::ExecutionCondition::CARRY_SET:
                flag_fetch = register_file.get_carry_flag(); 
                condition_value = true;
                break;
            case InstructionLR35902::ExecutionCondition::ZERO_NOT_SET:
                flag_fetch = register_file.get_zero_flag(); 
                condition_value = false;
                break;
            case InstructionLR35902::ExecutionCondition::CARRY_NOT_SET:
                flag_fetch = register_file.get_carry_flag(); 
                condition_value = false;
                break;
            }
            if(!flag_fetch.ok()){
                return flag_fetch.status();
            }
            jump_condition_satisfied = (flag_fetch.value() == condition_value);
        }
        //Modify the pc accordingly
        uint16_t pc = register_file.pc.get_word();
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        if(jump_condition_satisfied){
            //JP either uses a 16-bit read value or the register HL
            if(instruction.has_read_value){
                //Read value jump
                pc = instruction.read_value;
            }
            else if(instruction.operand_registers.size() > 0){
                //HL jump
                StatusOr<Register16Bit*> register_fetch = register_file.get_register_by_id(instruction.operand_registers[0].id);
                if(!register_fetch.ok()){
                    return register_fetch.status();
                }
                pc = register_fetch.value()->get_word();
            }
            else{
                return Status::unkown_error("Instruction lacked operands for execution. " + instruction.opcode);
            }
        }
        else{
            //No jump inc pc and fetch potential missed exec timing
            ++pc; 
            if(instruction.t_cycles_costs.size() > 1){
                execution_ticks = instruction.t_cycles_costs[1];
            }
        }
        register_file.pc.set_word(pc);
        return execution_ticks;
    }


}