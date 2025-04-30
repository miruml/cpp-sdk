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

# # INSTALLATION #
# # ============ #
# include(GNUInstallDirs)

# # Install the library
# install(TARGETS miru
#     EXPORT miru-targets
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#     INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
# )

# # Install the headers
# install(DIRECTORY miru
#     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#     FILES_MATCHING PATTERN "*.hpp"
#     PATTERN "*.h"
# )

# # Generate and install CMake config files
# install(EXPORT miru-targets
#     FILE miru-targets.cmake
#     NAMESPACE miru::
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/miru
# )

# include(CMakePackageConfigHelpers)
# configure_package_config_file(
#     ${CMAKE_CURRENT_SOURCE_DIR}/cmake/miru-config.cmake.in
#     ${CMAKE_CURRENT_BINARY_DIR}/miru-config.cmake
#     INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/miru
# )

# write_basic_package_version_file(
#     ${CMAKE_CURRENT_BINARY_DIR}/miru-config-version.cmake
#     VERSION ${PROJECT_VERSION}
#     COMPATIBILITY SameMajorVersion
# )

# install(FILES
#     ${CMAKE_CURRENT_BINARY_DIR}/miru-config.cmake
#     ${CMAKE_CURRENT_BINARY_DIR}/miru-config-version.cmake
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/miru
# )
