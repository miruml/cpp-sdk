// std
#include <thread>

// internal
#include <miru/http/details/models/HashSchemaSerializedRequest.h>
#include <miru/http/details/models/HashSerializedConfigSchemaFormat.h>

#include <miru/config/config.hpp>
#include <miru/config/details/builder.hpp>
#include <miru/config/details/utils.hpp>
#include <miru/config/errors.hpp>
#include <miru/http/details/socket_client.hpp>
#include <miru/params/details/parse.hpp>
#include <miru/query/ros2.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

namespace openapi = org::openapitools::server::model;

Config Config::from_file(
  const std::filesystem::path& schema_file_path,
  const std::filesystem::path& config_file_path
) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::FileSystem);

  // read the config slug from the schema file
  miru::filesys::details::File schema_file(schema_file_path);
  builder.with_schema_file(schema_file);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // read the config file
  miru::filesys::details::File config_file(config_file_path);
  builder.with_config_file(config_file);
  builder.with_data(miru::params::details::parse_file(config_slug, config_file));

  // build the config
  Config config = builder.build();
  return config;
}

std::string hash_schema(
  const miru::http::details::BackendClientI& client,
  const miru::filesys::details::File& schema_file
) {
  // determine the schema file type
  miru::filesys::details::FileType schema_file_type = schema_file.file_type();
  openapi::HashSerializedConfigSchemaFormat format;
  switch (schema_file_type) {
    case miru::filesys::details::FileType::JSON: {
      format.value = openapi::HashSerializedConfigSchemaFormat::eHashSerializedConfigSchemaFormat::HASH_SERIALIZED_CONFIG_SCHEMA_FORMAT_JSON;
      break;
    }
    case miru::filesys::details::FileType::YAML: {
      format.value = openapi::HashSerializedConfigSchemaFormat::eHashSerializedConfigSchemaFormat::HASH_SERIALIZED_CONFIG_SCHEMA_FORMAT_YAML;
      break;
    }
    default: {
      std::vector<std::string> expected_file_types = {"json", "yaml"};
      THROW_INVALID_CONFIG_SCHEMA_FILE_TYPE(schema_file, expected_file_types);
    }
  }

  std::string schema_contents = schema_file.read_string();
  openapi::HashSchemaSerializedRequest config_schema{format, schema_contents};
  return client.hash_schema(config_schema);
}

nlohmann::json get_latest_concrete_config(
  const miru::http::details::BackendClientI& client,
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
  const miru::http::details::BackendClientI& client,
  const std::filesystem::path& schema_file_path,
  const FromAgentOptions& options
) {
  ConfigBuilder builder;
  builder.with_source(ConfigSource::Agent);

  // read the config slug
  miru::filesys::details::File schema_file(schema_file_path);
  builder.with_schema_file(schema_file);
  std::string config_slug = read_schema_config_slug(schema_file);
  builder.with_config_slug(config_slug);

  // hash the schema contents to retrieve the schema digest
  std::string config_schema_digest = hash_schema(client, schema_file);
  builder.with_schema_digest(config_schema_digest);

  // load the config from the agent
  nlohmann::json concrete_config_data =
    get_latest_concrete_config(client, config_schema_digest, config_slug);
  builder.with_data(miru::params::details::parse_json_node(config_slug, concrete_config_data));

  // build the config
  Config config = builder.build();
  return config;
}

Config Config::from_agent(
  const miru::http::details::BackendClientI& client,
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
  miru::http::details::UnixSocketClient client;
  return from_agent_impl(client, schema_file_path, options);
}

miru::query::ROS2StyleQuery Config::ros2() const {
  return miru::query::ROS2StyleQuery(parameters_);
}

}  // namespace miru::config