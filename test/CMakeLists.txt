cmake_minimum_required(VERSION 3.22.1)

set(THIS mygbc_test)
set(THIS_LIB libmygbc)

set(TEST_SOURCES
    memory/gbc_binary_test.cc
    memory/addressable_memory_test.cc
    memory/register_test.cc
    util/util_test.cc
    util/status/status_test.cc
    util/status/status_or_test.cc
    instruction_set_lr35902/instruction_decoder_lr35902_test.cc
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