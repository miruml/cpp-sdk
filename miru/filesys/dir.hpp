#pragma once

// std
#include <filesystem>

// internal
#include <miru/filesys/file.hpp>
#include <miru/filesys/path.hpp>

namespace miru::filesys {

class Dir : public Path {
 public:
  explicit Dir(const std::filesystem::path& path) : Path(path) {}

  static Dir from_current_dir() { return Dir(std::filesystem::current_path()); }

  void assert_exists() const;
  bool exists() const;

  Dir parent() const;
  Dir subdir(const std::filesystem::path& path) const { return Dir(path_ / path); }
  miru::filesys::File file(const std::filesystem::path& path) const {
    return miru::filesys::File(path_ / path);
  }

  Dir git_root() const;
};

}  // namespace miru::filesys