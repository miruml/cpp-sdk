// std
#include <thread>

// internal
#include <miru/http/models/HashSchemaSerializedRequest.h>

#include <miru/config/config.hpp>
#include <miru/config/errors.hpp>
#include <miru/http/socket_client.hpp>
#include <miru/params/parse.hpp>
#include <miru/query/ros2.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

namespace openapi = org::openapitools::server::model;

std::string read_schema_config_slug(const miru::filesys::File& schema_file) {
  std::string config_slug;
  switch (schema_file.file_type()) {
    case miru::filesys::FileType::JSON: {
      nlohmann::json json_schema_content = schema_file.read_json();
      if (!json_schema_content.contains(MIRU_CONFIG_SLUG_FIELD)) {
        THROW_CONFIG_SLUG_NOT_FOUND(schema_file);
      }
      config_slug = json_schema_content[MIRU_CONFIG_SLUG_FIELD];
      break;
    }
    case miru::filesys::FileType::YAML: {
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

Config Config::from_file(
  const std::filesystem::path& schema_file_path,
  const std::filesystem::path& config_file_path
) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::FileSystem);

  // read the config slug from the schema file
  miru::filesys::File schema_file(schema_file_path);
  builder.with_schema_file(schema_file);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // read the config file
  miru::filesys::File config_file(config_file_path);
  builder.with_config_file(config_file);
  builder.with_data(miru::params::parse_file(config_slug, config_file));

  // build the config
  Config config = builder.build();
  return config;
}

std::string hash_schema(
  const miru::http::BackendClientI& client,
  const miru::filesys::File& schema_file
) {
  // determine the schema file type
  miru::filesys::FileType schema_file_type = schema_file.file_type();
  std::string format;
  switch (schema_file_type) {
    case miru::filesys::FileType::JSON: {
      format = "json";
      break;
    }
    case miru::filesys::FileType::YAML: {
      format = "yaml";
      break;
    }
    default: {
      std::vector<std::string> expected_file_types = {"json", "yaml"};
      THROW_INVALID_CONFIG_SCHEMA_FILE_TYPE(schema_file, expected_file_types);
    }
  }

  std::string schema_contents = schema_file.read_string();
  openapi::HashSchemaSerializedRequest config_schema{
    format,
    schema_contents
  };
  return client.hash_schema(config_schema);
}

nlohmann::json get_latest_concrete_config(
  const miru::http::BackendClientI& client,
  const std::string& config_schema_digest,
  const std::string& config_slug
) {
  openapi::BaseConcreteConfig config;
  try {
    // try to refresh the latest concrete config with the server
    openapi::RefreshLatestConcreteConfigRequest refresh_request{
      config_schema_digest, config_slug
    };
    config = client.refresh_latest_concrete_config(refresh_request);
  } catch (const std::exception& refresh_error) {
    // load from the local cache if the refresh fails
    config = client.get_latest_concrete_config(config_schema_digest, config_slug);
  }
  if (!config.concrete_config.has_value()) {
    THROW_EMPTY_CONCRETE_CONFIG(config_slug);
  }
  return config.concrete_config.value();
}

Config from_agent_impl(
  const miru::http::BackendClientI& client,
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::Agent);

  // read the config slug
  miru::filesys::File schema_file(schema_file_path);
  builder.with_schema_file(schema_file);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // hash the schema contents to retrieve the schema digest
  std::string config_schema_digest = hash_schema(client, schema_file);
  builder.with_schema_digest(config_schema_digest);

  // load the config from the agent
  nlohmann::json concrete_config_data =
    get_latest_concrete_config(client, config_schema_digest, config_slug);
  builder.with_data(miru::params::parse_json_node(config_slug, concrete_config_data));

  // build the config
  Config config = builder.build();
  return config;
}

Config Config::from_agent(
  const miru::http::BackendClientI& client,
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  if (options.num_retries == 0) {
    THROW_FROM_AGENT_OPTIONS_ERROR("Number of retries must be greater than 0");
  }

  // attempt to load the config from the agent for the number of retries
  std::exception_ptr last_from_agent_error;
  for (uint32_t attempt = 1; attempt <= options.num_retries; attempt++) {
    try {
      return from_agent_impl(client, schema_file_path, options);
    } catch (const std::exception& from_agent_error) {
      last_from_agent_error = std::current_exception();
    }
    // sleep between retries
    if (attempt < options.num_retries) {
      std::this_thread::sleep_for(options.retry_delay);
    }
  }

  // loading from the agent failed, try loading the default config from the file system
  if (options.default_config_path.has_value()) {
    try {
      return from_file(schema_file_path, options.default_config_path.value());
    } catch (const std::exception& from_default_config_error) {
      // do nothing
    }
  }

  // if we've made it here, we've tried to load the config from the agent
  // for the number of retries and failed each time
  // so we need to throw the last error
  std::rethrow_exception(last_from_agent_error);
}

Config Config::from_agent(
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  miru::http::UnixSocketClient client;
  return from_agent_impl(client, schema_file_path, options);
}

miru::query::ROS2StyleQuery Config::ros2() const {
  return miru::query::ROS2StyleQuery(parameters_);
}

// ================================ CONFIG BUILDER ================================ //
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
      "Schema digest not set (must be set when retrieving the config from the agent)"
    );
  }
  if (source_ == ConfigSource::FileSystem && !config_file_.has_value()) {
    throw std::runtime_error(
      "Config file not set (must be set when retrieving the config from the file "
      "system)"
    );
  }
  return Config(
    *schema_file_, *config_slug_, *source_, *data_, schema_digest_, config_file_
  );
}

}  // namespace miru::config