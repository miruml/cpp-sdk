#pragma once

// std
#include <cstdint>
#include <variant>

// external
#include <gtest/gtest.h>

namespace test::utils {

// ================================== INTEGERS ===================================== //
using IntVariants = std::variant<
  int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t>;

struct IntCastTestCase {
  int64_t input;
  IntVariants expected;
  bool expect_failure;
};

class IntegerCasting : public ::testing::Test {
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
    all_test_cases.insert(
      all_test_cases.end(), int8_test_cases.begin(), int8_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int16_test_cases.begin(), int16_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int32_test_cases.begin(), int32_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int64_test_cases.begin(), int64_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint8_test_cases.begin(), uint8_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint16_test_cases.begin(), uint16_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint32_test_cases.begin(), uint32_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint64_test_cases.begin(), uint64_test_cases.end()
    );
    return all_test_cases;
  }
};

// ================================ DOUBLE CASTING ================================= //
using DoubleVariants = std::variant<float, double>;

struct DoubleCastTestCase {
  double input;
  DoubleVariants expected;
  bool expect_failure;
};

class DoubleCasting : public ::testing::Test {
 protected:
  std::vector<DoubleCastTestCase> double_test_cases = {
    // double success
    {123.45, double(123.45), false},
    {-123.45, double(-123.45), false},
    {124, double(124.0), false},
    {-124, double(-124.0), false},
    {1.7976931348623157E308, double(1.7976931348623157E308), false},
    {-1.7976931348623157E308, double(-1.7976931348623157E308), false},

    // float success
    {123.45, float(123.45), false},
    {-123.45, float(-123.45), false},
    {124, float(124.0), false},
    {-124, float(-124.0), false},

    // float overflow
    {3.4028234663852886E39, float(0.0), true},
    {-3.4028234663852886E39, float(0.0), true},
  };
};

// =============================== STRING CONVERSIONS ============================== //
enum class StringConversionException {
  None,
  InvalidTypeConversion,
};

struct StringToBoolTestCase {
  std::string str;
  bool expected;
  StringConversionException expected_exception;
};

struct StringToIntTestCase {
  std::string str;
  IntVariants expected;
  StringConversionException expected_exception;
};

struct StringToDoubleTestCase {
  std::string str;
  DoubleVariants expected;
  StringConversionException expected_exception;
};

struct StringToStringTestCase {
  std::string str;
  std::string expected;
  StringConversionException expected_exception;
};

