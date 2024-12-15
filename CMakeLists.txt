cmake_minimum_required(VERSION 3.29)
project(Lab4)

set(CMAKE_CXX_STANDARD 20)


# Add GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
)
FetchContent_MakeAvailable(googletest)


add_executable(Lab4 main.cpp
        hash_dictionary.hpp)

# Link GoogleTest
target_link_libraries(Lab4 gtest_main)

# Enable testing
enable_testing()
add_test(NAME Lab4 COMMAND Lab4)