// internal
#include <miru/filesys/dir.hpp>
#include <miru/filesys/errors.hpp>
namespace miru::filesys {

Dir Dir::parent() const { return Dir(abs_path().parent_path()); }

// returns the root directory of the git repository from the given path
Dir Dir::git_repo_root_dir() const {
  assert_exists();
  Dir current = Dir(abs_path());

  while (!current.subdir(".git").exists()) {
    if (current.parent().path_ == current.path_) {
      THROW_UNABLE_TO_FIND_GIT_REPO(path_.string());
    }
    current = current.parent();
  }

  return Dir(current);
}

void Dir::assert_exists() const {
  if (!std::filesystem::exists(path_)) {
    THROW_DIR_NOT_FOUND(path_.string());
  }
  if (!std::filesystem::is_directory(path_)) {
    THROW_NOT_A_DIR(path_.string());
  }
}

bool Dir::exists() const {
  return std::filesystem::exists(path_) && std::filesystem::is_directory(path_);
}

}  // namespace miru::filesys