class StringConversion : public ::testing::Test {
 protected:
  std::vector<StringToBoolTestCase> bool_test_cases = {
    // success
    {"y", true, StringConversionException::None},
    {"Y", true, StringConversionException::None},
    {"yes", true, StringConversionException::None},
    {"YES", true, StringConversionException::None},
    {"true", true, StringConversionException::None},
    {"TRUE", true, StringConversionException::None},
    {"on", true, StringConversionException::None},
    {"ON", true, StringConversionException::None},
    {"n", false, StringConversionException::None},
    {"N", false, StringConversionException::None},
    {"no", false, StringConversionException::None},
    {"NO", false, StringConversionException::None},
    {"false", false, StringConversionException::None},
    {"FALSE", false, StringConversionException::None},
    {"off", false, StringConversionException::None},
    {"OFF", false, StringConversionException::None},
    // failure
    {"arglebargle", false, StringConversionException::InvalidTypeConversion},
    {"0", false, StringConversionException::InvalidTypeConversion},
    {"1", true, StringConversionException::InvalidTypeConversion},
    {"2", false, StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> int64_test_cases = {
    // success
    {"123", int64_t(123), StringConversionException::None},
    {"-123", int64_t(-123), StringConversionException::None},
    {"9223372036854775807", int64_t(9223372036854775807),
     StringConversionException::None},
    {"-9223372036854775807", int64_t(-9223372036854775807),
     StringConversionException::None},

    // invalid integers
    {"123.45", int64_t(0), StringConversionException::InvalidTypeConversion},
    {"arglebargle", int64_t(0), StringConversionException::InvalidTypeConversion},
    {"123abc", int64_t(0), StringConversionException::InvalidTypeConversion},
    {"-123abc", int64_t(0), StringConversionException::InvalidTypeConversion},

    // int64 overflow
    {"9223372036854775808", int64_t(0), StringConversionException::InvalidTypeConversion
    },
    {"-9223372036854775809", int64_t(0),
     StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> int8_test_cases = {
    // int8 overflow
    {"128", int8_t(0), StringConversionException::InvalidTypeConversion},
    {"-129", int8_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> int16_test_cases = {
    // int16 overflow
    {"32768", int16_t(0), StringConversionException::InvalidTypeConversion},
    {"-32769", int16_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> int32_test_cases = {
    // int32 overflow
    {"2147483648", int32_t(0), StringConversionException::InvalidTypeConversion},
    {"-2147483649", int32_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> uint8_test_cases = {
    // uint8 overflow
    {"256", uint8_t(0), StringConversionException::InvalidTypeConversion},
    {"-1", uint8_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> uint16_test_cases = {
    // uint16 overflow
    {"65536", uint16_t(0), StringConversionException::InvalidTypeConversion},
    {"-1", uint16_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> uint32_test_cases = {
    // uint32 overflow
    {"4294967296", uint32_t(0), StringConversionException::InvalidTypeConversion},
    {"-1", uint32_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> uint64_test_cases = {
    // uint64 overflow
    {"18446744073709551616", uint64_t(0),
     StringConversionException::InvalidTypeConversion},
    {"-1", uint64_t(0), StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToIntTestCase> int_test_cases() {
    std::vector<StringToIntTestCase> all_test_cases = {};
    all_test_cases.insert(
      all_test_cases.end(), int64_test_cases.begin(), int64_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int8_test_cases.begin(), int8_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int16_test_cases.begin(), int16_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), int32_test_cases.begin(), int32_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint8_test_cases.begin(), uint8_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint16_test_cases.begin(), uint16_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint32_test_cases.begin(), uint32_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), uint64_test_cases.begin(), uint64_test_cases.end()
    );
    return all_test_cases;
  }

  std::vector<StringToDoubleTestCase> float64_test_cases = {
    // success
    {"123.45", double(123.45), StringConversionException::None},
    {"-123.45", double(-123.45), StringConversionException::None},
    {"124", double(124.0), StringConversionException::None},
    {"-124", double(-124.0), StringConversionException::None},
    {"1.7976931348623157E308", double(1.7976931348623157E308),
     StringConversionException::None},
    {"-1.7976931348623157E308", double(-1.7976931348623157E308),
     StringConversionException::None},

    // failure
    {"arglebargle", double(0.0), StringConversionException::InvalidTypeConversion},
    {"123.45.67", double(0.0), StringConversionException::InvalidTypeConversion},
    {"123abc", double(0.0), StringConversionException::InvalidTypeConversion},
    {"-123abc", double(0.0), StringConversionException::InvalidTypeConversion},

    // double overflow
    {"1.7976931348623158E309", double(0.0),
     StringConversionException::InvalidTypeConversion},
    {"-1.7976931348623158E309", double(0.0),
     StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToDoubleTestCase> float32_test_cases = {
    // float overflow
    {"3.4028234663852886E39", float(0.0),
     StringConversionException::InvalidTypeConversion},
    {"-3.4028234663852886E39", float(0.0),
     StringConversionException::InvalidTypeConversion},
  };

  std::vector<StringToDoubleTestCase> float_test_cases() {
    std::vector<StringToDoubleTestCase> all_test_cases = {};
    all_test_cases.insert(
      all_test_cases.end(), float64_test_cases.begin(), float64_test_cases.end()
    );
    all_test_cases.insert(
      all_test_cases.end(), float32_test_cases.begin(), float32_test_cases.end()
    );
    return all_test_cases;
  }

  std::vector<StringToStringTestCase> string_test_cases = {
    // success
    {"123.45", "123.45", StringConversionException::None},
    {"-123.45", "-123.45", StringConversionException::None},
    {"124", "124", StringConversionException::None},
    {"-124", "-124", StringConversionException::None},
    {"arglebargle", "arglebargle", StringConversionException::None},
    {"123.45.67", "123.45.67", StringConversionException::None},
    {"123abc", "123abc", StringConversionException::None},
    {"-123abc", "-123abc", StringConversionException::None},
  };
};

}  // namespace test::utils
