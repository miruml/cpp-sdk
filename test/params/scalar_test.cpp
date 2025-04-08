// std
#include <string>

// internal
#include "miru/params/exceptions.hpp"
#include "miru/params/scalar.hpp"

// external
#include <gtest/gtest.h>


// ================================== EQUALITY ===================================== //
class ScalarEquality : public ::testing::Test {
protected:
};

TEST_F(ScalarEquality, equality) {
    miru::params::Scalar scalar1("true");
    miru::params::Scalar scalar2("true");
    EXPECT_EQ(scalar1, scalar2);
}

TEST_F(ScalarEquality, inequality) {
    miru::params::Scalar scalar1("true");
    miru::params::Scalar scalar2("false");
    EXPECT_NE(scalar1, scalar2);
}

// ================================= CONVERSION ==================================== //
struct ConvertBoolTestCase {
    std::string scalar;
    bool expected;
    bool expect_failure;
};

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

struct ConvertIntTestCase {
    std::string scalar;
    IntVariants expected;
    bool expect_failure;
};

using DoubleVariants = std::variant<
    float,
    double
>;

struct ConvertDoubleTestCase {
    std::string scalar;
    DoubleVariants expected;
    bool expect_failure;
};

struct ConvertStringTestCase {
    std::string scalar;
    std::string expected;
    bool expect_failure;
};

class ScalarConversion: public ::testing::Test {
protected:
    std::vector<ConvertBoolTestCase> bool_test_cases = {
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

    std::vector<ConvertIntTestCase> int_test_cases = {
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

    std::vector<ConvertDoubleTestCase> double_test_cases = {
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

    std::vector<ConvertStringTestCase> string_test_cases = {
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

TEST_F(ScalarConversion, bool_conversion_success) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_bool(), test_case.expected);
    }
}

TEST_F(ScalarConversion, bool_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_THROW(scalar.as_bool(), miru::params::InvalidScalarConversion);
    }
}

TEST_F(ScalarConversion, bool_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<bool> expected = {};
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(test_case.expected);
    }
    std::vector<bool> result = miru::params::transform_scalar_array<bool>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, bool_array_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        std::vector<miru::params::Scalar> scalars = {};
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        EXPECT_THROW(
            miru::params::transform_scalar_array<bool>(scalars), 
            miru::params::InvalidScalarConversion
        );
    }
}

TEST_F(ScalarConversion, int_conversion_success) {
    for (const auto & test_case : int_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        int64_t expected = std::get<int64_t>(test_case.expected);
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_int(), expected);
    }
}

TEST_F(ScalarConversion, int_conversion_failure) {
    for (const auto & test_case : int_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&scalar](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_THROW(
                scalar.as<T>(),
                miru::params::InvalidScalarConversion
            );
        }, test_case.expected);
    }
}

TEST_F(ScalarConversion, int_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<int64_t> expected = {};
    for (const auto & test_case : int_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(std::get<int64_t>(test_case.expected));
    }
    std::vector<int64_t> result = miru::params::transform_scalar_array<int64_t>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, int_array_conversion_failure) {
    for (const auto & test_case : int_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&scalar](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<miru::params::Scalar> scalars = {};
            scalars.push_back(scalar);
            EXPECT_THROW(
                miru::params::transform_scalar_array<T>(scalars), 
                miru::params::InvalidScalarConversion
            );
        }, test_case.expected);

    }
}

TEST_F(ScalarConversion, double_conversion_success) {
    for (const auto & test_case : double_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        double expected = std::get<double>(test_case.expected);
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_double(), expected);
    }
}

TEST_F(ScalarConversion, double_conversion_failure) {
    for (const auto & test_case : double_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&scalar](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_THROW(
                scalar.as<T>(),
                miru::params::InvalidScalarConversion
            );
        }, test_case.expected);
    }
}

TEST_F(ScalarConversion, double_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<double> expected = {};
    for (const auto & test_case : double_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(std::get<double>(test_case.expected));
    }
    std::vector<double> result = miru::params::transform_scalar_array<double>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, double_array_conversion_failure) {
    for (const auto & test_case : double_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&scalar](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<miru::params::Scalar> scalars = {};
            scalars.push_back(scalar);
            EXPECT_THROW(miru::params::transform_scalar_array<T>(scalars), miru::params::InvalidScalarConversion);
        }, test_case.expected);
    }
}

TEST_F(ScalarConversion, string_conversion_success) {
    for (const auto & test_case : string_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_string(), test_case.expected);
    }
}

TEST_F(ScalarConversion, string_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<std::string> expected = {};
    for (const auto & test_case : string_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(test_case.expected);
    }
    std::vector<std::string> result = miru::params::transform_scalar_array<std::string>(scalars);
    EXPECT_EQ(result, expected);
}