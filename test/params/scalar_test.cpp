// std
#include <string>

// internal
#include <miru/params/exceptions.hpp>
#include <miru/params/scalar.hpp>
#include <test/params/scalar_test.hpp>

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
        std::visit([&](auto&& arg) {
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