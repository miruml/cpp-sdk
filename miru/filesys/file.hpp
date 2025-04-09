#pragma once

// std
#include <filesystem>
#include <string>
#include <variant>
#include <vector>

// internal
#include <miru/filesys/path.hpp>

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
std::string_view file_type_to_string(FileType file_type);
std::vector<std::string_view> file_types_to_strings(
    const std::vector<FileType>& file_types);

FileType string_to_file_type(std::string_view str);

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

class FileNotFound : public std::runtime_error {
 public:
  explicit FileNotFound(const std::string& path);
};

class NotAFile : public std::runtime_error {
 public:
  explicit NotAFile(const std::string& path);
};

class InvalidFileType : public std::runtime_error {
 public:
  explicit InvalidFileType(const std::string& file_path);
  explicit InvalidFileType(const std::string& file_path,
                           const std::vector<FileType>& expected_file_types);
};

}  // namespace miru::filesys