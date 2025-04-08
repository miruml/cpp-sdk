#pragma once

// std
#include <cstdint>
#include <variant>

// internal
#include <miru/params/parameter.hpp>

// external
#include <gtest/gtest.h>


using IntVariants = std::variant<
    int8_t,
    int16_t,
    int32_t,
    int64_t,
    uint8_t,
    uint16_t,
    uint32_t,
    uint64_t
>;

using DoubleVariants = std::variant<
    float,
    double
>;

// ================================== INTEGERS ===================================== //
struct IntCastTestCase {
    int64_t input;
    IntVariants expected;
    bool expect_failure;
};

class IntegerCasting: public ::testing::Test {
protected:
    std::vector<IntCastTestCase> int8_test_cases = {
        // int8 success
        {123, int8_t(123), false},
        {-123, int8_t(-123), false},

        // int8 overflow
        {128, int8_t(0), true},
        {-129, int8_t(0), true},
    };

    std::vector<IntCastTestCase> int16_test_cases = {
        // int16 success
        {32767, int16_t(32767), false},
        {-32768, int16_t(-32768), false},

        // int16 overflow
        {32768, int16_t(0), true},
        {-32769, int16_t(0), true},
    };

    std::vector<IntCastTestCase> int32_test_cases = {
        // int32 success
        {2147483647, int32_t(2147483647), false},
        {-2147483648, int32_t(-2147483648), false},
        
        // int32 overflow
        {2147483648, int32_t(0), true},
        {-2147483649, int32_t(0), true},
    };

    std::vector<IntCastTestCase> int64_test_cases = {
        // int64 success
        {9223372036854775807, int64_t(9223372036854775807), false},
        {-9223372036854775807, int64_t(-9223372036854775807), false},
        
        // can't test overflow of int64_t from int64_t
    };

    std::vector<IntCastTestCase> uint8_test_cases = {
        // uint8 success
        {255, uint8_t(255), false},
        {0, uint8_t(0), false},

        // uint8 overflow
        {256, uint8_t(0), true},
        {-1, uint8_t(0), true},
    };

    std::vector<IntCastTestCase> uint16_test_cases = {
        // uint16 success
        {65535, uint16_t(65535), false},
        {0, uint16_t(0), false},

        // uint16 overflow
        {65536, uint16_t(0), true},
        {-1, uint16_t(0), true},
    };

    std::vector<IntCastTestCase> uint32_test_cases = {
        // uint32 success
        {4294967295, uint32_t(4294967295), false},
        {0, uint32_t(0), false},

        // uint32 overflow
        {4294967296, uint32_t(0), true},
        {-1, uint32_t(0), true},
    };

    std::vector<IntCastTestCase> uint64_test_cases = {
        // uint64 success
        {9223372036854775807, uint64_t(9223372036854775807), false},
        {0, uint64_t(0), false},

        // uint64 overflow
        {-1, uint64_t(0), true},
    };

    std::vector<IntCastTestCase> all_test_cases() {
        std::vector<IntCastTestCase> all_test_cases = {};
        all_test_cases.insert(all_test_cases.end(), int8_test_cases.begin(), int8_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), int16_test_cases.begin(), int16_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), int32_test_cases.begin(), int32_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), int64_test_cases.begin(), int64_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), uint8_test_cases.begin(), uint8_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), uint16_test_cases.begin(), uint16_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), uint32_test_cases.begin(), uint32_test_cases.end());
        all_test_cases.insert(all_test_cases.end(), uint64_test_cases.begin(), uint64_test_cases.end());
        return all_test_cases;
    }
};

// =================================== SCALARS ===================================== //
struct ScalarToBoolTestCase {
    std::string scalar;
    bool expected;
    bool expect_failure;
};

struct ScalarToIntTestCase {
    std::string scalar;
    IntVariants expected;
    bool expect_failure;
};

struct ScalarToDoubleTestCase {
    std::string scalar;
    DoubleVariants expected;
    bool expect_failure;
};

struct ScalarToStringTestCase {
    std::string scalar;
    std::string expected;
    bool expect_failure;
};

class ScalarConversion: public ::testing::Test {
protected:
    std::vector<ScalarToBoolTestCase> bool_test_cases = {
        // success
        {"y", true, false},
        {"Y", true, false},
        {"yes", true, false},
        {"YES", true, false},
        {"true", true, false},
        {"TRUE", true, false},
        {"on", true, false},
        {"ON", true, false},
        {"n", false, false},
        {"N", false, false},
        {"no", false, false},
        {"NO", false, false},
        {"false", false, false},
        {"FALSE", false, false},
        {"off", false, false},
        {"OFF", false, false},
        // failure
        {"arglebargle", false, true},
        {"0", false, true},
        {"1", true, true},
        {"2", false, true},
    };

    std::vector<ScalarToIntTestCase> int_test_cases = {
        // success
        {"123", int64_t(123), false},
        {"-123", int64_t(-123), false},
        {"9223372036854775807", int64_t(9223372036854775807), false},
        {"-9223372036854775807", int64_t(-9223372036854775807), false},

        // invalid integers
        {"123.45", int64_t(0), true},
        {"arglebargle", int64_t(0), true},
        {"123abc", int64_t(0), true},
        {"-123abc", int64_t(0), true},

        // int8 overflow
        {"128", int8_t(0), true},
        {"-129", int8_t(0), true},

        // int16 overflow
        {"32768", int16_t(0), true},
        {"-32769", int16_t(0), true},

        // int32 overflow
        {"2147483648", int32_t(0), true},
        {"-2147483649", int32_t(0), true},

        // int64 overflow
        {"9223372036854775808", int64_t(0), true},
        {"-9223372036854775809", int64_t(0), true},

        // uint8 overflow
        {"256", uint8_t(0), true},
        {"-1", uint8_t(0), true},

        // uint16 overflow
        {"65536", uint16_t(0), true},
        {"-1", uint16_t(0), true},

        // uint32 overflow
        {"4294967296", uint32_t(0), true},
        {"-1", uint32_t(0), true},

        // uint64 overflow
        {"18446744073709551616", uint64_t(0), true},
        {"-1", uint64_t(0), true},
    };

    std::vector<ScalarToDoubleTestCase> double_test_cases = {
        // success
        {"123.45", double(123.45), false},
        {"-123.45", double(-123.45), false},
        {"124", double(124.0), false},
        {"-124", double(-124.0), false},
        {"1.7976931348623157E308", double(1.7976931348623157E308), false},
        {"-1.7976931348623157E308", double(-1.7976931348623157E308), false},

        // failure
        {"arglebargle", double(0.0), true},
        {"123.45.67", double(0.0), true},
        {"123abc", double(0.0), true},
        {"-123abc", double(0.0), true},

        // float overflow
        {"3.4028234663852886E39", float(0.0), true},
        {"-3.4028234663852886E39", float(0.0), true},

        // double overflow
        {"1.7976931348623158E309", double(0.0), true},
        {"-1.7976931348623158E309", double(0.0), true},
    };

    std::vector<ScalarToStringTestCase> string_test_cases = {
        // success
        {"123.45", "123.45", false},
        {"-123.45", "-123.45", false},
        {"124", "124", false},
        {"-124", "-124", false},
        {"arglebargle", "arglebargle", false},
        {"123.45.67", "123.45.67", false},
        {"123abc", "123abc", false},
        {"-123abc", "-123abc", false},
    };
};