// std
#include <fstream>

// internal
#include <miru/filesys/file.hpp>

// external
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <fmt/os.h>

namespace miru::filesys {

std::vector<FileType> supported_file_types() {
    return {
        FileType::JSON,
        FileType::YAML,
    };
}

std::string_view file_type_to_string(FileType file_type) {
    return magic_enum::enum_name(file_type);
}

std::vector<std::string_view> file_types_to_strings(const std::vector<FileType>& file_types) {
    std::vector<std::string_view> strings;
    for (const auto& file_type : file_types) {
        strings.push_back(magic_enum::enum_name(file_type));
    }
    return strings;
}

FileType string_to_file_type(std::string_view str) {
    return magic_enum::enum_cast<FileType>(str).value();
}

// returns the file type
FileType File::file_type() const {
    if (extension() == ".json") return FileType::JSON;
    if (extension() == ".yaml") return FileType::YAML;
    if (extension() == ".yml") return FileType::YAML;
    throw InvalidFileType(path_.string());
}

void File::assert_exists() const {
    if (!std::filesystem::exists(path_)) {
        throw FileNotFound(path_.string());
    }
    if (!std::filesystem::is_regular_file(path_)) {
        throw NotAFile(path_.string());
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
        throw InvalidFileType(path_.string());
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
        throw InvalidFileType(path_.string());
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
    throw InvalidFileType(path_.string());
}

// =============================== ERROR HANDLING ================================ //
FileNotFound::FileNotFound(const std::string& path):
    std::runtime_error("File '" + path + "' does not exist") {}

NotAFile::NotAFile(const std::string& path):
    std::runtime_error("Path '" + path + "' exists but is not a file") {}

InvalidFileType::InvalidFileType(const std::string& file_path):
    std::runtime_error(
        fmt::format(
            "'{}' is not a supported file type, must be one of: {}",
            file_path,
            fmt::join(file_types_to_strings(supported_file_types()), ", ")
        )
    ) {}

InvalidFileType::InvalidFileType(
    const std::string& file_path,
    const std::vector<FileType>& expected_file_types
) : std::runtime_error(
        fmt::format(
            "'{}' is not a supported file type, must be one of: {}",
            file_path,
            fmt::join(file_types_to_strings(expected_file_types), ", ")
        )
    ) {}


} // namespace miru::filesys