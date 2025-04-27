#ifndef INSTRUCTION_SET_LR35902_H
#define INSTRUCTION_SET_LR35902_H

#include "instruction_lr35902.h" //InstructionLR35902
#include "../../util/status/status_or.h" //StatusOr
#include <unordered_map> //std::unordered_map

class InstructionSetLR35902{
    public:
    /// @brief Initializes the set by fetching the instruction table.
    /// @details Fetches the instruction table.
    InstructionSetLR35902();

    /// @brief Fetches instruction details by given opcode.
    /// @details If opcode is illegal returns a error status.
    /// @return Instruction details or error status.
    StatusOr<InstructionLR35902> get_by_opcode(uint16_t opcode) const noexcept;

    private:
    /// @brief Returns a built instruction table.
    /// @return Instruction table, keyd by hex represatation of opcodes.
    std::unordered_map<uint16_t, InstructionLR35902> get_instruction_table() const;

    //Hex opcode => Instruction details
    std::unordered_map<uint16_t, InstructionLR35902> instruction_table_;
};

#endif