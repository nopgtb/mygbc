#include "../util/io/log_message.h" //LOG
#include "instruction_executor_lr35902.h" //InstructionExecutorLR35902

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
            {"JP", InstructionExecutorLR35902::exec_jp}, //JP Absolute jump. Conditional. HL or Ruint16.
            {"JR", InstructionExecutorLR35902::exec_jr}, //JR Relative jump. Conditional. Rint8.
            {"CALL", InstructionExecutorLR35902::exec_call}, //CALL subroutine jump. Conditional. Ruint16.
            {"RET", InstructionExecutorLR35902::exec_ret}, //RET subroutine return. Conditional.
            {"RETI", InstructionExecutorLR35902::exec_reti} //RETI subroutine return, enable interupts.

        };
    }

    /// @brief Checks the common flag conditionals: Z, C, NZ, NC
    /// @param instruction Instruction info
    /// @param register_file Cpu register file
    /// @return Is the flag condition met that is specified by the instruction?
    StatusOr<bool> InstructionExecutorLR35902::helper_common_flag_check(const InstructionLR35902& instruction, LR35902RegisterFile& register_file){
        StatusOr<bool> flag_fetch;
        bool condition_value = false;
        //Check Z, C, NZ, NC
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
        default:
            //Crash us out, unexpected value
            LOG(FATAL) << "Invalid value in conditional switch! " 
                    << instruction.full_mnemonic << " : " << std::to_string(instruction.opcode) << ", Condition: "
                    << std::to_string(static_cast<int>(instruction.execution_condition));
        }
        if(!flag_fetch.ok()){
            return flag_fetch.status();
        }
        return (flag_fetch.value() == condition_value);
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
            //JP exists with common flag conditionals (C, Z, NC, NZ)
            StatusOr<bool> flag_check = helper_common_flag_check(instruction, register_file);
            if(!flag_check.ok()){
                return flag_check.status();
            }
            jump_condition_satisfied = flag_check.value();
        }
        //Modify the pc accordingly
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        if(jump_condition_satisfied){
            //JP either uses a unsigned 16-bit read value or the register HL
            if(instruction.has_read_value && instruction.read_value_operand_interp_hint == InstructionLR35902::OperandValueInterpHint::ADDRESS){
                //Read value jump
                register_file.pc.set_word(instruction.unsigned_16brv());
            }
            else if(instruction.operand_registers.size() > 0 && instruction.operand_registers[0].id == "HL"){
                //HL jump
                register_file.pc.set_word(register_file.h_l.get_word());
            }
            else{
                return Status::unkown_error("Instruction lacked operands for execution. " + instruction.opcode);
            }
        }
        else{
            //No jump inc pc and fetch potential missed exec timing
            register_file.pc.increment(1);
            if(instruction.t_cycles_costs.size() > 1){
                execution_ticks = instruction.t_cycles_costs[1];
            }
        }
        return execution_ticks;
    }

    /// @brief Executor for all of the JR instructions
    /// @details Handles and executes all of the relative jump variations
    /// @param instruction JR variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_jr(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        bool jump_condition_satisfied = true;
        if(instruction.execution_condition != InstructionLR35902::ExecutionCondition::NONE){
            //JR exists with common flag conditionals (C, Z, NC, NZ)
            StatusOr<bool> flag_check = helper_common_flag_check(instruction, register_file);
            if(!flag_check.ok()){
                return flag_check.status();
            }
            jump_condition_satisfied = flag_check.value();
        }
        //Modify the pc accordingly
        uint16_t pc = register_file.pc.get_word(); //Potential promo issues
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        if(jump_condition_satisfied){
            //JR only has a signed 8bit variant
            if(instruction.has_read_value && instruction.read_value_operand_interp_hint == InstructionLR35902::OperandValueInterpHint::SIGNED){
                int8_t relative_jump = instruction.signed_8brv();
                pc += relative_jump; //Int promo issue?
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

    /// @brief Executor for all of the CALL instructions
    /// @details Handles and executes all of the subroutine calls
    /// @param instruction CALL variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_call(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        bool jump_condition_satisfied = true;
        if(instruction.execution_condition != InstructionLR35902::ExecutionCondition::NONE){
            //CALL exists with common flag conditionals (C, Z, NC, NZ)
            StatusOr<bool> flag_check = helper_common_flag_check(instruction, register_file);
            if(!flag_check.ok()){
                return flag_check.status();
            }
            jump_condition_satisfied = flag_check.value();
        }
        //Modify the pc and sp accordingly
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        if(jump_condition_satisfied){
            //CALL only has a 16-bit address variant
            if(instruction.has_read_value && instruction.read_value_operand_interp_hint == InstructionLR35902::OperandValueInterpHint::ADDRESS){
                //Push pc to stack
                Status pc_push = memory_controller.set_word(register_file.sp.get_word(), register_file.pc.get_word());
                if(!pc_push.ok()){
                    return pc_push;
                }
                register_file.sp.increment(2);
                //Jump to subroutine
                register_file.pc.set_word(instruction.unsigned_16brv());
            }
            else{
                return Status::unkown_error("Instruction lacked operands for execution. " + instruction.opcode);
            }
        }
        else{
            //No jump inc pc and fetch potential missed exec timing
            register_file.pc.increment(1);
            if(instruction.t_cycles_costs.size() > 1){
                execution_ticks = instruction.t_cycles_costs[1];
            }
        }
        return execution_ticks;
    }

    /// @brief Executor for all of the RET instructions
    /// @details Handles and executes all of the returns from subroutines
    /// @param instruction RET variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_ret(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        bool jump_condition_satisfied = true;
        if(instruction.execution_condition != InstructionLR35902::ExecutionCondition::NONE){
            //RET exists with common flag conditionals (C, Z, NC, NZ)
            StatusOr<bool> flag_check = helper_common_flag_check(instruction, register_file);
            if(!flag_check.ok()){
                return flag_check.status();
            }
            jump_condition_satisfied = flag_check.value();
        }
        //Modify the pc and sp accordingly
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        if(jump_condition_satisfied){
            //Read PC from stack and jump to it
            uint16_t sp = register_file.sp.get_word() - 2; //Modify sp
            StatusOr<uint16_t> pc_read = memory_controller.get_word(sp);
            if(!pc_read.ok()){
                return pc_read.status();
            }
            register_file.sp.set_word(sp); //Modify sp
            register_file.pc.set_word(pc_read.value());
        }
        else{
            //No jump inc pc and fetch potential missed exec timing
            register_file.pc.increment(1);
            if(instruction.t_cycles_costs.size() > 1){
                execution_ticks = instruction.t_cycles_costs[1];
            }
        }
        return execution_ticks;
    }

    /// @brief Executor for all of the RETI instructions
    /// @details Handles and executes of the return from subroutine while enabling interupts
    /// @param instruction RETI variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_reti(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        //RETI has no conditions or variations. It just pops pc from stack and enables interupts.
        uint16_t execution_ticks = instruction.t_cycles_costs[0];
        //Read PC from stack and jump to it
        uint16_t sp = register_file.sp.get_word() - 2; //Modify sp
        StatusOr<uint16_t> pc_read = memory_controller.get_word(sp);
        if(!pc_read.ok()){
            return pc_read.status();
        }
        register_file.pc.set_word(pc_read.value());
        register_file.sp.set_word(sp); //Modify sp
        register_file.ime.store(true); //Enable interupts
        return instruction.t_cycles_costs[0];
    }

    /// @brief Executor for all of the LD instructions
    /// @details Handles and executes of load instructions
    /// @param instruction LD variation
    /// @param register_file CPU register file
    /// @param memory_controller Memory access
    /// @return Execution time in ticks or Status if can't execute.
    StatusOr<uint8_t> InstructionExecutorLR35902::exec_ld(const InstructionLR35902& instruction, LR35902RegisterFile& register_file, MemoryController& memory_controller){
        //Split into 16-bit LD, 8-bit LD
    }

}