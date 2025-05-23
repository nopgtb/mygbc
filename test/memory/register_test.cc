#include "../../src/memory/register_16bit.h" //Register
#include <gtest/gtest.h> //GTest
#include <vector> //std::vector
#include <tuple> //std::tuple

/// @brief Checks that register size is 2 bytes.
TEST(RegisterSizeTest, register_size_is_correct){
    const int expected_register_size = 2;
    mygbc::Register16Bit A;
    ASSERT_EQ(A.get_memory_size(), expected_register_size);
}

class RegisterReadBitsTest : public ::testing::TestWithParam<std::tuple<std::vector<bool>, uint16_t>> {};

/// @brief Checks that Register16Bit delivers right value when reading bits.
/// @details Checks that Register16Bit delivers right value when reading bits.
TEST_P(RegisterReadBitsTest, read_bits_test){
    const bool expected_ok_status = true;
    std::tuple<std::vector<bool>, uint16_t> test_values = GetParam();
    mygbc::Register16Bit A;
    uint16_t test_pattern = std::get<1>(test_values);
    A.set_word(test_pattern);
    std::vector<bool> expected = std::get<0>(test_values);
    //Go trough each bit and see if they match
    for (int byte_index = 0; byte_index < 2; ++byte_index){
        for(int bit_index = 0; bit_index < 8; ++bit_index){
            mygbc::StatusOr<bool> read_result = A.get_bit(byte_index, bit_index);
            ASSERT_EQ(read_result.ok(), expected_ok_status);
            ASSERT_EQ(read_result.value(), expected[(byte_index*8) + bit_index]);
        }
    }
}

/// @brief Initantiazation of read_bits_test.
/// @details  Initantiazation of read_bits_test.
INSTANTIATE_TEST_SUITE_P(
    read_bits_test_test_cases,
    RegisterReadBitsTest,
    ::testing::Values(
        std::make_tuple(std::vector<bool>(16, false), 0x0000), //All 0 bits, all false as result
        std::make_tuple(std::vector<bool>(16, true), 0xFFFF), //All 1 bits, all true as result
        std::make_tuple(
            std::vector<bool>{
                false, true, false, true, false, true, false, true,
                false, true, false, true, false, true, false, true,
            },
            0xAAAA
        ), //1010101010101010 Pattern
        std::make_tuple(
            std::vector<bool>{
                true, false, true, false, true, false, true, false,
                true, false, true, false, true, false, true, false
            },
            0x5555
        ) //0101010101010101 Pattern
    )
);

/// @brief Test that read bit returns error on invalid byte and bit indexes
/// @details Test all the edge cases for unaceptable indexes to registry bit read
TEST(RegisterReadBitsTest, invalid_index_bit_read_test){
    const bool expected_ok_status = false;
    const mygbc::Status::StatusType expected_status = mygbc::Status::StatusType::INVALID_INDEX_ERROR;
    mygbc::Register16Bit A;
    uint16_t test_pattern = 0x0000;
    A.set_word(test_pattern);

    std::vector<std::tuple<uint8_t, uint8_t>> invalid_indexes = {
        std::make_tuple(0, 8), //Above accepted bit range, byte ok
        std::make_tuple(3, 7), //Above accepted byte range, bit ok
        std::make_tuple(3, 8) //Both unacceptable
    };

    for(int i = 0; i < invalid_indexes.size(); ++i){
        uint8_t byte_index = std::get<0>(invalid_indexes[i]);
        uint8_t bit_index = std::get<1>(invalid_indexes[i]);
        mygbc::StatusOr<bool> read_result = A.get_bit(byte_index, bit_index);
        //Check that read fails with correct status
        ASSERT_EQ(read_result.ok(), expected_ok_status);
        ASSERT_EQ(read_result.status().code(), expected_status);
    }
}

struct RegisterWriteBitsTestTestData{
    uint16_t initial_value;
    uint16_t expected_value;
    uint8_t byte_index;
    uint8_t bit_index;
    bool set_value;

    /// @brief Set constructor for RegisterWriteBitsTestTestData
    /// @param init_val Initial value.
    /// @param except_val Expected value after set.
    /// @param byte_indx Set byte index
    /// @param bit_indx Set bit index
    /// @param new_value Set new value
    RegisterWriteBitsTestTestData(uint16_t init_val, uint16_t except_val, uint8_t byte_indx, uint8_t bit_indx, bool new_value)
    :initial_value(init_val), expected_value(except_val), byte_index(byte_indx), bit_index(bit_indx), set_value(new_value){
    }
};
class RegisterWriteBitsTest : public ::testing::TestWithParam<RegisterWriteBitsTestTestData> {};

