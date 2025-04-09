cmake_minimum_required(VERSION 3.22.1)
set(This gbcemu)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(Headers
    src/util.h
    src/gbc_binary.h
    src/binary_reader.h
    src/addressable_memory.h
)

set(Sources
    src/main.cc
    src/util.cc
    src/gbc_binary.cc
    src/binary_reader.cc
    src/addressable_memory.cc
)

add_executable(${This} ${Sources} ${Headers})