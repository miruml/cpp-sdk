// std
#include <execinfo.h>

// internal
#include <miru/utils.hpp>
#include <test/utils_test.hpp>
#include <test/test_utils/utils.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <boost/stacktrace.hpp>
#include <gtest/gtest.h>

namespace test::utils {

// ================================ HAS PREFIX ===================================== //
TEST(HasPrefix, True) {
    EXPECT_TRUE(miru::utils::has_prefix("hello", "hel"));
    EXPECT_TRUE(miru::utils::has_prefix("hello", "hello"));
    EXPECT_TRUE(miru::utils::has_prefix("hello world", "hello"));
    EXPECT_TRUE(miru::utils::has_prefix("a", "a"));
    EXPECT_TRUE(miru::utils::has_prefix("  spaces", "  "));
}

TEST(HasPrefix, False) {
    EXPECT_FALSE(miru::utils::has_prefix("hello", "world"));
    EXPECT_FALSE(miru::utils::has_prefix("hello", "hello!"));
    EXPECT_FALSE(miru::utils::has_prefix("", "a"));
    EXPECT_FALSE(miru::utils::has_prefix("hello", "HELLO"));  // case sensitive
    EXPECT_FALSE(miru::utils::has_prefix("a", "ab"));
}

TEST(HasPrefix, EmptyStrings) {
    EXPECT_TRUE(miru::utils::has_prefix("hello", ""));  // empty prefix
    EXPECT_TRUE(miru::utils::has_prefix("", ""));      // both empty
}

// ================================ HAS SUFFIX ===================================== //
TEST(HasSuffix, True) {
    EXPECT_TRUE(miru::utils::has_suffix("hello", "llo"));
    EXPECT_TRUE(miru::utils::has_suffix("hello", "hello"));
    EXPECT_TRUE(miru::utils::has_suffix("world hello", "hello"));
    EXPECT_TRUE(miru::utils::has_suffix("a", "a"));
    EXPECT_TRUE(miru::utils::has_suffix("spaces  ", "  "));
}

TEST(HasSuffix, False) {
    EXPECT_FALSE(miru::utils::has_suffix("hello", "world"));
    EXPECT_FALSE(miru::utils::has_suffix("hello", "!hello"));
    EXPECT_FALSE(miru::utils::has_suffix("", "a"));
    EXPECT_FALSE(miru::utils::has_suffix("hello", "HELLO"));  // case sensitive
    EXPECT_FALSE(miru::utils::has_suffix("a", "ba"));
}

TEST(HasSuffix, EmptyStrings) {
    EXPECT_TRUE(miru::utils::has_suffix("hello", ""));  // empty suffix
    EXPECT_TRUE(miru::utils::has_suffix("", ""));      // both empty
}

// ============================ ASSERT UNIQUE STRINGS ============================== //
TEST(AssertUniqueStrings, EmptyVector) {
    std::vector<std::string> empty;
    EXPECT_NO_THROW(miru::utils::assert_unique_strings(empty));
}

TEST(AssertUniqueStrings, SingleString) {
    std::vector<std::string> single{"test"};
    EXPECT_NO_THROW(miru::utils::assert_unique_strings(single));
}

TEST(AssertUniqueStrings, UniqueStrings) {
    std::vector<std::string> unique{
        "test1",
        "test2",
        "hello",
        "world",
        ""  // empty string
    };
    EXPECT_NO_THROW(miru::utils::assert_unique_strings(unique));
}

TEST(AssertUniqueStrings, DuplicateStrings) {
    std::vector<std::string> duplicates{
        "test1",
        "test2",
        "test1"  // duplicate
    };
    EXPECT_THROW(
        miru::utils::assert_unique_strings(duplicates),
        std::invalid_argument
    );
}

TEST(AssertUniqueStrings, CaseSensitive) {
    std::vector<std::string> cases{
        "Test",
        "test",
        "TEST"
    };
    EXPECT_NO_THROW(miru::utils::assert_unique_strings(cases));
}

TEST(AssertUniqueStrings, WhitespaceStrings) {
    std::vector<std::string> whitespace{
        "test",
        " test",  // different due to leading space
        "test ",  // different due to trailing space
        "  "
    };
    EXPECT_NO_THROW(miru::utils::assert_unique_strings(whitespace));
}

TEST(AssertUniqueStrings, MultipleEmptyStrings) {
    std::vector<std::string> empty_strings{
        "",
        ""  // duplicate empty string
    };
    EXPECT_THROW(
        miru::utils::assert_unique_strings(empty_strings),
        std::invalid_argument
    );
}

TEST(AssertUniqueStrings, MultipleDuplicates) {
    std::vector<std::string> multiple_dupes{
        "test1",
        "test2",
        "test1",  // duplicate
        "test3",
        "test2",  // duplicate
        "test4"
    };
    EXPECT_THROW(
        miru::utils::assert_unique_strings(multiple_dupes),
        std::invalid_argument
    );
}

// =============================== NUMBER CASTING ================================== //
class UtilsIntegerCasting : public IntegerCasting {};

TEST_F(UtilsIntegerCasting, integer_type_casting_success) {
    for (const auto & test_case : all_test_cases()) {
        if (test_case.expect_failure) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_EQ(
                miru::utils::int64_as<T>(test_case.input),
                std::get<T>(test_case.expected)
            );
        }, test_case.expected);
    }
}

