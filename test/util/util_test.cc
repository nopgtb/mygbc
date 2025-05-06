#include "../../src/util/util.h" //Util
#include <gtest/gtest.h> //GTest
#include <string> //std::string
#include <tuple> //std::tuple


class UtilCombinedCharBasedValueTest : public ::testing::TestWithParam<std::tuple<std::tuple<uint8_t, uint8_t>, uint8_t>> {};

/// @brief Checks that Util::combined_char_based_value produces expected values.
/// @details combined_char_based_value interps the values as number charecters (ASCII)
///         and combines the values as string. The string is then cast as uint8_t, giving the final value.
TEST_P(UtilCombinedCharBasedValueTest, combine_value_test){
    std::tuple<std::tuple<uint8_t, uint8_t>, uint8_t> test_values = GetParam();
    const bool expected_ok_status = true;
    mygbc::StatusOr<uint8_t> result = mygbc::Util::combined_char_based_value(
        std::get<0>(std::get<0>(test_values)),
        std::get<1>(std::get<0>(test_values))
    );
    ASSERT_EQ(result.ok(), expected_ok_status);
    ASSERT_EQ(std::get<1>(test_values), result.value());
}

/// @brief Initantiazation of combine_value_test.
/// @details  Initantiazation of combine_value_test.
INSTANTIATE_TEST_SUITE_P(
    combine_value_test_test_cases,
    UtilCombinedCharBasedValueTest,
    ::testing::Values(
        std::make_tuple(std::make_tuple(0x30, 0x30), 0), //0 + 0 = 00
        std::make_tuple(std::make_tuple(0x31, 0x30), 10), //1 + 0 = 10
        std::make_tuple(std::make_tuple(0x32, 0x30), 20), //2 + 0 = 20
        std::make_tuple(std::make_tuple(0x33, 0x30), 30), //3 + 0 = 30
        std::make_tuple(std::make_tuple(0x34, 0x30), 40), //4 + 0 = 40
        std::make_tuple(std::make_tuple(0x35, 0x30), 50), //5 + 0 = 50
        std::make_tuple(std::make_tuple(0x36, 0x30), 60), //6 + 0 = 60
        std::make_tuple(std::make_tuple(0x37, 0x30), 70), //7 + 0 = 70
        std::make_tuple(std::make_tuple(0x38, 0x30), 80), //8 + 0 = 80
        std::make_tuple(std::make_tuple(0x39, 0x30), 90), //9 + 0 = 90
        std::make_tuple(std::make_tuple(0x30, 0x31), 1), //0 + 1 = 01
        std::make_tuple(std::make_tuple(0x30, 0x32), 2), //0 + 2 = 02
        std::make_tuple(std::make_tuple(0x30, 0x33), 3), //0 + 3 = 03
        std::make_tuple(std::make_tuple(0x30, 0x34), 4), //0 + 4 = 04
        std::make_tuple(std::make_tuple(0x30, 0x35), 5), //0 + 5 = 05
        std::make_tuple(std::make_tuple(0x30, 0x36), 6), //0 + 6 = 06
        std::make_tuple(std::make_tuple(0x30, 0x37), 7), //0 + 7 = 07
        std::make_tuple(std::make_tuple(0x30, 0x38), 8), //0 + 8 = 08
        std::make_tuple(std::make_tuple(0x30, 0x39), 9)  //0 + 9 = 09
    )
);

/// @brief Checks that Util::combined_char_based_value gives error status when given invalid values (not number(ASCII)).
/// @details combined_char_based_value gives error status when either of the value is less than 0x30 or more than 0x39.
TEST(UtilCombinedCharBasedValueTest, error_status_on_invalid_values){
    const bool expected_ok_status = false;
    const mygbc::Status::StatusType expected_status = mygbc::Status::StatusType::INVALID_INPUT_ERROR;
    mygbc::StatusOr<uint8_t> out_of_bound_value1 = mygbc::Util::combined_char_based_value(0x29, 0x30);
    mygbc::StatusOr<uint8_t> out_of_bound_value2 = mygbc::Util::combined_char_based_value(0x30, 0x29);
    ASSERT_EQ(out_of_bound_value1.ok(), expected_ok_status);
    ASSERT_EQ(out_of_bound_value2.ok(), expected_ok_status);
    ASSERT_EQ(out_of_bound_value1.status().code(), expected_status);
    ASSERT_EQ(out_of_bound_value2.status().code(), expected_status);
}

class UtilTrimTrailingNullBytesTest : public ::testing::TestWithParam<std::tuple<std::string, std::string>> {};

/// @brief Checks that Util::trim_trailing_null_bytes produces expected values.
/// @details The function should remove trailing null(0x0) value bytes and return the remaining string.
TEST_P(UtilTrimTrailingNullBytesTest, trim_test){
    std::tuple<std::string, std::string> test_values = GetParam();
    std::string trim_result = std::get<0>(test_values);
    ASSERT_NO_THROW(trim_result = mygbc::Util::trim_trailing_null_bytes(trim_result));
    ASSERT_EQ(std::get<1>(test_values), trim_result);
}

/// @brief Initantiazation of combine_value_test.
/// @details  Initantiazation of combine_value_test.
INSTANTIATE_TEST_SUITE_P(
    trim_test_test_cases,
    UtilTrimTrailingNullBytesTest,
    ::testing::Values(
        std::make_tuple("", ""),
        std::make_tuple("\0\0\0", ""),
        std::make_tuple("abc\0", "abc"),
        std::make_tuple("abc\0\0\0", "abc"),
        std::make_tuple("\0abc", "\0abc")
    )
);