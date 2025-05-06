#include "../../../src/util/status/status.h" //Status
#include <gtest/gtest.h> //GTest
#include <string> //std::string
#include <tuple> //std::tuple


struct StatusBuildTestExpectedValues{
    mygbc::Status::StatusType expected_type;
    std::string expected_message;
    bool expected_ok_status;

    /// @brief Initialize struct data to the given parameters
    /// @param type Expected type from build Status
    /// @param message Expected message from build Status
    /// @param ok_status Expected ok status from build Status
    StatusBuildTestExpectedValues(const mygbc::Status::StatusType type, const std::string& message, const bool ok_status)
    :expected_type(type), expected_message(message), expected_ok_status(ok_status){
    }
};

class StatusBuildTest : public ::testing::TestWithParam<std::tuple<mygbc::Status, StatusBuildTestExpectedValues>> {};

/// @brief Checks that Status is built correctly in different scenarios.s 
/// @details Status outputs match the pregiven expected values.
TEST_P(StatusBuildTest, status_build_correctly){
    std::tuple<mygbc::Status, StatusBuildTestExpectedValues> test_data = GetParam();
    mygbc::Status built_status = std::get<0>(test_data);
    StatusBuildTestExpectedValues expected_values = std::get<1>(test_data);

    //Test that expectations match reality
    ASSERT_EQ(built_status.ok(), expected_values.expected_ok_status);
    ASSERT_EQ(built_status.code(), expected_values.expected_type);
    ASSERT_EQ(built_status.message(), expected_values.expected_message);
}

/// @brief Initantiazation of status_build_correctly.
/// @details  Initantiazation of status_build_correctly.
INSTANTIATE_TEST_SUITE_P(
    status_build_correctly_test_cases,
    StatusBuildTest,
    ::testing::Values(
        std::make_tuple(mygbc::Status(), StatusBuildTestExpectedValues(mygbc::Status::StatusType::UNKOWN, "", false)), //Default builds a UNKOWN type
        std::make_tuple(mygbc::Status(mygbc::Status::StatusType::OK, "status_type_ok"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::OK, "status_type_ok", true)), //Constructor builds given type
        std::make_tuple(mygbc::Status(mygbc::Status::StatusType::INVALID_BINARY_ERROR, "status_invalid"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_BINARY_ERROR, "status_invalid", false)),//Constructor inputs have effect on ok flag output
        std::make_tuple(mygbc::Status::ok_status(), StatusBuildTestExpectedValues(mygbc::Status::StatusType::OK, "", true)),//ok_status test
        std::make_tuple(mygbc::Status::unkown_error("unknown"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::UNKOWN, "unknown", false)),//unkown_status test
        std::make_tuple(mygbc::Status::io_error("io_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::IO_ERROR, "io_error", false)),//io_error_status test
        std::make_tuple(mygbc::Status::protected_memory_set_error("protected_memory_set_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::PROTECTED_MEMORY_SET_ERROR, "protected_memory_set_error", false)),//protected_memory_set_error_status test
        std::make_tuple(mygbc::Status::invalid_index_error("invalid_index_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_INDEX_ERROR, "invalid_index_error", false)),//invalid_index_error_status test
        std::make_tuple(mygbc::Status::invalid_binary_error("invalid_binary_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_BINARY_ERROR, "invalid_binary_error", false)),//invalid_binary_error_status test
        std::make_tuple(mygbc::Status::invalid_input_error("invalid_input_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_INPUT_ERROR, "invalid_input_error", false)),//invalid_input_error_status test
        std::make_tuple(mygbc::Status::invalid_opcode_error("invalid_opcode_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_OPCODE_ERROR, "invalid_opcode_error", false)),//invalid_opcode_error_status test
        std::make_tuple(mygbc::Status::invalid_register_id_error("invalid_register_id_error"), StatusBuildTestExpectedValues(mygbc::Status::StatusType::INVALID_REGISTER_ID_ERROR, "invalid_register_id_error", false))//invalid_register_id_error_status test
    )
);
