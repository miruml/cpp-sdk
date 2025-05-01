// std
#include <execinfo.h>

// internal
#include <test/test_utils/testdata.hpp>
#include <test/test_utils/utils.hpp>
#include <utils.hpp>

// external
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
  EXPECT_TRUE(miru::utils::has_prefix("", ""));       // both empty
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
  EXPECT_TRUE(miru::utils::has_suffix("", ""));       // both empty
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
  EXPECT_THROW(miru::utils::assert_unique_strings(duplicates), std::invalid_argument);
}

TEST(AssertUniqueStrings, CaseSensitive) {
  std::vector<std::string> cases{"Test", "test", "TEST"};
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
    miru::utils::assert_unique_strings(empty_strings), std::invalid_argument
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
    miru::utils::assert_unique_strings(multiple_dupes), std::invalid_argument
  );
}

// ================================ BASE64 ENCODING ==================================
// //
TEST(Base64Encoding, EmptyVector) {
  std::vector<uint8_t> empty;
  EXPECT_EQ(miru::utils::base64_encode(empty), "");
}

TEST(Base64Encoding, SingleByte) {
  std::vector<uint8_t> single_byte{0x4d};
  EXPECT_EQ(miru::utils::base64_encode(single_byte), "TQ==");
}

TEST(Base64Encoding, MultipleBytes) {
  std::vector<uint8_t> multiple_bytes{0x4d, 0x61, 0x6e};
  EXPECT_EQ(miru::utils::base64_encode(multiple_bytes), "TWFu");
}

TEST(Base64Encoding, MultipleBytesWithPadding) {
  std::vector<uint8_t> multiple_bytes_with_padding{0x4d, 0x61};
  EXPECT_EQ(miru::utils::base64_encode(multiple_bytes_with_padding), "TWE=");
}

TEST(Base64Encoding, MultipleBytesWithPaddingAndNewline) {
  std::vector<uint8_t> multiple_bytes_with_padding_and_newline{0x4d, 0x61, 0x0a};
  EXPECT_EQ(
    miru::utils::base64_encode(multiple_bytes_with_padding_and_newline), "TWEK"
  );
}

TEST(Base64Encoding, MultipleBytesWithPaddingAndNewlineAndWhitespace) {
  std::vector<uint8_t> multiple_bytes_with_padding_and_newline_and_whitespace{
    0x4d, 0x61, 0x0a, 0x20
  };
  EXPECT_EQ(
    miru::utils::base64_encode(multiple_bytes_with_padding_and_newline_and_whitespace),
    "TWEKIA=="
  );
}

TEST(Base64Encoding, MultipleBytesWithPaddingAndNewlineAndWhitespaceAndEquals) {
  std::vector<uint8_t>
    multiple_bytes_with_padding_and_newline_and_whitespace_and_equals{
      0x4d, 0x61, 0x0a, 0x20, 0x3d
    };
  EXPECT_EQ(
    miru::utils::base64_encode(
      multiple_bytes_with_padding_and_newline_and_whitespace_and_equals
    ),
    "TWEKID0="
  );
}

}  // namespace test::utils
