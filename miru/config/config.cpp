// internal
#include <miru/client/models/HashSchemaRequest.h>

#include <miru/client/unix_socket.hpp>
#include <miru/config/config.hpp>
#include <miru/params/load.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

namespace openapi = org::openapitools::server::model;

Config Config::from_file(const std::string& schema_file_path,
                         const std::string& config_file_path) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::FileSystem);

  // read the config slug from the schema file
  miru::filesys::File schema_file(schema_file_path);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // read the config file
  miru::filesys::File config_file(config_file_path);
  builder.with_config_file(config_file);
  std::variant<nlohmann::json, YAML::Node> config_data =
      config_file.read_structured_data();
  builder.with_data(miru::params::load_structured_data(config_data));

  // build the config
  Config config = builder.build();
  return config;
}

Config Config::from_agent(const std::string& schema_file_path) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::Agent);

  // read the config slug
  miru::filesys::File schema_file(schema_file_path);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // get the config from the agent
  miru::client::UnixSocketClient client;
  openapi::HashSchemaRequest config_schema{schema_file.read_string()};
  std::string config_schema_digest = client.hash_schema(config_schema);
  builder.with_schema_digest(config_schema_digest);
  nlohmann::json config_data =
      client.get_concrete_config(config_schema_digest, config_slug);
  builder.with_data(miru::params::load_structured_data(config_data));

  // build the config
  Config config = builder.build();
  return config;
}

std::string read_schema_config_slug(const miru::filesys::File& schema_file) {
  std::string config_slug;
  switch (schema_file.file_type()) {
    case miru::filesys::FileType::JSON: {
      nlohmann::json json_schema_content = schema_file.read_json();
      config_slug = json_schema_content["config_slug"];
      break;
    }
    case miru::filesys::FileType::YAML: {
      YAML::Node yaml_schema_content = schema_file.read_yaml();
      config_slug = yaml_schema_content["config_slug"].as<std::string>();
      break;
    }
    default:
      throw std::runtime_error("Unsupported schema file type");
  }
  if (config_slug.empty()) {
    throw ConfigSlugNotFound(schema_file);
  }
  return config_slug;
}

ConfigBuilder& ConfigBuilder::with_schema_file(const miru::filesys::File& schema_file) {
  if (schema_file_.has_value()) {
    throw std::runtime_error("Schema file already set");
  }
  schema_file_ = schema_file;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_config_slug(const std::string& config_slug) {
  if (config_slug_.has_value()) {
    throw std::runtime_error("Config slug already set");
  }
  config_slug_ = config_slug;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_source(ConfigSource source) {
  if (source_.has_value()) {
    throw std::runtime_error("Source already set");
  }
  source_ = source;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_data(const miru::params::Parameter& data) {
  if (data_.has_value()) {
    throw std::runtime_error("Data already set");
  }
  data_ = data;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_schema_digest(const std::string& schema_digest) {
  if (schema_digest_.has_value()) {
    throw std::runtime_error("Schema digest already set");
  }
  schema_digest_ = schema_digest;
  return *this;
}

ConfigBuilder& ConfigBuilder::with_config_file(const miru::filesys::File& config_file) {
  if (config_file_.has_value()) {
    throw std::runtime_error("Config file already set");
  }
  config_file_ = config_file;
  return *this;
}

Config ConfigBuilder::build() {
  if (!schema_file_.has_value()) {
    throw std::runtime_error("Schema file not set");
  }
  if (!config_slug_.has_value()) {
    throw std::runtime_error("Config slug not set");
  }
  if (!source_.has_value()) {
    throw std::runtime_error("Source not set");
  }
  if (!data_.has_value()) {
    throw std::runtime_error("Data not set");
  }
  if (source_ == ConfigSource::Agent && !schema_digest_.has_value()) {
    throw std::runtime_error(
        "Schema not set (must be set when retrieving the config from the agent)");
  }
  if (source_ == ConfigSource::FileSystem && !config_file_.has_value()) {
    throw std::runtime_error(
        "Config file not set (must be set when retrieving the config from the file "
        "system)");
  }
  return Config(*schema_file_, *config_slug_, *source_, *data_, schema_digest_,
                config_file_);
}
}  // namespace miru::config