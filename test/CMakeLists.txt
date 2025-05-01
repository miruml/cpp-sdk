enable_testing()

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE Miru_TEST_SOURCES CONFIGURE_DEPENDS ${Miru_SOURCE_DIR}/test/*.cpp)

add_executable(test ${Miru_TEST_SOURCES})

target_include_directories(test 
PRIVATE
    ${Miru_SOURCE_DIR}
    ${Miru_SOURCE_DIR}/src
)

target_link_libraries(test PRIVATE
    miru
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
    yaml-cpp::yaml-cpp
    Boost::asio
    Boost::beast
)

add_test(NAME test COMMAND test)