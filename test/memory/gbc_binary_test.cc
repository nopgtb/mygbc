#include "../../src/memory/gbc_binary.h" //GBCBinary
#include <gtest/gtest.h> //GTest
#include <stdexcept> //std::out_of_range
#include <cstring> //Fixed lenght variables
#include <vector> //std::vector
#include <tuple> //std::tuple


//Definitions for constants
#define LOGO_START_ADDR 0x104 //INCLUSIVE
#define LOGO_END_ADDR 0x133 //INCLUSIVE
#define HEADER_START_ADDR 0x134 //INCLUSIVE
#define HEADER_END_ADDR 0x14F //INCLUSIVE
#define OK_SIZE_FOR_BINARY 0x200

/// @brief Helper function to build test data. 
/// @details Inserts data into a empty (0 values) vector at given point.
/// @param size_of_empty Size of the base vector.
/// @param insert_point Insert point of the data.
/// @param insert_value Data to insert.
/// @throws std::out_of_range if trying to insert too long data into empty vector.
/// @return Empty vector with data inserted into it at given index.
std::vector<uint8_t> insert_into_empty(const int size_of_empty, const int insert_point, std::vector<uint8_t> insert_value){
    if((size_of_empty - insert_point) >= insert_value.size()){
        std::vector<uint8_t> empty(size_of_empty, 0x0);
        std::copy(insert_value.begin(), insert_value.end(), empty.begin() + insert_point);
        return empty;
    }
    throw std::out_of_range("Tried to insert too long value!");
}

class GBCBinaryMinimumBinarySizeEnforcedTest : public ::testing::TestWithParam<std::vector<uint8_t>> {};

/// @brief Checks that GBCBinary::parse_bytes enforces minimum binary sizes correctly.
/// @details  GBCBinary::parse_bytes should gives error status when binary size is less than 0x14F.
TEST_P(GBCBinaryMinimumBinarySizeEnforcedTest, minimum_binary_size_enforced){
    std::vector<uint8_t> binary_data = GetParam();
    const bool expected_ok_status = false;
    const Status::StatusType expected_status = Status::StatusType::INVALID_BINARY_ERROR;
    StatusOr<GBCBinary> binary = GBCBinary::parse_bytes(binary_data);
    //Correctly fails invalid binary
    ASSERT_EQ(binary.ok(), expected_ok_status);
    ASSERT_EQ(binary.get_status().get_type(), expected_status);
}

/// @brief Initantiazation of minimum_binary_size_enforced.
/// @details  Initantiazation of minimum_binary_size_enforced.
INSTANTIATE_TEST_SUITE_P(
    minimum_binary_size_test_cases,
    GBCBinaryMinimumBinarySizeEnforcedTest,
    ::testing::Values(
        std::vector<uint8_t>{}, //Empty
        std::vector<uint8_t>((HEADER_END_ADDR - 0x01), 0x0) //Edge case 0x14E
    )
);

class GBCBinaryValidSizeTest : public ::testing::TestWithParam<std::vector<uint8_t>> {};

/// @brief Checks that GBCBinary::parse_bytes parses when the binary is of valid size >= 0x14F.
/// @details  GBCBinary::parse_bytes should not give error status when binary size is >= than 0x14F.
TEST_P(GBCBinaryValidSizeTest, valid_size_no_error){
    std::vector<uint8_t> binary_data = GetParam();
    const bool expected_ok_status = true;
    StatusOr<GBCBinary> binary = GBCBinary::parse_bytes(binary_data);
    //Parse result matches in size
    ASSERT_EQ(binary.ok(), expected_ok_status);
    ASSERT_EQ(binary.value().get_memory().size(), binary_data.size());
}

/// @brief Initantiazation of valid_size_no_error.
/// @details  Initantiazation of valid_size_no_error.
INSTANTIATE_TEST_SUITE_P(
    valid_size_no_error_test_cases,
    GBCBinaryValidSizeTest,
    ::testing::Values(
        std::vector<uint8_t>(HEADER_END_ADDR, 0x0), //Only logo and header
        std::vector<uint8_t>(OK_SIZE_FOR_BINARY, 0x0) //> 0x14F
    )
);

class GBCBinaryLogoTest : public ::testing::TestWithParam<std::tuple<std::vector<uint8_t>, bool>> {};

/// @brief Checks that GBCBinary::parse_bytes validates the logo (0x104=>0x133) in the binary correctly.
/// @details  GBCBinary::parse_bytes should return a object with correct logo_validity flag for the input.
TEST_P(GBCBinaryLogoTest, logo_validated_correctly){
    std::tuple<std::vector<uint8_t>, bool> test_data = GetParam();
    const bool expected_ok_status = true;
    StatusOr<GBCBinary> binary = GBCBinary::parse_bytes(std::get<0>(test_data));
    //Parses ok
    ASSERT_EQ(binary.ok(), expected_ok_status);
    //Logo check result matches expected
    ASSERT_EQ(binary.value().has_valid_logo(), std::get<1>(test_data));
}

/// @brief Initantiazation of logo_validated_correctly.
/// @details  Initantiazation of logo_validated_correctly.
INSTANTIATE_TEST_SUITE_P(
    logo_validated_correctly_test_cases,
    GBCBinaryLogoTest,
    ::testing::Values(
        std::make_tuple(std::vector<uint8_t>(OK_SIZE_FOR_BINARY, 0x0), false), //Not valid logo
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                LOGO_START_ADDR,
                std::vector<uint8_t>{
                    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03,
                    0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08,
                    0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E,
                    0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
                    0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 
                    0xB9, 0x33, 0x3B
                }
            ),
            false
        ), //Not valid logo, last byte changed
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                LOGO_START_ADDR,
                std::vector<uint8_t>{
                    0xCF, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03,
                    0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08,
                    0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E,
                    0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
                    0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 
                    0xB9, 0x33, 0x3E
                }
            ),
            false
        ), //Not valid logo, first byte changed
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                LOGO_START_ADDR,
                std::vector<uint8_t>{
                    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03,
                    0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 0x00, 0x08,
                    0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E,
                    0xE6, 0xDD, 0xDD, 0xD9, 0x99, 0xBB, 0xBB, 0x67, 0x63,
                    0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 
                    0xB9, 0x33, 0x3E
                }
            ),
            true
        ) //Valid logo
    )
);

