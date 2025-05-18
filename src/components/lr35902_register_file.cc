#include "lr35902_register_file.h" //LR35902RegisterFile

namespace mygbc{

    /// @brief Initializes the register_file and lookup tables.
    LR35902RegisterFile::LR35902RegisterFile()
    :lower_eight_index(1), register_lookup_table(std::move(get_lookup_table())){
    }

    /// @brief Returns the lookup table for id to register lookup.
    /// @return Id to register lookup table.
    const std::unordered_map<std::string, Register16Bit*> LR35902RegisterFile::get_lookup_table(){
        return std::unordered_map<std::string, Register16Bit*>{
            {"A", &a_f}, {"F", &a_f}, {"AF", &a_f},
            {"B", &b_c}, {"C", &b_c}, {"BC", &b_c},
            {"D", &d_e}, {"E", &d_e}, {"DE", &d_e},
            {"H", &h_l}, {"L", &h_l}, {"HL", &h_l},
            {"PC", &pc},
            {"SP", &sp}
        };
    }

    /// @brief Helper to get the zero flag from F register
    /// @details Queries the 7th bit of the F registry.
    /// @return State of the zero flag or status.
    StatusOr<bool> LR35902RegisterFile::get_zero_flag() noexcept{
        const uint8_t zero_flag_index = 7;
        return a_f.get_bit(lower_eight_index, zero_flag_index);
    }

    /// @brief Helper to get the carry flag from F register
    /// @details Queries the 4th bit of the F registry.
    /// @return State of the carry flag or status.
    StatusOr<bool> LR35902RegisterFile::get_carry_flag() noexcept{
        const uint8_t carry_flag_index = 4;
        return a_f.get_bit(lower_eight_index, carry_flag_index);
    }

    /// @brief Helper to get the subtraction flag from F register
    /// @details Queries the 6th bit of the F registry.
    /// @return State of the subtraction flag or status.
    StatusOr<bool> LR35902RegisterFile::get_sub_flag() noexcept{
        const uint8_t sub_flag_index = 6;
        return a_f.get_bit(lower_eight_index, sub_flag_index);
    }

    /// @brief Helper to get the half carry flag from F register
    /// @details Queries the 5th bit of the F registry.
    /// @return State of the half carry flag or status.
    StatusOr<bool> LR35902RegisterFile::get_half_carry_flag() noexcept{
        const uint8_t half_carry_flag_index = 6;
        return a_f.get_bit(lower_eight_index, half_carry_flag_index);
    }

    /// @brief Helper to set the zero flag from F register
    /// @details Sets the 7th bit of the F registry.
    /// @param new_flag New value of the flag.
    /// @return Status of the set.
    Status LR35902RegisterFile::set_zero_flag(const bool new_flag) noexcept{
        const uint8_t zero_flag_index = 7;
        return a_f.set_bit(lower_eight_index, zero_flag_index, new_flag);
    }

    /// @brief Helper to set the carry flag from F register
    /// @details Sets the 4th bit of the F registry.
    /// @param new_flag New value of the flag.
    /// @return Status of the set.
    Status LR35902RegisterFile::set_carry_flag(const bool new_flag) noexcept{
        const uint8_t carry_flag_index = 4;
        return a_f.set_bit(lower_eight_index, carry_flag_index, new_flag);
    }

    /// @brief Helper to set the subtraction flag from F register
    /// @details Sets the 6th bit of the F registry.
    /// @param new_flag New value of the flag.
    /// @return Status of the set.
    Status LR35902RegisterFile::set_sub_flag(const bool new_flag) noexcept{
        const uint8_t sub_flag_index = 6;
        return a_f.set_bit(lower_eight_index, sub_flag_index, new_flag);
    }

    /// @brief Helper to set the half carry flag from F register
    /// @details Sets the 5th bit of the F registry.
    /// @param new_flag New value of the flag.
    /// @return Status of the set.
    Status LR35902RegisterFile::set_half_carry_flag(const bool new_flag) noexcept{
        const uint8_t half_carry_flag_index = 6;
        return a_f.set_bit(lower_eight_index, half_carry_flag_index, new_flag);
    }

    /// @brief Helper to get register by letter id
    /// @details Returns reference to the register.
    /// @return Reference to the register or Status.
    StatusOr<Register16Bit*> LR35902RegisterFile::get_register_by_id(const std::string& id) noexcept{
        if(register_lookup_table.find(id) != register_lookup_table.end()){
            return register_lookup_table.at(id);
        }
        //Valid opcode was not found for the input
        return Status::invalid_register_id_error(
            "Given register ID is not a LR35902 register! " + id
        );
    }
}