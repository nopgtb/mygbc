#include "../../../src/util/status/status.h" //Status
#include <gtest/gtest.h> //GTest
#include <string> //std::string
#include <tuple> //std::tuple


struct StatusBuildTestExpectedValues{
    Status::StatusType expected_type;
    std::string expected_message;
    bool expected_ok_status;

    /// @brief Initialize struct data to the given parameters
    /// @param type Expected type from build Status
    /// @param message Expected message from build Status
    /// @param ok_status Expected ok status from build Status
    StatusBuildTestExpectedValues(const Status::StatusType type, const std::string& message, const bool ok_status)
    :expected_type(type), expected_message(message), expected_ok_status(ok_status){
    }
};

class StatusBuildTest : public ::testing::TestWithParam<std::tuple<Status, StatusBuildTestExpectedValues>> {};

/// @brief Checks that Status is built correctly in different scenarios.s 
/// @details Status outputs match the pregiven expected values.
TEST_P(StatusBuildTest, status_build_correctly){
    std::tuple<Status, StatusBuildTestExpectedValues> test_data = GetParam();
    Status built_status = std::get<0>(test_data);
    StatusBuildTestExpectedValues expected_values = std::get<1>(test_data);

    //Test that expectations match reality
    ASSERT_EQ(built_status.ok(), expected_values.expected_ok_status);
    ASSERT_EQ(built_status.get_type(), expected_values.expected_type);
    ASSERT_EQ(built_status.get_message(), expected_values.expected_message);
}

/// @brief Initantiazation of status_build_correctly.
/// @details  Initantiazation of status_build_correctly.
INSTANTIATE_TEST_SUITE_P(
    status_build_correctly_test_cases,
    StatusBuildTest,
    ::testing::Values(
        std::make_tuple(Status(), StatusBuildTestExpectedValues(Status::StatusType::UNKOWN, "", false)), //Default builds a UNKOWN type
        std::make_tuple(Status(Status::StatusType::OK, "status_type_ok"), StatusBuildTestExpectedValues(Status::StatusType::OK, "status_type_ok", true)), //Constructor builds given type
        std::make_tuple(Status(Status::StatusType::INVALID_BINARY_ERROR, "status_invalid"), StatusBuildTestExpectedValues(Status::StatusType::INVALID_BINARY_ERROR, "status_invalid", false)),//Constructor inputs have effect on ok flag output
        std::make_tuple(Status::ok_status(), StatusBuildTestExpectedValues(Status::StatusType::OK, "", true)),//ok_status test
        std::make_tuple(Status::unkown_error("unknown"), StatusBuildTestExpectedValues(Status::StatusType::UNKOWN, "unknown", false)),//unkown_status test
        std::make_tuple(Status::io_error("io_error"), StatusBuildTestExpectedValues(Status::StatusType::IO_ERROR, "io_error", false)),//io_error_status test
        std::make_tuple(Status::protected_memory_set_error("protected_memory_set_error"), StatusBuildTestExpectedValues(Status::StatusType::PROTECTED_MEMORY_SET_ERROR, "protected_memory_set_error", false)),//protected_memory_set_error_status test
        std::make_tuple(Status::invalid_index_error("invalid_index_error"), StatusBuildTestExpectedValues(Status::StatusType::INVALID_INDEX_ERROR, "invalid_index_error", false)),//invalid_index_error_status test
        std::make_tuple(Status::invalid_binary_error("invalid_binary_error"), StatusBuildTestExpectedValues(Status::StatusType::INVALID_BINARY_ERROR, "invalid_binary_error", false)),//invalid_binary_error_status test
        std::make_tuple(Status::invalid_input_error("invalid_input_error"), StatusBuildTestExpectedValues(Status::StatusType::INVALID_INPUT_ERROR, "invalid_input_error", false))//invalid_input_error_status test
    )
);
