#pragma once

// std
#include <optional>
#include <string>

// internal
#include <miru/config/details/config_fwd.hpp>
#include <miru/filesys/details/file.hpp>
#include <miru/http/details/client.hpp>
#include <miru/http/details/socket_client.hpp>
#include <miru/params/parameter.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

// used to determine where to source the config from
enum class ConfigSource {
  Agent,
  FileSystem,
};

struct FromAgentOptions {
 public:
  FromAgentOptions()
    : num_retries(3),  // try to load from the agent 3 times
      retry_delay(std::chrono::milliseconds(500)),  // wait 500ms between retries
      default_config_path() {}

  uint32_t num_retries;
  std::chrono::milliseconds retry_delay;
  std::optional<std::string> default_config_path;
};

// Config class
class Config {
 public:
  // Initialize the config from a file system source. The config will read its
  // configuration and schema file from the file system.
  static Config from_file(
    const std::filesystem::path& schema_file_path,
    const std::filesystem::path& config_file_path
  );

  static Config from_agent(
    const miru::http::details::BackendClientI& client,
    const std::filesystem::path& schema_file_path,
    const FromAgentOptions& options = FromAgentOptions()
  );

  // Initialize the config from an agent source. The config will read its configuration
  // and schema file from the on-device agent.
  static Config from_agent(
    const std::filesystem::path& schema_file_path,
    const FromAgentOptions& options = FromAgentOptions()
  );

  const ConfigSource get_source() const { return source_; }
  const miru::params::Parameter& root_parameter() const { return parameters_; }

 private:
  Config(
    const miru::filesys::details::File& schema_file,
    const std::string& config_slug,
    ConfigSource source,
    const miru::params::Parameter& parameters,
    const std::optional<std::string>& schema_digest,
    const std::optional<miru::filesys::details::File>& config_file
  )
    : schema_file_(schema_file),
      config_slug_(config_slug),
      source_(source),
      parameters_(parameters),
      schema_digest_(schema_digest),
      config_file_(config_file) {}

  // required
  miru::filesys::details::File schema_file_;
  std::string config_slug_;
  ConfigSource source_;
  miru::params::Parameter parameters_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::details::File> config_file_;

  // friends
  friend class details::ConfigBuilder;
};

}  // namespace miru::config