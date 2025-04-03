cmake_minimum_required(VERSION 3.14)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(miru_sdk VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# At the top of your CMakeLists.txt

# Find required packages
find_package(fmt REQUIRED)
find_package(nlohmann_json 3.11 REQUIRED)
find_package(yaml-cpp 0.8 REQUIRED)
find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.3
)
FetchContent_MakeAvailable(magic_enum)

add_library(miru
    miru/config.hpp
    miru/config.cpp
    miru/filesys/file.hpp
    miru/filesys/file.cpp
    miru/filesys/path.hpp
    miru/filesys/path.cpp
    miru/filesys/dir.hpp
    miru/filesys/dir.cpp
    miru/filesys/json.hpp
    miru/filesys/yaml.hpp
    miru/unix_socket_client.hpp
    miru/unix_socket_client.cpp
)

# Add include directories
target_include_directories(miru PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${GTEST_INCLUDE_DIRS}
)

# Link dependencies
target_link_libraries(miru PRIVATE
    fmt::fmt
    magic_enum::magic_enum
    nlohmann_json::nlohmann_json
    yaml-cpp
)

# Make tests optional
if(BUILD_TESTING)
    enable_testing()
    
    add_executable(
        miru_tests.out
        test/filesys/file_test.cpp
        test/filesys/path_test.cpp
        test/filesys/dir_test.cpp
        test/test_utils/utils.hpp
        test/test_utils/utils.cpp
        test/test_utils/testdata.hpp
        test/test_utils/testdata.cpp
    )
    
    target_link_libraries(miru_tests.out
        PRIVATE
        miru
        fmt::fmt
        GTest::GTest
        GTest::Main
    )
    
    add_test(NAME miru_tests COMMAND miru_tests)
endif()

# Optional: Create example executable
if (BUILD_EXAMPLES)
    add_executable(example.out examples/main.cpp)
    target_link_libraries(example.out PRIVATE miru)
endif()
