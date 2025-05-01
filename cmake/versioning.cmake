# generate the version.h file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/miru/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/miru/version.hpp
    @ONLY
)

# generate the commit.hpp file
execute_process(
    COMMAND git rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if (NOT GIT_COMMIT_HASH)
    message(WARNING "Miru: failed to determine git commit hash")
    set(MIRU_COMMIT "unknown")
else()
    set(MIRU_COMMIT ${GIT_COMMIT_HASH})
endif()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/miru/commit.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/include/miru/commit.hpp
    @ONLY
)