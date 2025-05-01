# Ubuntu 20.04 ships with CMake 3.16 by default
cmake_minimum_required(VERSION 3.16)

# CMAKE CONFIGURATIONS #
# ==================== #
# policies
cmake_policy(SET CMP0069 NEW)
if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.24)
    cmake_policy(SET CMP0135 NEW)
endif()

# supress deprecation warnings from dependencies
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_WARN_DEPRECATED_BEFORE "3.15" CACHE STRING "" FORCE)

project(
    miru 
    VERSION 0.1.0 
    DESCRIPTION "Miru SDK for retrieving configurations from the miru agent"
    LANGUAGES CXX)
include(cmake/versioning.cmake)

# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# used by CMake files in subdirectories
set(Miru_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(Miru_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# OPTIONS #
# ======= #
option(MIRU_BUILD_TESTS "Build tests" ON)
option(MIRU_BUILD_EXAMPLES "Build examples" ON)
option(MIRU_FETCH_BOOST "fetch Boost packages with FetchContent as opposed to using the Boost package located on the build system" ON)

# add BUILD_TESTING option for convention purposs
if (CMAKE_PROJECT_NAME STREQUAL "miru" AND BUILD_TESTING)
    message(STATUS "Miru: project is 'miru' and BUILD_TESTING is ON, setting MIRU_BUILD_TESTS to ON")
    set(MIRU_BUILD_TESTS ON)
endif()


# DEPENDENCIES #
# ============= #
include(cmake/dependencies.cmake)


# SOURCE CODE #
# =========== #
add_subdirectory(src)


# TESTS #
# ===== #
if (MIRU_BUILD_TESTS)
    if (CMAKE_PROJECT_NAME STREQUAL "miru")
        message(STATUS "Miru: building tests")
    endif()
    add_subdirectory(test)
else()
    if (CMAKE_PROJECT_NAME STREQUAL "miru")
        message(STATUS "Miru: skipping tests")
    endif()
endif()


# EXAMPLES #
# ======== #
if (MIRU_BUILD_EXAMPLES)
    if (CMAKE_PROJECT_NAME STREQUAL "miru")
        message(STATUS "Miru: building examples")
    endif()
    add_subdirectory(examples)
else()
    if (CMAKE_PROJECT_NAME STREQUAL "miru")
        message(STATUS "Miru: skipping examples")
    endif()
endif()