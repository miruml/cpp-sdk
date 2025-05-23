// internal
#include <filesys/dir.hpp>
#include <filesys/errors.hpp>

namespace miru::filesys {

Dir Dir::parent() const { return Dir(abs_path().parent_path()); }

// returns the root directory of the git repository from the given path
Dir Dir::git_root() const {
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
    THROW_DIR_NOT_FOUND(path_.string(), abs_path().string());
  }
  if (!std::filesystem::is_directory(path_)) {
    THROW_NOT_A_DIR(path_.string());
  }
}

bool Dir::exists() const {
  return std::filesystem::exists(path_) && std::filesystem::is_directory(path_);
}

}  // namespace miru::filesys
