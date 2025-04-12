// std
#include <fstream>

// internal
#include <miru/filesys/file.hpp>
#include <miru/filesys/errors.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::filesys {

std::vector<FileType> supported_file_types() {
  return {
      FileType::JSON,
      FileType::YAML,
  };
}

std::string file_type_to_string(FileType file_type) {
  switch (file_type) {
    case FileType::JSON:
      return "JSON";
    case FileType::YAML:
      return "YAML";
  }
  return "unknown";
}

std::vector<std::string> file_types_to_strings(
    const std::vector<FileType>& file_types) {
  std::vector<std::string> strings;
  for (const auto& file_type : file_types) {
    strings.push_back(file_type_to_string(file_type));
  }
  return strings;
}

FileType string_to_file_type(std::string str) {
  std::transform(
    str.begin(),
    str.end(),
    str.begin(),
    ::tolower
  );
  if (str == "json") return FileType::JSON;
  if (str == "yaml") return FileType::YAML;
  if (str == "yml") return FileType::YAML;
  THROW_INVALID_FILE_TYPE(str, file_types_to_strings(supported_file_types()));
}

// returns the file type
FileType File::file_type() const {
  if (extension() == ".json") return FileType::JSON;
  if (extension() == ".yaml") return FileType::YAML;
  if (extension() == ".yml") return FileType::YAML;
  THROW_INVALID_FILE_TYPE(path_.string(), file_types_to_strings(supported_file_types()));
}

void File::assert_exists() const {
  if (!std::filesystem::exists(path_)) {
    THROW_FILE_NOT_FOUND(path_.string());
  }
  if (!std::filesystem::is_regular_file(path_)) {
    THROW_NOT_A_FILE(path_.string());
  }
}

// ================================= READING FILES ================================= //
std::string File::read_string() const {
  assert_exists();
  std::ifstream file(path_, std::ios::binary | std::ios::ate);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path_.string());
  }

  auto size = file.tellg();
  std::string content(size, '\0');
  file.seekg(0);
  file.read(content.data(), size);

  return content;
}

nlohmann::json File::read_json() const {
  assert_exists();

  if (file_type() != FileType::JSON) {
    THROW_INVALID_FILE_TYPE(path_.string(), file_types_to_strings(supported_file_types()));
  }

  std::ifstream file(path_);
  return nlohmann::json::parse(file);
}

YAML::Node File::read_yaml() const {
  assert_exists();

  // the yaml-cpp parser can actually parse json files as well (since json is a subset
  // of yaml) however if we want to read strict yaml then we should use the dedicated
  // yaml parser
  if (file_type() != FileType::YAML && file_type() != FileType::JSON) {
    THROW_INVALID_FILE_TYPE(path_.string(), file_types_to_strings(supported_file_types()));
  }

  std::ifstream file(path_);
  return YAML::Load(file);
}

std::variant<nlohmann::json, YAML::Node> File::read_structured_data() const {
  switch (file_type()) {
    case FileType::JSON:
      return read_json();
    case FileType::YAML:
      return read_yaml();
  }
  THROW_INVALID_FILE_TYPE(path_.string(), file_types_to_strings(supported_file_types()));
}

}  // namespace miru::filesys