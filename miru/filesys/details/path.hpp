#pragma once

// std
#include <filesystem>

namespace miru::filesys::details {

class Path {
 public:
  explicit Path(const std::filesystem::path& path) : path_(path) {}
  std::filesystem::path path() const { return path_; }
  std::filesystem::path abs_path() const;
  std::string name() const { return path_.filename().string(); }

 protected:
  std::filesystem::path path_;
};

}  // namespace miru::filesys::details
