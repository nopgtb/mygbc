cmake_minimum_required(VERSION 3.22.1)
set(THIS gbcemu)
set(THIS_LIB libgbcemu)

project(${THIS} CXX)
set(CMAKE_CXX_STANDARD 20)

#Get source code
add_subdirectory(src)

#Build tests
enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG main #google declared the repo 'live at head' repo so pull the latest
)
FetchContent_MakeAvailable(googletest)
add_library(${THIS_LIB} STATIC ${SRC_SOURCES} ${SRC_HEADERS})
add_subdirectory(test)

#Build program
add_executable(${THIS} ${SRC_SOURCES} ${SRC_HEADERS})