/// @brief Checks that Register16Bit delivers right value when reading bits.
/// @details Checks that Register16Bit delivers right value when reading bits.
TEST_P(RegisterWriteBitsTest, write_bits_test){
    RegisterWriteBitsTestTestData test_values = GetParam();
    mygbc::Register16Bit A;
    bool expected_ok_status = true;
    A.set_word(test_values.initial_value);
    mygbc::Status bit_set = A.set_bit(test_values.byte_index, test_values.bit_index, test_values.set_value);
    ASSERT_EQ(bit_set.ok(), expected_ok_status);
    uint16_t read_result = A.get_word();
    ASSERT_EQ(read_result, test_values.expected_value);
}

/// @brief Initantiazation of write_bits_test.
/// @details  Initantiazation of write_bits_test.
INSTANTIATE_TEST_SUITE_P(
    write_bits_test_test_cases,
    RegisterWriteBitsTest,
    ::testing::Values(
        //Set Each bit to 1
        RegisterWriteBitsTestTestData(0x0000, 0x0001, 0x1, 0x0, true), //0000000000000001
        RegisterWriteBitsTestTestData(0x0000, 0x0002, 0x1, 0x1, true), //0000000000000010
        RegisterWriteBitsTestTestData(0x0000, 0x0004, 0x1, 0x2, true), //0000000000000100
        RegisterWriteBitsTestTestData(0x0000, 0x0008, 0x1, 0x3, true), //0000000000001000
        RegisterWriteBitsTestTestData(0x0000, 0x0010, 0x1, 0x4, true), //0000000000010000
        RegisterWriteBitsTestTestData(0x0000, 0x0020, 0x1, 0x5, true), //0000000000100000
        RegisterWriteBitsTestTestData(0x0000, 0x0040, 0x1, 0x6, true), //0000000001000000
        RegisterWriteBitsTestTestData(0x0000, 0x0080, 0x1, 0x7, true), //0000000010000000
        RegisterWriteBitsTestTestData(0x0000, 0x0100, 0x0, 0x0, true), //0000000100000000
        RegisterWriteBitsTestTestData(0x0000, 0x0200, 0x0, 0x1, true), //0000001000000000
        RegisterWriteBitsTestTestData(0x0000, 0x0400, 0x0, 0x2, true), //0000010000000000
        RegisterWriteBitsTestTestData(0x0000, 0x0800, 0x0, 0x3, true), //0001000000000000
        RegisterWriteBitsTestTestData(0x0000, 0x1000, 0x0, 0x4, true), //0010000000000000
        RegisterWriteBitsTestTestData(0x0000, 0x2000, 0x0, 0x5, true), //0100000000000000
        RegisterWriteBitsTestTestData(0x0000, 0x4000, 0x0, 0x6, true), //0100000000000000
        RegisterWriteBitsTestTestData(0x0000, 0x8000, 0x0, 0x7, true), //1000000000000000
        //Set each bit to 0
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFFE, 0x1, 0x0, false), //1111111111111110
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFFD, 0x1, 0x1, false), //1111111111111101
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFFB, 0x1, 0x2, false), //1111111111111011
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFF7, 0x1, 0x3, false), //1111111111110111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFEF, 0x1, 0x4, false), //1111111111101111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFDF, 0x1, 0x5, false), //1111111111011111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFFBF, 0x1, 0x6, false), //1111111110111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFF7F, 0x1, 0x7, false), //1111111101111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFEFF, 0x0, 0x0, false), //1111111011111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFDFF, 0x0, 0x1, false), //1111110111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xFBFF, 0x0, 0x2, false), //1111101111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xF7FF, 0x0, 0x3, false), //1111011111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xEFFF, 0x0, 0x4, false), //1110111111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xDFFF, 0x0, 0x5, false), //1101111111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0xBFFF, 0x0, 0x6, false), //1011111111111111
        RegisterWriteBitsTestTestData(0xFFFF, 0x7FFF, 0x0, 0x7, false)  //0111111111111111
    )
);

/// @brief Test that write bit returns error on invalid byte and bit indexes
/// @details Test all the edge cases for unaceptable indexes to registry bit write
TEST(RegisterWriteBitsTest, invalid_index_bit_write_test){
    const mygbc::Status::StatusType expected_status = mygbc::Status::StatusType::INVALID_INDEX_ERROR;
    mygbc::Register16Bit A;
    uint16_t test_pattern = 0x0000;
    A.set_word(test_pattern);

    std::vector<std::tuple<uint8_t, uint8_t>> invalid_indexes = {
        std::make_tuple(0, 8), //Above accepted bit range, byte ok
        std::make_tuple(3, 7), //Above accepted byte range, bit ok
        std::make_tuple(3, 8) //Both unacceptable
    };

    for(int i = 0; i < invalid_indexes.size(); ++i){
        uint8_t byte_index = std::get<0>(invalid_indexes[i]);
        uint8_t bit_index = std::get<1>(invalid_indexes[i]);
        mygbc::Status write_test = A.set_bit(byte_index, bit_index, false);
        //Check that write fails with correct status
        ASSERT_EQ(write_test.code(), expected_status);
    }
}