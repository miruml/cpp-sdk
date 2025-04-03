// std
#include <filesystem>
#include <fstream>

// internal
#include <miru/filesys/path.hpp>
#include <test/test_utils/utils.hpp>

// external
#include <gtest/gtest.h>

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

std::string PathTestNameGenerator(
    const testing::TestParamInfo<PathTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Path, Paths,
    testing::Values(
        PathTestCase{
            "test/file_test/test.txt",
            std::filesystem::path("test") / "file_test" / "test.txt"
        },
        PathTestCase{
            "arglebargle.json",
            std::filesystem::path("arglebargle.json")
        },
        PathTestCase{
            "lebron.james.yaml",
            std::filesystem::path("lebron.james.yaml")
        },
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

std::string AbsPathTestNameGenerator(
    const testing::TestParamInfo<AbsPathTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Path, AbsPaths,
    testing::Values(
        AbsPathTestCase{
            "already absolute path",
            std::filesystem::path("/already") / "absolute" / "path.txt",
            std::filesystem::path("/already") / "absolute" / "path.txt"
        },
        AbsPathTestCase{
            "root path with ../../random/../removals/to/see/../../if/this/works",
            std::filesystem::path("/root") / "path" / "with" / ".." / ".." / "random" / ".." / "removals" / "to" / "see" / ".." / ".." / "if" / "this" / "works",
            std::filesystem::path("/root") / "removals" / "if" / "this" / "works"
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

std::string NameTestNameGenerator(
    const testing::TestParamInfo<NameTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Path, Names,
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