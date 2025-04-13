cmake_minimum_required(VERSION 3.22.1)

set(THIS gbcemu_test)
set(THIS_LIB libgbcemu)

set(TEST_SOURCES
    gbc_binary_test.cc
    addressable_memory_test.cc
)

add_executable(${THIS} ${TEST_SOURCES})
target_link_libraries(${THIS} PUBLIC
    gtest_main
    ${THIS_LIB}
)

add_test(
    NAME ${THIS}
    COMMAND ${THIS}
)