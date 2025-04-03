// std
#include <filesystem>
#include <string>

// internal
#include <miru/filesys/dir.hpp>
#include <test/test_utils/utils.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

enum class ExceptionType {
    None,
    DirNotFound,
    NotADir,
    UnableToFindGitRepo,
};

// ==================================== exists() =================================== //
struct ExistsTestCase {
    std::string test_name;
    std::string path;
    ExceptionType expected_exception;
};

class Exists : public testing::TestWithParam<ExistsTestCase> {};

TEST_P(Exists, Exists) {
    const auto& [test_name, path, expected_exception] = GetParam();
    miru::filesys::Dir dir(path);

    switch (expected_exception) {
        case ExceptionType::None:
            EXPECT_TRUE(dir.exists());
            EXPECT_NO_THROW(dir.assert_exists());
            break;
        case ExceptionType::DirNotFound:
            EXPECT_FALSE(dir.exists());
            EXPECT_THROW(dir.assert_exists(), miru::filesys::DirNotFound);
            break;
        case ExceptionType::NotADir:
            EXPECT_FALSE(dir.exists());
            EXPECT_THROW(dir.assert_exists(), miru::filesys::NotADir);
            break;
        default:
            FAIL() << "Unexpected exception type: " << static_cast<int>(expected_exception);
    }
}

std::string ExistsTestNameGenerator(
    const testing::TestParamInfo<ExistsTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Dir, Exists,
    testing::Values(
        ExistsTestCase{
            "directory exists",
            miru::test_utils::filesys_testdata_dir().path(),
            ExceptionType::None
        },
        ExistsTestCase{
            "directory does not exist",
            "doesnt/exist",
            ExceptionType::DirNotFound
        },
        ExistsTestCase{
            "is a file",
            miru::test_utils::filesys_testdata_dir().path() / "text.txt",
            ExceptionType::NotADir
        }
    ),
    ExistsTestNameGenerator
);

// ================================== parent() ===================================== //
struct ParentTestCase {
    std::string test_name;
    std::string path;
    std::string parent_path;
};

class Parent : public testing::TestWithParam<ParentTestCase> {};

TEST_P(Parent, Parent) {
    const auto& [test_name, path, expected_parent] = GetParam();
    miru::filesys::Dir dir(path);
    EXPECT_EQ(dir.parent().path(), expected_parent);
}

std::string ParentTestNameGenerator(
    const testing::TestParamInfo<ParentTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Dir, Parent,
    testing::Values(
        ParentTestCase{
            "absolute path",
            "/root/dir/with/subdir",
            "/root/dir/with"
        },
        ParentTestCase{
            "relative path",
            "/root/dir/with/subdir/../..",
            "/root"
        },
        ParentTestCase{
            "root path",
            "/",
            "/"
        }
    ),
    ParentTestNameGenerator
);

// ================================== subdir() ===================================== //
struct SubdirTestCase {
    std::string test_name;
    std::filesystem::path path;
    std::filesystem::path subdir_extension;
    std::filesystem::path expected_subdir_path;
};

class Subdir : public testing::TestWithParam<SubdirTestCase> {};

TEST_P(Subdir, Subdir) {
    const auto& [test_name, path, subdir_extension, expected_subdir_path] = GetParam();
    miru::filesys::Dir dir(path);
    EXPECT_EQ(dir.subdir(subdir_extension).path(), expected_subdir_path);
}

std::string SubdirTestNameGenerator(
    const testing::TestParamInfo<SubdirTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Dir, Subdir,
    testing::Values(
        SubdirTestCase{
            "single extension",
            std::filesystem::path("lebron") / "the",
            std::filesystem::path("goat"),
            std::filesystem::path("lebron") / "the" / "goat"
        },
        SubdirTestCase{
            "multiple extensions",
            std::filesystem::path("like") / "he",
            std::filesystem::path("fr") / "fr" / "the" / "goat",
            std::filesystem::path("like") / "he" / "fr" / "fr" / "the" / "goat"
        },
        SubdirTestCase{
            "trailing slash",
            std::filesystem::path("but") / "like",
            std::filesystem::path("seriously") / "/",
            std::filesystem::path("but") / "like" / "seriously" / "/"
        }
    ),
    SubdirTestNameGenerator
);

