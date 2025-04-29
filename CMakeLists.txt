cmake_minimum_required(VERSION 3.15)

# cmake policies
cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0069 NEW)

# supress deprecation warnings from dependencies
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED_BEFORE "3.15" CACHE STRING "" FORCE)

project(miru_sdk VERSION 0.1.0 LANGUAGES CXX)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# Debug flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /DEBUG")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
endif()

# Set optimization flags
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /DNDEBUG /Zi")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -g")
    endif()
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)
if(supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Use FetchContent for most dependencies
include(FetchContent)

# https://github.com/nlohmann/json?tab=readme-ov-file#embedded-fetchcontent
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

# https://github.com/jbeder/yaml-cpp?tab=readme-ov-file#how-to-integrate-it-within-your-project-using-cmake
FetchContent_Declare(
yaml-cpp
GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
GIT_TAG 0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

# fetch boost from system by default but allow FetchContent use if desire
option(MIRU_USE_SYSTEM_BOOST "Use system-installed Boost instead of FetchContent" ON)
if(MIRU_USE_SYSTEM_BOOST)
    find_package(Boost REQUIRED COMPONENTS asio beast)
else()
    FetchContent_Declare(
        boost
        URL https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.tar.gz
    )
    set(BOOST_INCLUDE_LIBRARIES asio beast)
    set(BOOST_ENABLE_CMAKE ON)
    FetchContent_MakeAvailable(boost)
endif()

add_library(miru
    miru/config/config.hpp
    miru/config/config.cpp
    miru/config/errors.hpp
    miru/filesys/dir.hpp
    miru/filesys/dir.cpp
    miru/filesys/errors.hpp
    miru/filesys/file.hpp
    miru/filesys/file.cpp
    miru/filesys/path.hpp
    miru/filesys/path.cpp
    miru/http/client.hpp
    miru/http/errors.hpp
    miru/http/errors.cpp
    miru/http/socket_client.hpp
    miru/http/socket_client.cpp
    miru/http/socket_session.hpp
    miru/http/socket_session.cpp
    miru/http/utils.hpp
    miru/http/utils.cpp
    miru/http/models/BaseConcreteConfig.h
    miru/http/models/BaseConcreteConfig.cpp
    miru/http/models/Error.h
    miru/http/models/Error.cpp
    miru/http/models/ErrorResponse.h
    miru/http/models/ErrorResponse.cpp
    miru/http/models/HashSchemaSerializedRequest.h
    miru/http/models/HashSchemaSerializedRequest.cpp
    miru/http/models/HashSerializedConfigSchemaFormat.h
    miru/http/models/HashSerializedConfigSchemaFormat.cpp
    miru/http/models/Helpers.h
    miru/http/models/Helpers.cpp
    miru/http/models/RefreshLatestConcreteConfigRequest.h
    miru/http/models/RefreshLatestConcreteConfigRequest.cpp
    miru/http/models/SchemaDigestResponse.h
    miru/http/models/SchemaDigestResponse.cpp
    miru/params/composite.hpp
    miru/params/composite.cpp
    miru/params/errors.hpp
    miru/params/iterator.hpp
    miru/params/parameter.hpp
    miru/params/parameter.cpp
    miru/params/parse.hpp
    miru/params/parse.cpp
    miru/params/scalar.cpp
    miru/params/type.hpp
    miru/params/value.hpp
    miru/params/value.cpp
    miru/params/utils.hpp
    miru/params/utils.cpp
    miru/query/filter.hpp
    miru/query/filter.cpp
    miru/query/query.hpp
    miru/query/query.cpp
    miru/query/ros2.hpp
    miru/query/ros2.cpp
    miru/errors.hpp
    miru/errors.cpp
    miru/utils.hpp
    miru/utils.cpp
)

# Add include directories
target_include_directories(miru PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link dependencies
target_link_libraries(miru PRIVATE
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
    Boost::asio
    Boost::beast
)

# Make tests optional
if(BUILD_TESTING)
    enable_testing()

    find_package(GTest REQUIRED)
    
    add_executable(
        test.out
        test/config/config_test.cpp
        test/filesys/dir_test.cpp
        test/filesys/file_test.cpp
        test/filesys/path_test.cpp
        test/http/socket_client_test.cpp
        test/http/mock.hpp
        test/http/mock.cpp
        test/params/composite_test.cpp
        test/params/parse_test.cpp
        test/params/scalar_test.cpp
        test/params/parameter_test.cpp
        test/params/utils_test.cpp
        test/query/filter_test.cpp
        test/query/query_test.hpp
        test/query/query_test.cpp
        test/query/ros2_test.cpp
        test/test_utils/testdata.hpp
        test/test_utils/testdata.cpp
        test/test_utils/utils.hpp
        test/test_utils/utils.cpp
        test/utils_test.cpp
    )

    target_link_libraries(test.out
        PRIVATE
        miru
        GTest::GTest
        GTest::Main
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        Boost::asio
        Boost::beast
    )
    
    add_test(NAME test COMMAND test.out)
endif()

# Optional: Create example executable
# if (BUILD_EXAMPLES OR BUILD_FILESYS_EXAMPLE)
#     add_executable(filesys_example.out examples/filesys/main.cpp)
#     target_link_libraries(filesys_example.out PRIVATE miru)
# endif()