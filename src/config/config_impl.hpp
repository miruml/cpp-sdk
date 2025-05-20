#pragma once

// std
#include <optional>
#include <string>

// internal
#include <filesys/file.hpp>
#include <http/client.hpp>
#include <miru/config/config.hpp>
#include <miru/params/parameter.hpp>

namespace miru::config {

const std::string MIRU_CONFIG_SLUG_FIELD = "$miru_config_slug";

// Config class
class ConfigImpl {
 public:
  // Initialize the config from a file system source. The config will read its
  // configuration and schema file from the file system.
  static ConfigImpl from_file(
    const std::filesystem::path& schema_file_path,
    const std::filesystem::path& config_instance_file_path
  );

  static ConfigImpl from_agent(
    const miru::http::BackendClientI& client,
    const std::filesystem::path& schema_file_path,
    const miru::config::FromAgentOptions& options = miru::config::FromAgentOptions()
  );

  // Initialize the config from an agent source. The config will read its configuration
  // and schema file from the on-device agent.
  static ConfigImpl from_agent(
    const std::filesystem::path& schema_file_path,
    const miru::config::FromAgentOptions& options = miru::config::FromAgentOptions()
  );

  const miru::config::ConfigSource get_source() const { return source_; }
  const miru::params::Parameter& root_parameter() const { return parameters_; }

 private:
  ConfigImpl(
    const miru::filesys::File& schema_file,
    const std::string& config_slug,
    miru::config::ConfigSource source,
    const miru::params::Parameter& parameters,
    const std::optional<std::string>& schema_digest,
    const std::optional<miru::filesys::File>& config_file
  )
    : schema_file_(schema_file),
      config_slug_(config_slug),
      source_(source),
      parameters_(parameters),
      schema_digest_(schema_digest),
      config_file_(config_file) {}

  // required
  miru::filesys::File schema_file_;
  std::string config_slug_;
  miru::config::ConfigSource source_;
  miru::params::Parameter parameters_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_file_;

  // friends
  friend class ConfigBuilder;
};

std::string read_schema_config_slug(const miru::filesys::File& schema_file);

}  // namespace miru::config