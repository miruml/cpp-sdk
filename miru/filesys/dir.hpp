#pragma once

// std
#include <filesystem>

// internal
#include <miru/filesys/path.hpp>
#include <miru/filesys/file.hpp>

namespace miru::filesys {

// error handling

class Dir: public Path {
public:
    explicit Dir(const std::filesystem::path& path) : Path(path) {}

    static Dir current_dir() { return Dir(std::filesystem::current_path()); }

    void assert_exists() const;
    bool exists() const;

    Dir parent() const;
    Dir subdir(const std::filesystem::path& path) const {
        return Dir(path_ / path);
    }
    miru::filesys::File file(const std::filesystem::path& path) const {
        return miru::filesys::File(path_ / path);
    }

    Dir git_repo_root_dir() const;
};

class DirNotFound : public std::runtime_error {
public:
    explicit DirNotFound(
        const std::string& path
    ) : std::runtime_error("Directory '" + path + "' does not exist") {}
};

class NotADir : public std::runtime_error {
public:
    explicit NotADir(
        const std::string& path
    ) : std::runtime_error("Path '" + path + "' exists but is not a directory") {}
};

class UnableToFindGitRepo : public std::runtime_error {
public:
    explicit UnableToFindGitRepo(
        const std::string& path
    ) : std::runtime_error("The path '" + path + "' is not part of a git repository") {}
};

} // namespace miru::filesys
