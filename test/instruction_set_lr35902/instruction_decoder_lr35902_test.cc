#include "../../src/instruction_set_lr35902/instruction_decoder_lr35902.h" //Instruction*LR35902
#include "../../src/memory/addressable_memory.h" //AddressableMemory
#include <gtest/gtest.h> //GTest
#include <string> //std::string
#include <tuple> //std::tuple

class InstructionDecoderValidTest : public ::testing::TestWithParam<std::tuple<mygbc::AddressableMemory, mygbc::InstructionLR35902>> {
    protected:
    //Make Instruction set available to all tests
    mygbc::InstructionSetLR35902 instruction_set_;
};

/// @brief Tests decoding valid instructions from memory with varying read lenghts
/// @details Status and given instruction checked for valid result
TEST_P(InstructionDecoderValidTest, instruction_decode_valid_instruction){
    std::tuple<mygbc::AddressableMemory, mygbc::InstructionLR35902> test_values = GetParam();
    const uint16_t read_address = 0x0;
    const bool expected_ok_status = true;
    mygbc::StatusOr<mygbc::InstructionLR35902> fetch_value = mygbc::InstructionDecoderLR35902::decode(std::get<0>(test_values), read_address, instruction_set_);
    ASSERT_EQ(fetch_value.ok(), expected_ok_status);
    ASSERT_EQ(fetch_value.value(), std::get<1>(test_values));
}

/// @brief Initantiazation of instruction_decode_valid_instruction.
/// @details  Initantiazation of instruction_decode_valid_instruction.
INSTANTIATE_TEST_SUITE_P(
    instruction_decode_valid_instruction_test_cases,
    InstructionDecoderValidTest,
    ::testing::Values(
        //No read
        std::make_tuple(
            mygbc::AddressableMemory(std::vector<uint8_t>{0x76}, false), //HALT
            mygbc::InstructionLR35902(0x0076,1,std::vector<mygbc::InstructionLR35902::OperandRegister>{},std::vector<mygbc::InstructionLR35902::OperandConstValue>{},false, 0, 0, mygbc::InstructionLR35902::OperandValueInterpHint::NONE,mygbc::InstructionLR35902::ExecutionCondition::NONE,"HALT","HALT","HALT",std::vector<uint8_t>{4},mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE)
        ),
        //1 byte read
        std::make_tuple(
            mygbc::AddressableMemory(std::vector<uint8_t>{0x30, 0xF0}, false), //JR NC, e8
            mygbc::InstructionLR35902(0x0030,2,std::vector<mygbc::InstructionLR35902::OperandRegister>{},std::vector<mygbc::InstructionLR35902::OperandConstValue>{},true, 1, 1, 0xF0, mygbc::InstructionLR35902::OperandValueInterpHint::SIGNED,mygbc::InstructionLR35902::ExecutionCondition::CARRY_NOT_SET,"JR","JR NC, e8","JR NC, %1",std::vector<uint8_t>{12,8},mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE)
        ),
        //2 byte read
        std::make_tuple(
            mygbc::AddressableMemory(std::vector<uint8_t>{0x11, 0xF0, 0xA0}, false), //LD DE, n16 
            mygbc::InstructionLR35902(0x0011,3,std::vector<mygbc::InstructionLR35902::OperandRegister>{mygbc::InstructionLR35902::OperandRegister("DE", 0, false, false, false, false)},std::vector<mygbc::InstructionLR35902::OperandConstValue>{},true, 2, 1, 0xF0A0, mygbc::InstructionLR35902::OperandValueInterpHint::VALUE,mygbc::InstructionLR35902::ExecutionCondition::NONE,"LD","LD DE, n16","LD DE, %1",std::vector<uint8_t>{12},mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE,mygbc::InstructionLR35902::FlagOperation::NO_CHANGE)
        )
    )
);

class InstructionDecoderInvalidTest : public ::testing::TestWithParam<std::tuple<mygbc::AddressableMemory, mygbc::InstructionLR35902>> {
    protected:
    //Make Instruction set available to all tests
    mygbc::InstructionSetLR35902 instruction_set_;
};

/// @brief Tests that decoder rejects illegal insturctions and returns a status for them
/// @details Tests all sizes of illegal instructions
TEST_P(InstructionDecoderInvalidTest, instruction_decode_invalid_instruction){
    std::tuple<mygbc::AddressableMemory, mygbc::InstructionLR35902> test_values = GetParam();
    const bool expected_ok_status = false;
    const uint16_t read_address = 0x0;
    mygbc::Status::StatusType expected_status = mygbc::Status::StatusType::INVALID_OPCODE_ERROR;
    mygbc::StatusOr<mygbc::InstructionLR35902> fetch_value = mygbc::InstructionDecoderLR35902::decode(std::get<0>(test_values), read_address, instruction_set_);
    ASSERT_EQ(fetch_value.ok(), expected_ok_status);
    ASSERT_EQ(fetch_value.status().code(), expected_status);
}

/// @brief Initantiazation of instruction_decode_invalid_instruction.
/// @details  Initantiazation of instruction_decode_invalid_instruction.
INSTANTIATE_TEST_SUITE_P(
    instruction_decode_invalid_instruction_test_cases,
    InstructionDecoderInvalidTest,
    ::testing::Values(
        //Illegal instruction 1byte
        std::make_tuple(
            mygbc::AddressableMemory(std::vector<uint8_t>{0xE3}, false), //Illegal
            mygbc::InstructionLR35902()
        )
        //Two byte illegals dont exist
    )
);