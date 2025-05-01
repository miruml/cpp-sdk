# 3.19+ is required for Apple Silicon
cmake_minimum_required(VERSION 3.19)

# CMAKE CONFIGURATIONS #
# ==================== #
# policies
cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0069 NEW)

# supress deprecation warnings from dependencies
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED_BEFORE "3.15" CACHE STRING "" FORCE)

project(miru VERSION 0.1.0 LANGUAGES CXX)

# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# OPTIONS #
# ======= #
option(MIRU_BUILD_TESTS "Build tests" ON)
option(MIRU_BUILD_EXAMPLES "Build examples" ON)


# DEPENDENCIES #
# ============= #
include(cmake/dependencies.cmake)


# SOURCE CODE #
# =========== #
include(cmake/source.cmake)

add_library(miru ${MIRU_SOURCES})
add_library(miru::miru ALIAS miru)

target_include_directories(miru PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(miru PRIVATE
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
    Boost::asio
    Boost::beast
)


# TESTS #
# ====== #
include(cmake/tests.cmake)
if (MIRU_BUILD_TESTS)
    build_miru_tests()
endif()


# EXAMPLES #
# ======== #
include(cmake/examples.cmake)
if (MIRU_BUILD_EXAMPLES)
    build_miru_examples()
endif()