class GBCBinaryHeaderValidityTest : public ::testing::TestWithParam<std::tuple<std::vector<uint8_t>, bool>> {};

/// @brief Checks that GBCBinary::parse_bytes validates the header (0x134=>0x14F) in the binary correctly.
/// @details  GBCBinary::parse_bytes should return a object with correct header_validity flag for the input.
TEST_P(GBCBinaryHeaderValidityTest, header_validated_correctly){
    std::tuple<std::vector<uint8_t>, bool> test_data = GetParam();
    const bool expected_ok_status = true;
    StatusOr<GBCBinary> binary = GBCBinary::parse_bytes(std::get<0>(test_data));
    //Parses ok
    ASSERT_EQ(binary.ok(), expected_ok_status);
    //header check result matches expected
    ASSERT_EQ(binary.value().has_valid_header(), std::get<1>(test_data));
}

/// @brief Initantiazation of header_validated_correctly.
/// @details  Initantiazation of header_validated_correctly.
INSTANTIATE_TEST_SUITE_P(
    header_validated_correctly_test_cases,
    GBCBinaryHeaderValidityTest,
    ::testing::Values(
        std::make_tuple(std::vector<uint8_t>(OK_SIZE_FOR_BINARY, 0x0), false), //Empty not valid header
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x61, 0x64, 0x64, 0x5f, 0x64, 0x65, 0x6d, 0x6f, 0x2e, 0x67,
                    0x62, 0x00, 0x00, 0x00, 0x00, 0x80, 0x37, 0x31, 0x00, 0x00,
                    0x01, 0x01, 0x00, 0x01, 0x02, 0xd5, 0x1d, 0x5B
                }
            ),
            false
        ), //Header data, fake checksum (hex byte, 3rd from end)
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x61, 0x64, 0x64, 0x5f, 0x64, 0x65, 0x6d, 0x6f, 0x2e, 0x67,
                    0x62, 0x00, 0x00, 0x00, 0x00, 0x80, 0x37, 0x31, 0x00, 0x00,
                    0x01, 0x01, 0x00, 0x01, 0x02, 0xd6, 0x1d, 0x5B
                }
            ),
            true
        ) //Header data, correct checksum (hex byte, 3rd from end)
    )
);

class GBCBinaryHeaderParsingTest : public ::testing::TestWithParam<std::tuple<std::vector<uint8_t>, GBCBinary::GBCBinaryHeaderData>> {};

/// @brief Checks that GBCBinary::parse_bytes validates the header (0x134=>0x14F) in the binary correctly.
/// @details  GBCBinary::parse_bytes should return a object with correct header_validity flag for the input.
TEST_P(GBCBinaryHeaderParsingTest, header_parsed_correctly){
    std::tuple<std::vector<uint8_t>, GBCBinary::GBCBinaryHeaderData> test_data = GetParam();
    const bool expected_ok_status = true;
    StatusOr<GBCBinary> binary = GBCBinary::parse_bytes(std::get<0>(test_data));
    //Parses ok
    ASSERT_EQ(binary.ok(), expected_ok_status);
    //header data matches expected
    ASSERT_TRUE(binary.value().get_header_data() == std::get<1>(test_data));
}

/// @brief Initantiazation of header_parsed_correctly.
/// @details  Initantiazation of header_parsed_correctly.
INSTANTIATE_TEST_SUITE_P(
    header_parsed_correctly_test_cases,
    GBCBinaryHeaderParsingTest,
    ::testing::Values(
        std::make_tuple(std::vector<uint8_t>(OK_SIZE_FOR_BINARY, 0x0), GBCBinary::GBCBinaryHeaderData()), //Empty header
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x61, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("a", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //title one letter, "a". rest 0
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 
                    0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("abcdefghijklmnop", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //title 16 letters, "abcdefghjklmno". rest 0. gameboy_type byte taken by title(WIKI).
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //gameboy_type value present
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 
                    0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x80, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("abcdefghijklmno", 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //title 15 letters, "abcdefghjklmno". gameboy_type value present (Not part of title).
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x37, 0x31, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //licensee new (2 bytes, interpeted as ASCII and combined into number)
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0x03, 0, 0, 0, 0, 0, 0, 0, 0)
        ), //Super gameboy compatability flag, 0x03 == Supported
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0x01, 0, 0, 0, 0, 0, 0, 0)
        ), //Cartridge type, 0x01 == MBC1
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0x01, 0, 0, 0, 0, 0, 0)
        ), //Rom size
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0, 0x01, 0, 0, 0, 0, 0)
        ), //Ram size
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0, 0, 0x01, 0, 0, 0, 0)
        ), //Japanese market code, 0x01 == Non japanese market
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x33, 0x00, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0, 0, 0, 0x33, 0, 0, 0)
        ), //Licensee code old, 0x33 == New used
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0, 0, 0, 0, 0x01, 0, 0)
        ), //Rom mask version
        std::make_tuple(
            insert_into_empty(
                OK_SIZE_FOR_BINARY,
                HEADER_START_ADDR,
                std::vector<uint8_t>{
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x37
                }
            ),
            GBCBinary::GBCBinaryHeaderData("", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5137)
        ) //Global checksum
    )
);