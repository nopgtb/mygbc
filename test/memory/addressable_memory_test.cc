#include "../../src/memory/addressable_memory.h" //GBCBinary
#include <gtest/gtest.h> //GTest
#include <vector> //std::vector
#include <tuple> //std::tuple

//Definitions
#define OK_MEMORY_SIZE 0x200

class AddressableMemoryMemoryCreationTest : public ::testing::TestWithParam<std::tuple<std::vector<uint8_t>, bool>> {};

/// @brief Checks that AddressableMemory is created correctly based on the inputs.
/// @details  Memory contents matches input and so does the protection flag.
TEST_P(AddressableMemoryMemoryCreationTest, memory_created_correctly){
    std::tuple<std::vector<uint8_t>, bool> test_data = GetParam();
    AddressableMemory memory(std::get<0>(test_data), std::get<1>(test_data));
    ASSERT_EQ(memory.get_memory(), std::get<0>(test_data));
    ASSERT_EQ(memory.is_read_only(), std::get<1>(test_data));
}

/// @brief Initantiazation of minimum_binary_size_enforced.
/// @details  Initantiazation of minimum_binary_size_enforced.
INSTANTIATE_TEST_SUITE_P(
    memory_created_correctly_test_cases,
    AddressableMemoryMemoryCreationTest,
    ::testing::Values(
        std::make_tuple(std::vector<uint8_t>(), false), //Empty memory and not protected
        std::make_tuple(std::vector<uint8_t>(OK_MEMORY_SIZE, 0x00), true) //Not empty memory, and protected
    )
);

/// @brief Checks that AddressableMemory fetches byte sized data correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemoryReadTest, read_byte_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint8_t expected_result = 0x01;
    const uint16_t read_addr = 0x01;
    const bool ok_status_expected = true;
    StatusOr<uint8_t> read_result;
    read_result = memory.get_byte(read_addr);
    ASSERT_EQ(read_result.ok(), ok_status_expected);
    ASSERT_EQ(read_result.value(), expected_result);
}

/// @brief Checks that AddressableMemory gives error status on invalid address byte fetch.
/// @details  gives error status on invalid address byte fetch.
TEST(AddressableMemoryReadTest, read_byte_invalid_addr_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint16_t read_addr = 0x04;
    const bool ok_status_expected = false;
    StatusOr<uint8_t> read_result = memory.get_byte(read_addr);
    ASSERT_EQ(read_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory fetches word sized data correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemoryReadTest, read_word_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint16_t expected_result = 0x0102;
    const uint16_t read_addr = 0x01;
    const bool ok_status_expected = true;
    StatusOr<uint16_t> read_result = memory.get_word(read_addr);
    ASSERT_EQ(read_result.ok(), ok_status_expected);
    ASSERT_EQ(read_result.value(), expected_result);
}

/// @brief Checks that AddressableMemory gives error status on invalid address word fetch.
/// @details  gives error status on invalid address word fetch.
TEST(AddressableMemoryReadTest, read_word_invalid_addr_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint16_t read_addr = 0x03;
    const bool ok_status_expected = false;
    StatusOr<uint16_t> read_result = memory.get_word(read_addr);
    ASSERT_EQ(read_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory sets byte sized data correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemorySetTest, set_byte_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const std::vector<uint8_t> expected_result{0x00, 0x05, 0x02};
    const uint16_t set_addr = 0x01;
    const uint8_t set_value = 0x05;
    const bool ok_status_expected = true;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
    ASSERT_EQ(memory.get_memory(), expected_result);
}

/// @brief Checks that AddressableMemory sets word sized data correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemorySetTest, set_word_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const std::vector<uint8_t> expected_result{0x00, 0x05, 0x04};
    const uint16_t set_addr = 0x01;
    const uint16_t set_value = 0x0504;
    const bool ok_status_expected = true;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
    ASSERT_EQ(memory.get_memory(), expected_result);
}

/// @brief Checks that AddressableMemory sets the whole memorydata correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemorySetTest, set_memory_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const std::vector<uint8_t> expected_result{0x00, 0x05, 0x04};
    const bool ok_status_expected = true;
    Status write_result = memory.set_memory(expected_result);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
    ASSERT_EQ(memory.get_memory(), expected_result);
}

/// @brief Checks that AddressableMemory gives error status when trying to set byte in protected memory.
/// @details gives error status when trying to set byte in protected memory.
TEST(AddressableMemorySetTest, set_byte_protected_memory_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, true);
    const uint16_t set_addr = 0x01;
    const uint8_t set_value = 0x05;
    const bool ok_status_expected = false;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory gives error status when trying to set word in protected memory.
/// @details gives error status when trying to set word in protected memory.
TEST(AddressableMemorySetTest, set_word_protected_memory_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, true);
    const uint16_t set_addr = 0x01;
    const uint16_t set_value = 0x0575;
    const bool ok_status_expected = false;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory sets the whole memorydata correctly.
/// @details  Result of the fetch matches the expected value.
TEST(AddressableMemorySetTest, set_memory_protected_memory_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, true);
    const std::vector<uint8_t> expected_result{0x00, 0x05, 0x04};
    const bool ok_status_expected = false;
    Status write_result = memory.set_memory(expected_result);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory gives error status when trying to set byte with invalid addr.
/// @details gives error status when trying to set byte with invalid addr.
TEST(AddressableMemorySetTest, set_byte_invalid_addr_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint16_t set_addr = 0x03;
    const uint8_t set_value = 0x05;
    const bool ok_status_expected = false;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory gives error status when trying to set word with invalid addr.
/// @details gives error status when trying to set word with invalid addr.
TEST(AddressableMemorySetTest, set_word_invalid_addr_test){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    const uint16_t set_addr = 0x03;
    const uint16_t set_value = 0x0575;
    const bool ok_status_expected = false;
    Status write_result = memory.set(set_addr, set_value);
    ASSERT_EQ(write_result.ok(), ok_status_expected);
}

/// @brief Checks that AddressableMemory frees memory correctly.
/// @details Used memory is freed correctly when .free is called.
TEST(AddressableMemoryTearDownTest, free_memory_test){
    std::vector<uint8_t> contents {0x00, 0x01, 0x02};
    AddressableMemory memory(contents, false);
    ASSERT_EQ(memory.get_memory(), contents);
    memory.free();
    ASSERT_EQ(memory.get_memory(), std::vector<uint8_t>());
    ASSERT_EQ(memory.get_memory().size(), 0);
}

/// @brief Checks that AddressableMemory frees memory correctly.
/// @details Used memory is freed correctly when .free is called.
TEST(AddressableMemoryChangeProtectedMemoryFlag, ChangeProtectedMemoryFlag){
    AddressableMemory memory(std::vector<uint8_t>{0x00, 0x01, 0x02}, false);
    ASSERT_EQ(memory.is_read_only(), false);
    memory.set_read_only_flag(true);
    ASSERT_EQ(memory.is_read_only(), true);
}