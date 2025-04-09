// std
#include <string>

// internal
#include <miru/params/exceptions.hpp>
#include <miru/params/scalar.hpp>
#include <miru/utils.hpp>
#include <test/utils_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {


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
class StringConversion : public test::utils::StringConversion {};

TEST_F(StringConversion, bool_conversion_success) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_bool(), test_case.expected);
    }
}

TEST_F(StringConversion, bool_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        switch (test_case.expected_exception) {
            case test::utils::StringConversionException::InvalidStringConversion:
                EXPECT_THROW(scalar.as_bool(), miru::params::InvalidScalarConversion);
                break;
            default:
                FAIL() << "Unexpected exception type";
        }
    }
}

TEST_F(StringConversion, bool_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<bool> expected = {};
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(test_case.expected);
    }
    std::vector<bool> result = miru::params::scalar_array_as<bool>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(StringConversion, bool_array_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::vector<miru::params::Scalar> scalars = {};
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        EXPECT_THROW(
            miru::params::scalar_array_as<bool>(scalars), 
            miru::params::InvalidScalarConversion
        );
    }
}

TEST_F(StringConversion, int_conversion_success) {
    for (const auto & test_case : int_test_cases() ) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        int64_t expected = std::get<int64_t>(test_case.expected);
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_int(), expected);
    }
}

TEST_F(StringConversion, int_conversion_failure) {
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidStringConversion:
                    EXPECT_THROW(
                        scalar.as<T>(),
                        miru::params::InvalidScalarConversion
                    );
                    break;
                case test::utils::StringConversionException::InvalidTypeCast:
                    EXPECT_THROW(
                        scalar.as<T>(),
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(StringConversion, int_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<int64_t> expected = {};
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(std::get<int64_t>(test_case.expected));
    }
    std::vector<int64_t> result = miru::params::scalar_array_as<int64_t>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(StringConversion, int_array_conversion_failure) {
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<miru::params::Scalar> scalars = {};
            scalars.push_back(scalar);
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeCast:
                    EXPECT_THROW(
                        miru::params::scalar_array_as<T>(scalars), 
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                case test::utils::StringConversionException::InvalidStringConversion:
                    EXPECT_THROW(
                        miru::params::scalar_array_as<T>(scalars), 
                        miru::params::InvalidScalarConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(StringConversion, double_conversion_success) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        double expected = std::get<double>(test_case.expected);
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_double(), expected);
    }
}

TEST_F(StringConversion, double_conversion_failure) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidStringConversion:
                    EXPECT_THROW(
                        scalar.as<T>(),
                        miru::params::InvalidScalarConversion
                    );
                    break;
                case test::utils::StringConversionException::InvalidTypeCast:
                    EXPECT_THROW(
                        scalar.as<T>(),
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(StringConversion, double_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<double> expected = {};
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(std::get<double>(test_case.expected));
    }
    std::vector<double> result = miru::params::scalar_array_as<double>(scalars);
    EXPECT_EQ(result, expected);
}

TEST_F(StringConversion, double_array_conversion_failure) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<miru::params::Scalar> scalars = {};
            scalars.push_back(scalar);
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeCast:
                    EXPECT_THROW(
                        miru::params::scalar_array_as<T>(scalars), 
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                case test::utils::StringConversionException::InvalidStringConversion:
                    EXPECT_THROW(
                        miru::params::scalar_array_as<T>(scalars), 
                        miru::params::InvalidScalarConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(StringConversion, string_conversion_success) {
    for (const auto & test_case : string_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        miru::params::Scalar scalar(test_case.scalar);
        EXPECT_EQ(scalar.as_string(), test_case.expected);
    }
}

TEST_F(StringConversion, string_array_conversion_success) {
    std::vector<miru::params::Scalar> scalars = {};
    std::vector<std::string> expected = {};
    for (const auto & test_case : string_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        scalars.push_back(miru::params::Scalar(test_case.scalar));
        expected.push_back(test_case.expected);
    }
    std::vector<std::string> result = miru::params::scalar_array_as<std::string>(scalars);
    EXPECT_EQ(result, expected);
}

} // namespace test::params