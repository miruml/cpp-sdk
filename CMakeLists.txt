cmake_minimum_required(VERSION 3.12)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
project(miru_sdk VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(fmt REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS asio beast)
find_package(nlohmann_json 3.11 REQUIRED)
find_package(yaml-cpp 0.8 REQUIRED)
find_package(GTest REQUIRED)

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
    miru/client/http_client.hpp
    miru/client/http_client.cpp
    miru/client/unix_socket.hpp
    miru/client/unix_socket.cpp
    miru/client/models/BaseConcreteConfig.h
    miru/client/models/BaseConcreteConfig.cpp
    miru/client/models/Error.h
    miru/client/models/Error.cpp
    miru/client/models/ErrorResponse.h
    miru/client/models/ErrorResponse.cpp
    miru/client/models/HashSchemaRequest.h
    miru/client/models/HashSchemaRequest.cpp
    miru/client/models/Helpers.h
    miru/client/models/Helpers.cpp
    miru/client/models/SchemaDigestResponse.h
    miru/client/models/SchemaDigestResponse.cpp
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
    yaml-cpp::yaml-cpp
    Boost::asio
    Boost::beast
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
        GTest::GTest
        GTest::Main
    )
    
    add_test(NAME miru_tests COMMAND miru_tests)
endif()

# Optional: Create example executable
if (BUILD_EXAMPLES OR BUILD_FILESYS_EXAMPLE)
    add_executable(filesys_example.out examples/filesys/main.cpp)
    target_link_libraries(filesys_example.out PRIVATE miru)
endif()

if (BUILD_EXAMPLES OR BUILD_SOCKET_EXAMPLE)
    add_executable(socket_example.out examples/socket/main.cpp)
    target_link_libraries(
        socket_example.out PRIVATE
        miru
        Boost::asio
    )
endif()
