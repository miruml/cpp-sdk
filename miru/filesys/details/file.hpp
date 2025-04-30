#pragma once

// std
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

// internal
#include <miru/filesys/details/path.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::filesys {

// file types
enum class FileType {
  JSON,
  YAML,
};

std::vector<FileType> supported_file_types();
std::string file_type_to_string(FileType file_type);
std::vector<std::string> file_types_to_strings(const std::vector<FileType>& file_types);
FileType string_to_file_type(std::string str);

class File : public Path {
 public:
  explicit File(const std::filesystem::path& path) : Path(path) {}

  std::string extension() const { return path_.extension().string(); }
  FileType file_type() const;
  void assert_exists() const;

  nlohmann::json read_json() const;
  YAML::Node read_yaml() const;
  std::variant<nlohmann::json, YAML::Node> read_structured_data() const;
  std::string read_string() const;
};

}  // namespace miru::filesys