TEST_F(UtilsIntegerCasting, integer_type_casting_failure) {
    for (const auto & test_case : all_test_cases()) {
        if (!test_case.expect_failure) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_THROW(
                miru::utils::int64_as<T>(test_case.input),
                miru::utils::InvalidTypeConversion
            );
        }, test_case.expected);
    }
}

// casting integer arrays to different types is not currently supported (only std::vector<int64_t> is supported)

class UtilsDoubleCasting : public DoubleCasting {};

TEST_F(UtilsDoubleCasting, double_type_casting_success) {
    for (const auto & test_case : double_test_cases) {
        if (test_case.expect_failure) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_EQ(
                miru::utils::double_as<T>(test_case.input),
                std::get<T>(test_case.expected)
            );
        }, test_case.expected);
    }
}

TEST_F(UtilsDoubleCasting, double_type_casting_failure) {
    for (const auto & test_case : double_test_cases) {
        if (!test_case.expect_failure) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            EXPECT_THROW(
                miru::utils::double_as<T>(test_case.input),
                miru::utils::InvalidTypeConversion
            );
        }, test_case.expected);
    }
}

// =============================== STRING CONVERSIONS ============================== //
class UtilsStringConversion : public StringConversion {};

TEST_F(UtilsStringConversion, bool_conversion_success) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        EXPECT_EQ(
            miru::utils::yaml_string_to_bool(test_case.str),
            test_case.expected
        );
        EXPECT_EQ(
            miru::utils::string_as<bool>(test_case.str),
            test_case.expected
        );
    }
}

TEST_F(UtilsStringConversion, bool_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        switch (test_case.expected_exception) {
            case test::utils::StringConversionException::InvalidTypeConversion:
                EXPECT_THROW(
                    miru::utils::yaml_string_to_bool(test_case.str),
                    miru::utils::InvalidTypeConversion
                );
                EXPECT_THROW(
                    miru::utils::string_as<bool>(test_case.str),
                    miru::utils::InvalidTypeConversion
                );
                break;
            default:
                FAIL() << "Unexpected exception type";
        }
    }
}

TEST_F(UtilsStringConversion, int_conversion_success) {
    for (const auto & test_case : int_test_cases() ) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        EXPECT_EQ(
            miru::utils::string_to_int64(test_case.str),
            std::get<int64_t>(test_case.expected)
        );
        EXPECT_EQ(
            miru::utils::string_as<int64_t>(test_case.str),
            std::get<int64_t>(test_case.expected)
        );
    }
}

