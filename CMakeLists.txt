cmake_minimum_required(VERSION 3.22.1)
set(This gbcemu)
project(${This} C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(Headers
    src/util/util.h
    src/memory/gbc_binary.h
    src/util/binary_reader.h
    src/memory/addressable_memory.h
)

set(Sources
    src/main.cc
    src/util/util.cc
    src/memory/gbc_binary.cc
    src/util/binary_reader.cc
    src/memory/addressable_memory.cc
)

add_executable(${This} ${Sources} ${Headers})