#include "../../../src/util/status/status_or.h" //Status
#include <gtest/gtest.h> //GTest
#include <string> //std::string
#include <tuple> //std::tuple

/// @brief Tests building StatusOr from value, fetching that value and that value_or returns value.
/// @details Tests that Status is set to ok correctly for good access.
TEST(StatusOrTest, built_from_value_and_fetch){
    const bool expected_ok_status = true;
    const int expected_value = 0x5;
    const int default_value = 0x3;

    //Build value status
    mygbc::StatusOr<int> test_statusor(expected_value);
    int fetched_value = 0;
    //Test ok status and value fetch
    ASSERT_EQ(test_statusor.ok(), expected_ok_status);
    ASSERT_NO_THROW(fetched_value = test_statusor.value()); //No badaccess exception
    ASSERT_EQ(fetched_value, expected_value);
    ASSERT_EQ(test_statusor.value_or(default_value), expected_value);
}

/// @brief Tests building StatusOr from error Status, Status is returned correctly with correct values and that fetching value throws
/// @details Tests that ok returns false correctly for bad access.
TEST(StatusOrTest, built_from_status_and_fetch){
    const bool expected_ok_status = false;
    const mygbc::Status::StatusType expected_status = mygbc::Status::StatusType::INVALID_INDEX_ERROR;
    const std::string expected_message = "BadAccessTest";
    const int default_value = 0x3;

    //Build error StatusOr
    mygbc::StatusOr<int> test_statusor(mygbc::Status::invalid_index_error(expected_message));
    //Check that status matches expected values
    ASSERT_EQ(test_statusor.get_status().get_type(), expected_status);
    ASSERT_EQ(test_statusor.get_status().get_message(), expected_message);
    //Check that ok is false and access throws
    ASSERT_EQ(test_statusor.ok(), expected_ok_status);
    ASSERT_THROW(test_statusor.value(), mygbc::BadStatusOrAccess);
    //Check that valueor returns given default value
    ASSERT_EQ(test_statusor.value_or(default_value), default_value);
}

