// internal
#include <miru/config/details/utils.hpp>
#include <miru/config/errors.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

std::string read_schema_config_slug(const miru::filesys::details::File& schema_file) {
  std::string config_slug;
  switch (schema_file.file_type()) {
    case miru::filesys::details::FileType::JSON: {
      nlohmann::json json_schema_content = schema_file.read_json();
      if (!json_schema_content.contains(MIRU_CONFIG_SLUG_FIELD)) {
        THROW_CONFIG_SLUG_NOT_FOUND(schema_file);
      }
      config_slug = json_schema_content[MIRU_CONFIG_SLUG_FIELD];
      break;
    }
    case miru::filesys::details::FileType::YAML: {
      YAML::Node yaml_schema_content = schema_file.read_yaml();
      if (!yaml_schema_content[MIRU_CONFIG_SLUG_FIELD]) {
        THROW_CONFIG_SLUG_NOT_FOUND(schema_file);
      }
      config_slug = yaml_schema_content[MIRU_CONFIG_SLUG_FIELD].as<std::string>();
      break;
    }
    default:
      throw std::runtime_error("Unsupported schema file type");
  }
  if (config_slug.empty()) {
    THROW_EMPTY_CONFIG_SLUG(schema_file);
  }
  return config_slug;
}

}  // namespace miru::config