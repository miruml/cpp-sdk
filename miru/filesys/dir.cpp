// internal
#include <miru/filesys/dir.hpp>

namespace miru::filesys {

Dir Dir::parent() const {
    return Dir(abs_path().parent_path());
}

// returns the root directory of the git repository from the given path
Dir Dir::git_repo_root_dir() const {
    assert_exists();
    Dir current = Dir(abs_path());

    while (!current.subdir(".git").exists()) {
        if (current.parent().path_ == current.path_) {
            throw UnableToFindGitRepo(path_.string());
        }
        current = current.parent();
    }

    return Dir(current);
}

void Dir::assert_exists() const {
    if (!std::filesystem::exists(path_)) {
        throw DirNotFound(path_.string());
    }
    if (!std::filesystem::is_directory(path_)) {
        throw NotADir(path_.string());
    }
}

bool Dir::exists() const {
    return std::filesystem::exists(path_) && std::filesystem::is_directory(path_);
}

} // namespace miru::filesys
