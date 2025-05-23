// std
#include <string>

// internal
#include <miru/params/details/errors.hpp>
#include <miru/params/parameter.hpp>
#include <miru/params/scalar.hpp>
#include <test/details/type_conversion_test.hpp>

// external
#include <gtest/gtest.h>

namespace test::params {

using StringConversionException =
  test::details::type_conversion::StringConversionException;

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

// =============================== SCALAR CONVERSION =============================== //
class ScalarConversion : public test::details::type_conversion::StringConversion {};

TEST_F(ScalarConversion, bool_conversion_success) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    EXPECT_EQ(scalar.as_bool(), test_case.expected);
    EXPECT_EQ(scalar.as<bool>(), test_case.expected);
  }
}

TEST_F(ScalarConversion, bool_conversion_failure) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    switch (test_case.expected_exception) {
      case StringConversionException::InvalidTypeConversion:
        EXPECT_THROW(
          scalar.as_bool(), miru::params::details::InvalidScalarConversionError
        );
        EXPECT_THROW(
          scalar.as<bool>(), miru::params::details::InvalidScalarConversionError
        );
        break;
      default:
        FAIL() << "Unexpected exception type";
    }
  }
}

TEST_F(ScalarConversion, int_conversion_success) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    EXPECT_EQ(scalar.as_int(), std::get<int64_t>(test_case.expected));
    EXPECT_EQ(scalar.as<int64_t>(), std::get<int64_t>(test_case.expected));
  }
}

TEST_F(ScalarConversion, int_conversion_failure) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              scalar.as<T>(), miru::params::details::InvalidScalarConversionError
            );
            break;
          default:
            FAIL() << "Unexpected exception type";
        }
      },
      test_case.expected
    );
  }
}

TEST_F(ScalarConversion, double_conversion_success) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception !=
        test::details::type_conversion::StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    EXPECT_EQ(scalar.as_double(), std::get<double>(test_case.expected));
    EXPECT_EQ(scalar.as<double>(), std::get<double>(test_case.expected));
  }
}

TEST_F(ScalarConversion, double_conversion_failure) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              scalar.as<T>(), miru::params::details::InvalidScalarConversionError
            );
            break;
          default:
            FAIL() << "Unexpected exception type";
        }
      },
      test_case.expected
    );
  }
}

TEST_F(ScalarConversion, string_conversion_success) {
  for (const auto& test_case : string_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    EXPECT_EQ(scalar.as_string(), test_case.expected);
    EXPECT_EQ(scalar.as<std::string>(), test_case.expected);
  }
}

// ============================ SCALAR ARRAY CONVERSION ============================ //

TEST_F(ScalarConversion, bool_array_conversion_success) {
  std::vector<miru::params::Scalar> scalars = {};
  std::vector<bool> expected = {};
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    scalars.push_back(miru::params::Scalar(test_case.str));
    expected.push_back(test_case.expected);
  }
  std::vector<bool> result = miru::params::details::scalar_array_as<bool>(scalars);
  EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, bool_array_conversion_failure) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::vector<miru::params::Scalar> scalars = {};
    scalars.push_back(miru::params::Scalar(test_case.str));
    EXPECT_THROW(
      miru::params::details::scalar_array_as<bool>(scalars),
      miru::params::details::InvalidScalarConversionError
    );
  }
}

TEST_F(ScalarConversion, int_array_conversion_success) {
  std::vector<miru::params::Scalar> scalars = {};
  std::vector<int64_t> expected = {};
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    scalars.push_back(miru::params::Scalar(test_case.str));
    expected.push_back(std::get<int64_t>(test_case.expected));
  }
  std::vector<int64_t> result =
    miru::params::details::scalar_array_as<int64_t>(scalars);
  EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, int_array_conversion_failure) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        std::vector<miru::params::Scalar> scalars = {};
        scalars.push_back(scalar);
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::params::details::scalar_array_as<T>(scalars),
              miru::params::details::InvalidScalarConversionError
            );
            break;
          default:
            FAIL() << "Unexpected exception type";
        }
      },
      test_case.expected
    );
  }
}

TEST_F(ScalarConversion, double_array_conversion_success) {
  std::vector<miru::params::Scalar> scalars = {};
  std::vector<double> expected = {};
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    scalars.push_back(miru::params::Scalar(test_case.str));
    expected.push_back(std::get<double>(test_case.expected));
  }
  std::vector<double> result = miru::params::details::scalar_array_as<double>(scalars);
  EXPECT_EQ(result, expected);
}

TEST_F(ScalarConversion, double_array_conversion_failure) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    miru::params::Scalar scalar(test_case.str);
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        std::vector<miru::params::Scalar> scalars = {};
        scalars.push_back(scalar);
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::params::details::scalar_array_as<T>(scalars),
              miru::params::details::InvalidScalarConversionError
            );
            break;
          default:
            FAIL() << "Unexpected exception type";
        }
      },
      test_case.expected
    );
  }
}

TEST_F(ScalarConversion, string_array_conversion_success) {
  std::vector<miru::params::Scalar> scalars = {};
  std::vector<std::string> expected = {};
  for (const auto& test_case : string_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    scalars.push_back(miru::params::Scalar(test_case.str));
    expected.push_back(test_case.expected);
  }
  std::vector<std::string> result =
    miru::params::details::scalar_array_as<std::string>(scalars);
  EXPECT_EQ(result, expected);
}

}  // namespace test::params