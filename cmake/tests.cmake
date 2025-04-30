set(CONFIG_TEST_SOURCES
    test/config/config_test.cpp
)

set(FILESYS_TEST_SOURCES
    test/filesys/dir_test.cpp
    test/filesys/file_test.cpp
    test/filesys/path_test.cpp
)

set(HTTP_TEST_SOURCES
    test/http/mock.hpp
    test/http/mock.cpp
    test/http/socket_client_test.cpp
)

set(PARAMS_TEST_SOURCES
    # private
    test/params/details/composite_test.cpp
    test/params/details/parse_test.cpp
    test/params/details/scalar_test.cpp

    # public
    test/params/parameter_test.cpp
    test/params/utils_test.cpp
)

set(QUERY_TEST_SOURCES
    test/query/filter_test.cpp
    test/query/query_test.hpp
    test/query/query_test.cpp
    test/query/ros2_test.cpp
)

set(TEST_UTILS_SOURCES
    test/test_utils/query.hpp
    test/test_utils/testdata.hpp
    test/test_utils/testdata.cpp
    test/test_utils/utils.hpp
    test/test_utils/utils.cpp
)

set(MISC_TEST_SOURCES
    test/utils_test.cpp
    test/utils_test.hpp
)

set(ALL_TEST_SOURCES
    ${CONFIG_TEST_SOURCES}
    ${FILESYS_TEST_SOURCES}
    ${HTTP_TEST_SOURCES}
    ${PARAMS_TEST_SOURCES}
    ${QUERY_TEST_SOURCES}
    ${TEST_UTILS_SOURCES}
    ${MISC_TEST_SOURCES}
)

function(build_miru_tests)
    enable_testing()

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    )
    FetchContent_MakeAvailable(googletest)
    
    add_executable(test.out ${ALL_TEST_SOURCES})

    target_link_libraries(test.out
        PRIVATE
            miru
            GTest::gtest
            GTest::gtest_main
            nlohmann_json::nlohmann_json
            yaml-cpp::yaml-cpp
            Boost::asio
            Boost::beast
    )
    
    add_test(NAME test COMMAND test.out)
endfunction()