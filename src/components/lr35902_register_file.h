#ifndef LR35902_REGISTER_FILE_H
#define LR35902_REGISTER_FILE_H

#include <unordered_map> //std::unordered_map
#include <string> //std::string
#include "../memory/register_16bit.h" //Register16Bit

namespace mygbc{
    struct LR35902RegisterFile{
        Register16Bit ir_ie;
        Register16Bit a_f;
        Register16Bit b_c;
        Register16Bit d_e;
        Register16Bit h_l;
        Register16Bit pc;
        Register16Bit sp;

        //Flag registry index
        const uint8_t lower_eight_index;

        /// id lookup table
        const std::unordered_map<std::string, Register16Bit*> register_lookup_table;

        /// @brief Initializes the register_file and lookup tables.
        LR35902RegisterFile();

        /// @brief Returns the lookup table for id to register lookup.
        /// @return Id to register lookup table.
        const std::unordered_map<std::string, Register16Bit*> get_lookup_table();

        /// @brief Helper to get the zero flag from F register
        /// @details Queries the 7th bit of the F registry.
        /// @return State of the zero flag or status.
        StatusOr<bool> get_zero_flag() noexcept;

        /// @brief Helper to get the carry flag from F register
        /// @details Queries the 4th bit of the F registry.
        /// @return State of the carry flag or status.
        StatusOr<bool> get_carry_flag() noexcept;

        /// @brief Helper to get the subtraction flag from F register
        /// @details Queries the 6th bit of the F registry.
        /// @return State of the subtraction flag or status.
        StatusOr<bool> get_sub_flag() noexcept;

        /// @brief Helper to get the half carry flag from F register
        /// @details Queries the 5th bit of the F registry.
        /// @return State of the half carry flag or status.
        StatusOr<bool> get_half_carry_flag() noexcept;

        /// @brief Helper to set the zero flag from F register
        /// @details Sets the 7th bit of the F registry.
        /// @param new_flag New value of the flag.
        Status set_zero_flag(const bool new_flag) noexcept;

        /// @brief Helper to set the carry flag from F register
        /// @details Sets the 4th bit of the F registry.
        /// @param new_flag New value of the flag.
        Status set_carry_flag(const bool new_flag) noexcept;

        /// @brief Helper to set the subtraction flag from F register
        /// @details Sets the 6th bit of the F registry.
        /// @param new_flag New value of the flag.
        Status set_sub_flag(const bool new_flag) noexcept;

        /// @brief Helper to set the half carry flag from F register
        /// @details Sets the 5th bit of the F registry.
        /// @param new_flag New value of the flag.
        Status set_half_carry_flag(const bool new_flag) noexcept;

        /// @brief Helper to get register by letter id
        /// @details Returns reference to the register.
        /// @return Reference to the register or Status.
        StatusOr<Register16Bit*> get_register_by_id(const std::string& id) noexcept;

    };
}

#endif