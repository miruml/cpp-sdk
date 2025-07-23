// std
#include <thread>

// internal
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/HashSerializedConfigSchemaFormat.h>

#include <configs/errors.hpp>
#include <configs/instance_builder.hpp>
#include <http/socket_client.hpp>
#include <miru/configs/instance.hpp>
#include <params/parse.hpp>

namespace miru::config {

namespace openapi = org::openapitools::server::model;

std::string read_schema_config_type_slug(const miru::filesys::File& schema_file) {
  std::string config_type_slug;
  switch (schema_file.file_type()) {
    case miru::filesys::FileType::JSON: {
      nlohmann::json json_schema_content = schema_file.read_json();
      if (!json_schema_content.contains(MIRU_CONFIG_TYPE_SLUG_FIELD)) {
        THROW_CONFIG_TYPE_SLUG_NOT_FOUND(schema_file);
      }
      config_type_slug = json_schema_content[MIRU_CONFIG_TYPE_SLUG_FIELD];
      break;
    }
    case miru::filesys::FileType::YAML: {
      YAML::Node yaml_schema_content = schema_file.read_yaml();
      if (!yaml_schema_content[MIRU_CONFIG_TYPE_SLUG_FIELD]) {
        THROW_CONFIG_TYPE_SLUG_NOT_FOUND(schema_file);
      }
      config_type_slug =
        yaml_schema_content[MIRU_CONFIG_TYPE_SLUG_FIELD].as<std::string>();
      break;
    }
    default:
      throw std::runtime_error("Unsupported schema file type");
  }
  if (config_type_slug.empty()) {
    THROW_EMPTY_CONFIG_TYPE_SLUG(schema_file);
  }
  return config_type_slug;
}

// ================================= FROM FILE ===================================== //
ConfigInstanceImpl ConfigInstanceImpl::from_file(
  const std::filesystem::path& schema_file_path,
  const std::filesystem::path& instance_file_path
) {
  ConfigInstanceBuilder builder;
  builder.with_source(miru::config::ConfigInstanceSource::FileSystem);

  // read the config type slug from the schema file
  miru::filesys::File schema_file(schema_file_path);
  builder.with_config_schema_file(schema_file);
  std::string config_type_slug = read_schema_config_type_slug(schema_file);
  builder.with_config_type_slug(config_type_slug);

  // read the config instance file
  miru::filesys::File config_instance_file(instance_file_path);
  builder.with_config_instance_file(config_instance_file);
  builder.with_data(miru::params::parse_file(config_type_slug, config_instance_file));

  // build the config instance
  ConfigInstanceImpl config_instance = builder.build();
  return config_instance;
}

std::string hash_schema(
  const miru::http::AgentClientI& client,
  const miru::filesys::File& schema_file
) {
  // determine the schema file type
  miru::filesys::FileType schema_file_type = schema_file.file_type();
  openapi::HashSerializedConfigSchemaFormat format;
  switch (schema_file_type) {
    case miru::filesys::FileType::JSON: {
      format.value = openapi::HashSerializedConfigSchemaFormat::
        eHashSerializedConfigSchemaFormat::HASH_SERIALIZED_CONFIG_SCHEMA_FORMAT_JSON;
      break;
    }
    case miru::filesys::FileType::YAML: {
      format.value = openapi::HashSerializedConfigSchemaFormat::
        eHashSerializedConfigSchemaFormat::HASH_SERIALIZED_CONFIG_SCHEMA_FORMAT_YAML;
      break;
    }
    default: {
      std::vector<std::string> expected_file_types = {"json", "yaml"};
      THROW_INVALID_CONFIG_SCHEMA_FILE_TYPE(schema_file, expected_file_types);
    }
  }

  std::vector<uint8_t> contents = schema_file.read_bytes();
  std::string base64_contents = miru::utils::base64_encode(contents);
  openapi::HashSchemaSerializedRequest config_schema{format, base64_contents};
  return client.hash_schema(config_schema);
}

nlohmann::json get_deployed_config_instance(
  const miru::http::AgentClientI& client,
  const std::string& config_schema_digest,
  const std::string& config_type_slug
) {
  openapi::ConfigInstance config_instance;
  config_instance = client.get_deployed_config_instance(
    config_schema_digest, config_type_slug
  );
  return config_instance.content;
}

ConfigInstanceImpl from_agent_impl(
  const miru::http::AgentClientI& client,
  const std::filesystem::path& schema_file_path,
  const miru::config::FromAgentOptions& options
) {
  ConfigInstanceBuilder builder;
  builder.with_source(miru::config::ConfigInstanceSource::Agent);

  // read the config type slug
  miru::filesys::File schema_file(schema_file_path);
  builder.with_config_schema_file(schema_file);
  std::string config_type_slug = read_schema_config_type_slug(schema_file);
  builder.with_config_type_slug(config_type_slug);

  // hash the schema contents to retrieve the schema digest
  std::string config_schema_digest = hash_schema(client, schema_file);
  builder.with_config_schema_digest(config_schema_digest);

  // load the config instance from the agent
  nlohmann::json config_instance_data =
    get_deployed_config_instance(client, config_schema_digest, config_type_slug);
  builder.with_data(
    miru::params::parse_json_node(config_type_slug, config_instance_data)
  );

  // build the config instance
  ConfigInstanceImpl config_instance = builder.build();
  return config_instance;
}

ConfigInstanceImpl ConfigInstanceImpl::from_agent(
  const miru::http::AgentClientI& client,
  const std::filesystem::path& schema_file_path,
  const miru::config::FromAgentOptions& options
) {
  if (options.num_retries == 0) {
    THROW_FROM_AGENT_OPTIONS_ERROR("Number of retries must be greater than 0");
  }

  // attempt to load the config instance from the agent for the number of retries
  std::exception_ptr last_from_agent_error;
  std::string last_from_agent_error_msg;
  for (uint32_t attempt = 1; attempt <= options.num_retries; attempt++) {
    try {
      return from_agent_impl(client, schema_file_path, options);
    } catch (const std::exception& from_agent_error) {
      last_from_agent_error = std::current_exception();
      last_from_agent_error_msg = from_agent_error.what();
    }
    // sleep between retries
    if (attempt < options.num_retries) {
      std::this_thread::sleep_for(options.retry_delay);
    }
  }

  // loading from the agent failed, try loading the default config instance from the
  // file system
  if (options.default_instance_file_path.has_value()) {
    try {
      return from_file(schema_file_path, options.default_instance_file_path.value());
    } catch (const std::exception& from_default_file_error) {
      THROW_GET_DEPLOYED_CONFIG_INSTANCE_ERROR(
        last_from_agent_error_msg,
        options.default_instance_file_path.value(),
        from_default_file_error.what()
      );
    }
  }

  // if we've made it here, we've tried to load the config instance from the agent
  // for the number of retries and failed each time
  // so we need to throw the last error
  std::rethrow_exception(last_from_agent_error);
}

ConfigInstanceImpl ConfigInstanceImpl::from_agent(
  const std::filesystem::path& schema_file_path,
  const miru::config::FromAgentOptions& options
) {
  miru::http::UnixSocketClient client;
  return from_agent_impl(client, schema_file_path, options);
}

}  // namespace miru::config