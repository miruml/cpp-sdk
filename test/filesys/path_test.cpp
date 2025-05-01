// std
#include <filesystem>

// internal
#include <filesys/path.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

namespace test::filesys {

// =================================== path() ====================================== //
struct PathTestCase {
  std::string test_name;
  std::filesystem::path path;
};

class Paths : public testing::TestWithParam<PathTestCase> {};

TEST_P(Paths, Path) {
  const auto& [test_name, expected_path] = GetParam();
  miru::filesys::Path path(expected_path);
  EXPECT_EQ(path.path(), expected_path);
}

std::string PathTestNameGenerator(const testing::TestParamInfo<PathTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
  Path,
  Paths,
  testing::Values(
    PathTestCase{
      "test/file_test/test.txt",
      std::filesystem::path("test") / "file_test" / "test.txt"
    },
    PathTestCase{"arglebargle.json", std::filesystem::path("arglebargle.json")},
    PathTestCase{"lebron.james.yaml", std::filesystem::path("lebron.james.yaml")},
    PathTestCase{
      "ske/de/pop/pop/pop.txt",
      std::filesystem::path("ske") / "de" / "pop" / "pop" / "pop.txt"
    }
  ),
  PathTestNameGenerator
);

// =================================== abs_path() ================================== //
struct AbsPathTestCase {
  std::string test_name;
  std::filesystem::path path;
  std::filesystem::path expected_abs_path;
};

class AbsPaths : public testing::TestWithParam<AbsPathTestCase> {};

TEST_P(AbsPaths, AbsPath) {
  const auto& [test_name, path_arg, expected_abs_path] = GetParam();
  miru::filesys::Path path(path_arg);
  EXPECT_EQ(path.abs_path(), expected_abs_path);
}

std::string AbsPathTestNameGenerator(const testing::TestParamInfo<AbsPathTestCase>& info
) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
  Path,
  AbsPaths,
  testing::Values(
    AbsPathTestCase{
      "already absolute path",
      std::filesystem::path("/already") / "absolute" / "path.txt",
      std::filesystem::path("/already") / "absolute" / "path.txt"
    },
    AbsPathTestCase{
      "root path with ../../random/../removals/to/see/../../if/this/works",
      std::filesystem::path("/root") / "path" / "with" / ".." / ".." / "random" / ".." /
        "removals" / "to" / "see" / ".." / ".." / "if" / "this" / "works",
      std::filesystem::path("/root") / "removals" / "if" / "this" / "works"
    },
    AbsPathTestCase{
      "extra tailing slashes",
      std::filesystem::path("////"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "relative path with .",
      std::filesystem::path("////"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "single slash",
      std::filesystem::path("/"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "double slash",
      std::filesystem::path("//"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "triple slash",
      std::filesystem::path("///"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "dot double slash",
      std::filesystem::path(".//"),
      std::filesystem::current_path(),
    },
    AbsPathTestCase{
      "double slash dot dot",
      std::filesystem::path("//.."),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "dot dot double slash",
      std::filesystem::path("..//"),
      std::filesystem::current_path().parent_path(),
    },
    AbsPathTestCase{
      "root dot dot double slash",
      std::filesystem::path("/..//"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "root dot double dot",
      std::filesystem::path("/.//./"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "multiple dots and slashes",
      std::filesystem::path("././/./"),
      std::filesystem::current_path(),
    },
    AbsPathTestCase{
      "relative path with multiple slashes",
      std::filesystem::path("path//to///thing"),
      std::filesystem::current_path() / "path" / "to" / "thing",
    },
    AbsPathTestCase{
      "absolute path with multiple slashes",
      std::filesystem::path("/path//to///thing"),
      std::filesystem::path("/path/to/thing"),
    },
    AbsPathTestCase{
      "dot slash",
      std::filesystem::path("./"),
      std::filesystem::current_path(),
    },
    AbsPathTestCase{
      "root dot slash",
      std::filesystem::path("/./"),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "dot slash with file",
      std::filesystem::path("./test"),
      std::filesystem::current_path() / "test",
    },
    AbsPathTestCase{
      "dot slash in middle of path",
      std::filesystem::path("./test/./path"),
      std::filesystem::current_path() / "test" / "path",
    },
    AbsPathTestCase{
      "absolute path with dot slash",
      std::filesystem::path("/test/./path/"),
      std::filesystem::path("/test/path"),
    },
    AbsPathTestCase{
      "trailing dot",
      std::filesystem::path("test/path/."),
      std::filesystem::current_path() / "test" / "path",
    },
    AbsPathTestCase{
      "root parent",
      std::filesystem::path("/.."),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "root parent with path",
      std::filesystem::path("/../test"),
      std::filesystem::path("/test"),
    },
    AbsPathTestCase{
      "single parent",
      std::filesystem::path("test/.."),
      std::filesystem::current_path(),
    },
    AbsPathTestCase{
      "parent at end",
      std::filesystem::path("test/path/.."),
      std::filesystem::current_path() / "test",
    },
    AbsPathTestCase{
      "parent in middle",
      std::filesystem::path("test/../path"),
      std::filesystem::current_path() / "path",
    },
    AbsPathTestCase{
      "absolute parent in middle",
      std::filesystem::path("/test/../path"),
      std::filesystem::path("/path"),
    },
    AbsPathTestCase{
      "double parent at end",
      std::filesystem::path("test/path/../../"),
      std::filesystem::current_path(),
    },
    AbsPathTestCase{
      "triple parent at end",
      std::filesystem::path("test/path/../../.."),
      std::filesystem::current_path().parent_path(),
    },
    AbsPathTestCase{
      "absolute triple parent",
      std::filesystem::path("/test/path/../../.."),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "absolute quad parent",
      std::filesystem::path("/test/path/../../../.."),
      std::filesystem::path("/"),
    },
    AbsPathTestCase{
      "relative quad parent",
      std::filesystem::path("test/path/../../../.."),
      std::filesystem::current_path().parent_path().parent_path(),
    },
    AbsPathTestCase{
      "parent with new path",
      std::filesystem::path("test/path/../../another/path"),
      std::filesystem::current_path() / "another" / "path",
    },
    AbsPathTestCase{
      "parent with new path and parent",
      std::filesystem::path("test/path/../../another/path/.."),
      std::filesystem::current_path() / "another",
    },
    AbsPathTestCase{
      "parent prefix",
      std::filesystem::path("../test"),
      std::filesystem::current_path().parent_path() / "test",
    },
    AbsPathTestCase{
      "parent prefix with slash",
      std::filesystem::path("../test/"),
      std::filesystem::current_path().parent_path() / "test",
    },
    AbsPathTestCase{
      "parent prefix with path",
      std::filesystem::path("../test/path"),
      std::filesystem::current_path().parent_path() / "test" / "path",
    },
    AbsPathTestCase{
      "parent prefix with parent",
      std::filesystem::path("../test/.."),
      std::filesystem::current_path().parent_path(),
    }
  ),
  AbsPathTestNameGenerator
);

// ===================================== name() ==================================== //
struct NameTestCase {
  std::string test_name;
  std::filesystem::path path;
  std::filesystem::path expected_name;
};

class Names : public testing::TestWithParam<NameTestCase> {};

TEST_P(Names, Name) {
  const auto& [test_name, path_arg, expected_name] = GetParam();
  miru::filesys::Path path(path_arg);
  EXPECT_EQ(path.name(), expected_name);
}

std::string NameTestNameGenerator(const testing::TestParamInfo<NameTestCase>& info) {
  return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(
  Path,
  Names,
  testing::Values(
    // files
    NameTestCase{
      "json file",
      std::filesystem::path("arglebargle.json"),
      std::filesystem::path("arglebargle.json")
    },
    NameTestCase{
      "text file",
      std::filesystem::path("ske") / "de" / "pop" / "pop" / "pop.txt",
      std::filesystem::path("pop.txt")
    },
    // dirs
    NameTestCase{
      "test directory",
      std::filesystem::path("test") / "file_test" / "test",
      std::filesystem::path("test")
    },
    NameTestCase{
      "arglebargle directory",
      std::filesystem::path("arglebargle"),
      std::filesystem::path("arglebargle")
    }
  ),
  NameTestNameGenerator
);

}  // namespace test::filesys
