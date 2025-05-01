// internal
#include <filesys/path.hpp>

namespace miru::filesys {

// returns the lexically normalized absolute path ("../" would be removed)
std::filesystem::path Path::abs_path() const {
  std::string abs_path =
    std::filesystem::absolute(path_).lexically_normal().make_preferred();
  if (abs_path.length() > 1 && abs_path.back() == '/') {
    abs_path.pop_back();
  }
  return abs_path;
}

}  // namespace miru::filesys