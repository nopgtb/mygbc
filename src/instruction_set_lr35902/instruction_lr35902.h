#ifndef INSTRUCTION_LR35902_H
#define INSTRUCTION_LR35902_H

#include <cstdint> //Fixed lenght variables
#include <vector> //std::vector
#include <string> //std::string

namespace mygbc{
    
    /// @brief Represents a single instruction in the LR35902 instruction set.
    struct InstructionLR35902{
                
        enum class FlagOperation{
            SET = 0,  // Set to 1
            RESET = 1, // Set to 0
            DICTATE = 2,  // As dicated by the instruction
            NO_CHANGE = 3 // Not touched by instruction
        };

        //effects on flags 
        const FlagOperation effect_on_flag_z;
        const FlagOperation effect_on_flag_n;
        const FlagOperation effect_on_flag_h;
        const FlagOperation effect_on_flag_c;

        //0xCB start = 16bit, otherwise 8 bit opcode.
        const uint16_t opcode;

        //Size in bytes
        const uint8_t size_in_bytes;

        //Register affected, letter id, bool to mark non immidiate mode. (Value used rather than register)
        struct OperandRegister{ 
            const std::string id; //Register id
            const uint8_t operand_position; //Order of appearance
            const bool address_mode; // Use as address
            const bool decrement; // Decremented after value is used
            const bool increment; // Incremented after value is used
            const bool value_operand_modified; // -/+ given value operand, before use

            /// @brief Initializes the struct members for register operand.
            /// @details Sets up the constants of the structure.
            /// @param reg_id Letter id of the register
            /// @param op_pos_asm Position of the operand (Assembly)
            /// @param is_address Value is to be treated as address
            /// @param dec_post_op Decrement register after operation is done
            /// @param inc_post_op Increment register after operation is done
            /// @param val_op_mod Value of register is modified by the read value before op
            OperandRegister(const std::string& reg_id, const uint8_t op_pos_asm, const bool is_address, const bool dec_post_op, const bool inc_post_op, const bool val_op_mod);
        };
        const std::vector<OperandRegister> operand_registers;

        //Constant value operands to instructions
        struct OperandConstValue{
            const uint8_t value; //Value of operand
            const uint8_t operand_position; //Order of appearance

            /// @brief Initializes the struct members for constant operand
            /// @param val Value of the constant operand
            /// @param op_pos_asm Position of the operand in the asm instruction 
            OperandConstValue(const uint8_t val, const uint8_t op_pos_asm);
        };
        const std::vector<OperandConstValue> operand_const_values;

        //Immidiate bytes, filled in by decoder
        const bool has_read_value;
        const uint8_t read_value_size_in_bytes;
        const uint8_t read_value_operand_position;
        uint16_t read_value;

        //Some instructions deal with the value operands differently
        enum class OperandValueInterpHint{ 
            NONE = 0,
            VALUE = 1,
            ADDRESS = 2,
            SIGNED = 3
        } const read_value_operand_interp_hint;
        
        //Some instructions have conditions to their execution
        enum class ExecutionCondition{
            NONE = 0,
            CARRY_SET = 1,
            CARRY_NOT_SET = 2,
            ZERO_SET = 3,
            ZERO_NOT_SET = 4
        } const execution_condition;

        //Assembly mnemonic of opcode
        const std::string short_mnemonic;
        const std::string full_mnemonic;

        //Execute cycles
        const uint8_t t_cycles_to_execute;
        const uint8_t t_cycles_to_not_execute;

        /// @brief Initializes invalid InstructionLR35902.
        /// @details Uses the gap at 0xED.
        InstructionLR35902();

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
        /// @param t_cyc_exec Cost in cycles to execute the instruction
        /// @param t_cyc_not_exec Cost in cycles to not execute the instruction
        /// @param eff_flag_z Effect on Zero flag
        /// @param eff_flag_n Effect on Subtract flag 
        /// @param eff_flag_h Effect on Half carry flag
        /// @param eff_flag_c Effect on Carry flag
        InstructionLR35902(
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
            const uint8_t t_cyc_exec,
            const uint8_t t_cyc_not_exec,
            const FlagOperation eff_flag_z,
            const FlagOperation eff_flag_n,
            const FlagOperation eff_flag_h,
            const FlagOperation eff_flag_c
        );

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
        /// @param t_cyc_exec Cost in cycles to execute the instruction
        /// @param t_cyc_not_exec Cost in cycles to not execute the instruction
        /// @param eff_flag_z Effect on Zero flag
        /// @param eff_flag_n Effect on Subtract flag 
        /// @param eff_flag_h Effect on Half carry flag
        /// @param eff_flag_c Effect on Carry flag
        InstructionLR35902(
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
            const uint8_t t_cyc_exec,
            const uint8_t t_cyc_not_exec,
            const FlagOperation eff_flag_z,
            const FlagOperation eff_flag_n,
            const FlagOperation eff_flag_h,
            const FlagOperation eff_flag_c
        );

        /// @brief Comparison operator for the data type
        /// @param other 
        /// @return are the structs a match data wise?
        bool operator==(const InstructionLR35902& other) const noexcept;
    };

}//namespace_mygbc
#endif