// ==================================== file() ===================================== //
struct FileTestCase {
    std::string test_name;
    std::filesystem::path path;
    std::filesystem::path file_extension;
    std::filesystem::path expected_file_path;
};

class File : public testing::TestWithParam<FileTestCase> {};

TEST_P(File, File) {
    const auto& [test_name, path, file_extension, expected_file_path] = GetParam();
    miru::filesys::Dir dir(path);
    EXPECT_EQ(dir.file(file_extension).path(), expected_file_path);
}

std::string FileTestNameGenerator(
    const testing::TestParamInfo<FileTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Dir, File,
    testing::Values(
        FileTestCase{
            "single extension",
            std::filesystem::path("lebron") / "the",
            std::filesystem::path("goat.txt"),
            std::filesystem::path("lebron") / "the" / "goat.txt"
        },
        FileTestCase{
            "multiple extensions",
            std::filesystem::path("like") / "he",
            std::filesystem::path("fr") / "fr" / "the" / "goat.txt",
            std::filesystem::path("like") / "he" / "fr" / "fr" / "the" / "goat.txt"
        },
        FileTestCase{
            "trailing slash",
            std::filesystem::path("but") / "like",
            std::filesystem::path("seriously") / "/",
            std::filesystem::path("but") / "like" / "seriously" / "/"
        }
    ),
    FileTestNameGenerator
);

// =============================== git_repo_root_dir() ============================= //
struct GitRepoRootDirTestCase {
    std::string test_name;
    std::filesystem::path path;
    std::filesystem::path expected_git_repo_root_dir;
    ExceptionType expected_exception;
};

class GitRepoRootDirs : public testing::TestWithParam<GitRepoRootDirTestCase> {};

TEST_P(GitRepoRootDirs, GitRepoRootDir) {
    const auto& [test_name, path, expected_git_repo_root_dir, expected_exception] = GetParam();
    miru::filesys::Dir dir(path);
    switch (expected_exception) {
        case ExceptionType::None:
            EXPECT_EQ(dir.git_repo_root_dir().path(), expected_git_repo_root_dir);
            break;
        case ExceptionType::DirNotFound:
            EXPECT_THROW(dir.git_repo_root_dir(), miru::filesys::DirNotFound);
            break;
        case ExceptionType::UnableToFindGitRepo:
            EXPECT_THROW(dir.git_repo_root_dir(), miru::filesys::UnableToFindGitRepo);
            break;
        default:
            FAIL() << "Unexpected exception type: " << static_cast<int>(expected_exception);
    }
}

std::string GitRepoRootDirTestNameGenerator(
    const testing::TestParamInfo<GitRepoRootDirTestCase>& info
) {
    return miru::test_utils::sanitize_test_name(info.param.test_name);
}

INSTANTIATE_TEST_SUITE_P(Dir, GitRepoRootDirs,
    testing::Values(
        GitRepoRootDirTestCase{
            "git repo from filesystem data directory",
            miru::test_utils::filesys_testdata_dir().path(),
            miru::filesys::Dir(miru::test_utils::filesys_testdata_dir().path() / ".." / ".." / "..").abs_path(),
            ExceptionType::None
        },
        GitRepoRootDirTestCase{
            "git repo from current directory",
            miru::filesys::Dir::current_dir().path(),
            miru::filesys::Dir(miru::test_utils::filesys_testdata_dir().path() / ".." / ".." / "..").abs_path(),
            ExceptionType::None
        },
        GitRepoRootDirTestCase{
            "directory does not exist",
            std::filesystem::path("doesnt") / "exist",
            std::filesystem::path(),
            ExceptionType::DirNotFound
        },
        GitRepoRootDirTestCase{
            "is not a git repo",
            miru::test_utils::filesys_testdata_dir().path() / ".." / ".." / ".." / "..",
            std::filesystem::path(),
            ExceptionType::UnableToFindGitRepo
        }
    ),
    GitRepoRootDirTestNameGenerator
);

