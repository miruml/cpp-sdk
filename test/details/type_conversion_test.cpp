// std
#include <execinfo.h>

#include <variant>

// internal
#include <miru/details/type_conversion.hpp>
#include <test/details/type_conversion_test.hpp>
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

namespace test::details::type_conversion {

// =============================== NUMBER CASTING ================================== //
class UtilsIntegerCasting : public IntegerCasting {};

TEST_F(UtilsIntegerCasting, integer_type_casting_success) {
  for (const auto& test_case : all_test_cases()) {
    if (test_case.expect_failure) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_EQ(
          miru::details::type_conversion::int64_as<T>(test_case.input),
          std::get<T>(test_case.expected)
        );
      },
      test_case.expected
    );
  }
}

TEST_F(UtilsIntegerCasting, integer_type_casting_failure) {
  for (const auto& test_case : all_test_cases()) {
    if (!test_case.expect_failure) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_THROW(
          miru::details::type_conversion::int64_as<T>(test_case.input),
          miru::details::type_conversion::InvalidTypeConversionError
        );
      },
      test_case.expected
    );
  }
}

// casting integer arrays to different types is not currently supported (only
// std::vector<int64_t> is supported)

class UtilsDoubleCasting : public DoubleCasting {};

TEST_F(UtilsDoubleCasting, double_type_casting_success) {
  for (const auto& test_case : double_test_cases) {
    if (test_case.expect_failure) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_EQ(
          miru::details::type_conversion::double_as<T>(test_case.input),
          std::get<T>(test_case.expected)
        );
      },
      test_case.expected
    );
  }
}

TEST_F(UtilsDoubleCasting, double_type_casting_failure) {
  for (const auto& test_case : double_test_cases) {
    if (!test_case.expect_failure) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        EXPECT_THROW(
          miru::details::type_conversion::double_as<T>(test_case.input),
          miru::details::type_conversion::InvalidTypeConversionError
        );
      },
      test_case.expected
    );
  }
}

// =============================== STRING CONVERSIONS ============================== //
class UtilsStringConversion : public StringConversion {};

TEST_F(UtilsStringConversion, bool_conversion_success) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    EXPECT_EQ(
      miru::details::type_conversion::yaml_string_to_bool(test_case.str),
      test_case.expected
    );
    EXPECT_EQ(
      miru::details::type_conversion::string_as<bool>(test_case.str), test_case.expected
    );
  }
}

TEST_F(UtilsStringConversion, bool_conversion_failure) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    switch (test_case.expected_exception) {
      case test::details::type_conversion::StringConversionException::
        InvalidTypeConversion:
        EXPECT_THROW(
          miru::details::type_conversion::yaml_string_to_bool(test_case.str),
          miru::details::type_conversion::InvalidTypeConversionError
        );
        EXPECT_THROW(
          miru::details::type_conversion::string_as<bool>(test_case.str),
          miru::details::type_conversion::InvalidTypeConversionError
        );
        break;
      default:
        FAIL() << "Unexpected exception type";
    }
  }
}

TEST_F(UtilsStringConversion, int_conversion_success) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    EXPECT_EQ(
      miru::details::type_conversion::string_to_int64(test_case.str),
      std::get<int64_t>(test_case.expected)
    );
    EXPECT_EQ(
      miru::details::type_conversion::string_as<int64_t>(test_case.str),
      std::get<int64_t>(test_case.expected)
    );
  }
}

TEST_F(UtilsStringConversion, int_conversion_failure) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::details::type_conversion::string_as<T>(test_case.str),
              miru::details::type_conversion::InvalidTypeConversionError
            ) << "Exception not thrown for "
              << test_case.str;
            break;
          default:
            FAIL() << "Unexpected exception type";
        }
      },
      test_case.expected
    );
  }
}

TEST_F(UtilsStringConversion, double_conversion_success) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    EXPECT_EQ(
      miru::details::type_conversion::string_to_double(test_case.str),
      std::get<double>(test_case.expected)
    );
    EXPECT_EQ(
      miru::details::type_conversion::string_as<double>(test_case.str),
      std::get<double>(test_case.expected)
    );
  }
}

TEST_F(UtilsStringConversion, double_conversion_failure) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::details::type_conversion::string_as<T>(test_case.str),
              miru::details::type_conversion::InvalidTypeConversionError
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

TEST_F(UtilsStringConversion, string_conversion_success) {
  for (const auto& test_case : string_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    EXPECT_EQ(
      miru::details::type_conversion::string_as<std::string>(test_case.str),
      test_case.expected
    );
  }
}

// ============================ STRING ARRAY CONVERSIONS =========================== //

TEST_F(UtilsStringConversion, bool_array_conversion_success) {
  std::vector<std::string> strs = {};
  std::vector<bool> expected = {};
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    strs.push_back(test_case.str);
    expected.push_back(test_case.expected);
  }
  std::vector<bool> result =
    miru::details::type_conversion::string_array_as<bool>(strs);
  EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, bool_array_conversion_failure) {
  for (const auto& test_case : bool_test_cases) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::vector<std::string> strs = {};
    strs.push_back(test_case.str);
    EXPECT_THROW(
      miru::details::type_conversion::string_array_as<bool>(strs),
      miru::details::type_conversion::InvalidTypeConversionError
    );
  }
}

TEST_F(UtilsStringConversion, int_array_conversion_success) {
  std::vector<std::string> strs = {};
  std::vector<int64_t> expected = {};
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    strs.push_back(test_case.str);
    expected.push_back(std::get<int64_t>(test_case.expected));
  }
  std::vector<int64_t> result =
    miru::details::type_conversion::string_array_as<int64_t>(strs);
  EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, int_array_conversion_failure) {
  for (const auto& test_case : int_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        std::vector<std::string> strs = {};
        strs.push_back(test_case.str);
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::details::type_conversion::string_array_as<T>(strs),
              miru::details::type_conversion::InvalidTypeConversionError
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

TEST_F(UtilsStringConversion, double_array_conversion_success) {
  std::vector<std::string> strs = {};
  std::vector<double> expected = {};
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    strs.push_back(test_case.str);
    expected.push_back(std::get<double>(test_case.expected));
  }
  std::vector<double> result =
    miru::details::type_conversion::string_array_as<double>(strs);
  EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, double_array_conversion_failure) {
  for (const auto& test_case : float_test_cases()) {
    if (test_case.expected_exception == StringConversionException::None) {
      continue;
    }
    std::visit(
      [&](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        std::vector<std::string> strs = {};
        strs.push_back(test_case.str);
        switch (test_case.expected_exception) {
          case StringConversionException::InvalidTypeConversion:
            EXPECT_THROW(
              miru::details::type_conversion::string_array_as<T>(strs),
              miru::details::type_conversion::InvalidTypeConversionError
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

TEST_F(UtilsStringConversion, string_array_conversion_success) {
  std::vector<std::string> strs = {};
  std::vector<std::string> expected = {};
  for (const auto& test_case : string_test_cases) {
    if (test_case.expected_exception != StringConversionException::None) {
      continue;
    }
    strs.push_back(test_case.str);
    expected.push_back(test_case.expected);
  }
  std::vector<std::string> result =
    miru::details::type_conversion::string_array_as<std::string>(strs);
  EXPECT_EQ(result, expected);
}

}  // namespace test::details::type_conversion