TEST_F(UtilsStringConversion, int_conversion_failure) {
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeConversion:
                    EXPECT_THROW(
                        miru::utils::string_as<T>(test_case.str),
                        miru::utils::InvalidTypeConversion
                    ) << "Exception not thrown for " << test_case.str;
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(UtilsStringConversion, double_conversion_success) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        EXPECT_EQ(
            miru::utils::string_to_double(test_case.str),
            std::get<double>(test_case.expected)
        );
        EXPECT_EQ(
            miru::utils::string_as<double>(test_case.str),
            std::get<double>(test_case.expected)
        );
    }
}

TEST_F(UtilsStringConversion, double_conversion_failure) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeConversion:
                    EXPECT_THROW(
                        miru::utils::string_as<T>(test_case.str),
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(UtilsStringConversion, string_conversion_success) {
    for (const auto & test_case : string_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        EXPECT_EQ(
            miru::utils::string_as<std::string>(test_case.str),
            test_case.expected
        );
    }
}

// ============================ STRING ARRAY CONVERSIONS =========================== //

TEST_F(UtilsStringConversion, bool_array_conversion_success) {
    std::vector<std::string> strs = {};
    std::vector<bool> expected = {};
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        strs.push_back(test_case.str);
        expected.push_back(test_case.expected);
    }
    std::vector<bool> result = miru::utils::string_array_as<bool>(strs);
    EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, bool_array_conversion_failure) {
    for (const auto & test_case : bool_test_cases) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::vector<std::string> strs = {};
        strs.push_back(test_case.str);
        EXPECT_THROW(
            miru::utils::string_array_as<bool>(strs), 
            miru::utils::InvalidTypeConversion
        );
    }
}

TEST_F(UtilsStringConversion, int_array_conversion_success) {
    std::vector<std::string> strs = {};
    std::vector<int64_t> expected = {};
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        strs.push_back(test_case.str);
        expected.push_back(std::get<int64_t>(test_case.expected));
    }
    std::vector<int64_t> result = miru::utils::string_array_as<int64_t>(strs);
    EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, int_array_conversion_failure) {
    for (const auto & test_case : int_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<std::string> strs = {};
            strs.push_back(test_case.str);
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeConversion:
                    EXPECT_THROW(
                        miru::utils::string_array_as<T>(strs), 
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(UtilsStringConversion, double_array_conversion_success) {
    std::vector<std::string> strs = {};
    std::vector<double> expected = {};
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        strs.push_back(test_case.str);
        expected.push_back(std::get<double>(test_case.expected));
    }
    std::vector<double> result = miru::utils::string_array_as<double>(strs);
    EXPECT_EQ(result, expected);
}

TEST_F(UtilsStringConversion, double_array_conversion_failure) {
    for (const auto & test_case : float_test_cases()) {
        if (test_case.expected_exception == test::utils::StringConversionException::None) {
            continue;
        }
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            std::vector<std::string> strs = {};
            strs.push_back(test_case.str);
            switch (test_case.expected_exception) {
                case test::utils::StringConversionException::InvalidTypeConversion:
                    EXPECT_THROW(
                        miru::utils::string_array_as<T>(strs), 
                        miru::utils::InvalidTypeConversion
                    );
                    break;
                default:
                    FAIL() << "Unexpected exception type";
            }
        }, test_case.expected);
    }
}

TEST_F(UtilsStringConversion, string_array_conversion_success) {
    std::vector<std::string> strs = {};
    std::vector<std::string> expected = {};
    for (const auto & test_case : string_test_cases) {
        if (test_case.expected_exception != test::utils::StringConversionException::None) {
            continue;
        }
        strs.push_back(test_case.str);
        expected.push_back(test_case.expected);
    }
    std::vector<std::string> result = miru::utils::string_array_as<std::string>(strs);
    EXPECT_EQ(result, expected);
}

} // namespace test::utils
