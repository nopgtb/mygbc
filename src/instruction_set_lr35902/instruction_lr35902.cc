#include "instruction_lr35902.h"

namespace mygbc{

    /// @brief Initializes the struct members
    /// @details Sets up the constants of the structure.
    /// @param reg_id Letter id of the register
    /// @param op_pos_asm Position of the operand (Assembly)
    /// @param is_address Value is to be treated as address
    /// @param dec_post_op Decrement register after operation is done
    /// @param inc_post_op Increment register after operation is done
    /// @param val_op_mod Value of register is modified by the read value before op
    InstructionLR35902::OperandRegister::OperandRegister(const std::string& reg_id, const uint8_t op_pos_asm, const bool is_address, const bool dec_post_op, const bool inc_post_op, const bool val_op_mod)
    :id(reg_id), operand_position(op_pos_asm), address_mode(is_address), decrement(dec_post_op), increment(inc_post_op), value_operand_modified(val_op_mod){
    }

    /// @brief Initializes the struct members for constant operand
    /// @param val Value of the constant operand
    /// @param op_pos_asm Position of the operand in the asm instruction 
    InstructionLR35902::OperandConstValue::OperandConstValue(const uint8_t val, const uint8_t op_pos_asm)
    :value(val), operand_position(op_pos_asm){
    }

    /// @brief Initializes a invalid InstructionLR35902.
    /// @details Uses the gap at 0xED.
    InstructionLR35902::InstructionLR35902()
    :opcode(0xED), size_in_bytes(0), operand_registers(), operand_const_values(), has_read_value(false),
    read_value_size_in_bytes(0), read_value_operand_position(0), read_value(0), read_value_operand_interp_hint(InstructionLR35902::OperandValueInterpHint::NONE),
    execution_condition(InstructionLR35902::ExecutionCondition::NONE), short_mnemonic("ILLEGAL"), full_mnemonic("ILLEGAL"), t_cycles_costs({0}),
    effect_on_flag_z(InstructionLR35902::FlagOperation::NO_CHANGE), effect_on_flag_n(InstructionLR35902::FlagOperation::NO_CHANGE), 
    effect_on_flag_h(InstructionLR35902::FlagOperation::NO_CHANGE), effect_on_flag_c(InstructionLR35902::FlagOperation::NO_CHANGE){
    }

    /// @brief Initializes the constant values of the instruction
    /// @param op Opcode 1-2 bytes long
    /// @param byte_size Size of the whole instruction (including read values)
    /// @param oper_reg Operand registers
    /// @param oper_const Operand constants
    /// @param has_r_val Has read value following opcode
    /// @param r_val_size Size of the read value
    /// @param r_val_pos Position of the read value asm
    /// @param r_val_interp_hint How to interpert the read value
    /// @param exec_cond Condition of the execution
    /// @param s_mnem Short ASM mnemonic, e.g. LD
    /// @param f_mnem Full ASM mnemonic 
    /// @param r_mnem Full ASM mnenonic with convinient markers for read value placement. 
    /// @param cycle_costs list of the costs assosiated with executing and skipping the instruction.
    /// @param eff_flag_z Effect on Zero flag
    /// @param eff_flag_n Effect on Subtract flag 
    /// @param eff_flag_h Effect on Half carry flag
    /// @param eff_flag_c Effect on Carry flag
    InstructionLR35902::InstructionLR35902(
        const uint16_t op,
        const uint8_t byte_size,
        const std::vector<OperandRegister>& oper_reg,
        const std::vector<OperandConstValue>& oper_const,
        const bool has_r_val,
        const uint8_t r_val_size,
        const uint8_t r_val_pos,
        const OperandValueInterpHint r_val_interp_hint,
        const ExecutionCondition exec_cond,
        const std::string& s_mnem,
        const std::string& f_mnem,
        const std::string& r_mnem,
        const std::vector<uint8_t>& cycle_costs,
        const FlagOperation eff_flag_z,
        const FlagOperation eff_flag_n,
        const FlagOperation eff_flag_h,
        const FlagOperation eff_flag_c
    )
    :opcode(op), size_in_bytes(byte_size), operand_registers(oper_reg), operand_const_values(oper_const), has_read_value(has_r_val),
    read_value_size_in_bytes(r_val_size), read_value_operand_position(r_val_pos), read_value(0), read_value_operand_interp_hint(r_val_interp_hint),
    execution_condition(exec_cond), short_mnemonic(s_mnem), full_mnemonic(f_mnem), replace_mnenomic(r_mnem), t_cycles_costs(cycle_costs),
    effect_on_flag_z(eff_flag_z), effect_on_flag_n(eff_flag_n), effect_on_flag_h(eff_flag_h), effect_on_flag_c(eff_flag_c){
    }

    /// @brief Initializes the constant values of the instruction
    /// @param op Opcode 1-2 bytes long
    /// @param byte_size Size of the whole instruction (including read values)
    /// @param oper_reg Operand registers
    /// @param oper_const Operand constants
    /// @param has_r_val Has read value following opcode
    /// @param r_val_size Size of the read value
    /// @param r_val_pos Position of the read value asm
    /// @param r_val Read value
    /// @param r_val_interp_hint How to interpert the read value
    /// @param exec_cond Condition of the execution
    /// @param s_mnem Short ASM mnemonic, e.g. LD
    /// @param f_mnem Full ASM mnemonic 
    /// @param r_mnem Full ASM mnenonic with convinient markers for read value placement. 
    /// @param cycle_costs list of the costs assosiated with executing and skipping the instruction.
    /// @param eff_flag_z Effect on Zero flag
    /// @param eff_flag_n Effect on Subtract flag 
    /// @param eff_flag_h Effect on Half carry flag
    /// @param eff_flag_c Effect on Carry flag
    InstructionLR35902::InstructionLR35902(
        const uint16_t op,
        const uint8_t byte_size,
        const std::vector<OperandRegister>& oper_reg,
        const std::vector<OperandConstValue>& oper_const,
        const bool has_r_val,
        const uint8_t r_val_size,
        const uint8_t r_val_pos,
        const uint16_t r_val,
        const OperandValueInterpHint r_val_interp_hint,
        const ExecutionCondition exec_cond,
        const std::string& s_mnem,
        const std::string& f_mnem,
        const std::string& r_mnem,
        const std::vector<uint8_t>& cycle_costs,
        const FlagOperation eff_flag_z,
        const FlagOperation eff_flag_n,
        const FlagOperation eff_flag_h,
        const FlagOperation eff_flag_c
    )
    :opcode(op), size_in_bytes(byte_size), operand_registers(oper_reg), operand_const_values(oper_const), has_read_value(has_r_val),
    read_value_size_in_bytes(r_val_size), read_value_operand_position(r_val_pos), read_value(r_val), read_value_operand_interp_hint(r_val_interp_hint),
    execution_condition(exec_cond), short_mnemonic(s_mnem), full_mnemonic(f_mnem), replace_mnenomic(r_mnem), t_cycles_costs(cycle_costs),
    effect_on_flag_z(eff_flag_z), effect_on_flag_n(eff_flag_n), effect_on_flag_h(eff_flag_h), effect_on_flag_c(eff_flag_c){
    }

    /// @brief Comparison operator for the instructions originating from instruction decoder
    /// @details compares opcode and read_value to determine similarity.
    /// @param other 
    /// @return are the structs a match data wise?
    bool InstructionLR35902::operator==(const InstructionLR35902& other) const noexcept{
        if(has_read_value){
            return (opcode == other.opcode) && (read_value == other.read_value);
        }
        return (opcode == other.opcode);
    }

}//namespace_mygbc