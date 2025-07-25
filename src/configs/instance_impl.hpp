#pragma once

// std
#include <optional>
#include <string>

// internal
#include <filesys/file.hpp>
#include <http/client.hpp>
#include <miru/configs/instance.hpp>
#include <miru/params/parameter.hpp>

namespace miru::config {

const std::string MIRU_CONFIG_TYPE_SLUG_FIELD = "$miru_config_type_slug";

std::ostream& operator<<(std::ostream& os, const ConfigInstanceSource& source);

// Config class
class ConfigInstanceImpl {
 public:
  // Initialize the config instance from a file system source. The config instance and
  // its schema will be read from the file system.
  static ConfigInstanceImpl from_file(
    const std::filesystem::path& cfg_sch_file_path,
    const std::filesystem::path& cfg_inst_file_path
  );

  static ConfigInstanceImpl from_agent(
    const miru::http::AgentClientI& client,
    const std::filesystem::path& schema_file_path,
    const miru::config::FromAgentOptions& options = miru::config::FromAgentOptions()
  );

  // Initialize the config instance from the on-device agent. The config instance will
  // be retrieve from the agent while the the schema file is read from the file system.
  static ConfigInstanceImpl from_agent(
    const std::filesystem::path& schema_file_path,
    const miru::config::FromAgentOptions& options = miru::config::FromAgentOptions()
  );

  const miru::config::ConfigInstanceSource get_source() const { return source_; }
  const miru::params::Parameter& root_parameter() const { return parameters_; }

 private:
  ConfigInstanceImpl(
    const miru::filesys::File& config_schema_file,
    const std::string& config_type_slug,
    miru::config::ConfigInstanceSource source,
    const miru::params::Parameter& parameters,
    const std::optional<std::string>& config_schema_digest,
    const std::optional<miru::filesys::File>& config_instance_file
  )
    : config_schema_file_(config_schema_file),
      config_type_slug_(config_type_slug),
      source_(source),
      parameters_(parameters),
      config_schema_digest_(config_schema_digest),
      config_instance_file_(config_instance_file) {}

  // required
  miru::filesys::File config_schema_file_;
  std::string config_type_slug_;
  miru::config::ConfigInstanceSource source_;
  miru::params::Parameter parameters_;

  // only needed if sourcing from the agent
  std::optional<std::string> config_schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_instance_file_;

  // friends
  friend class ConfigInstanceBuilder;
};

std::string read_schema_config_type_slug(const miru::filesys::File& schema_file);

}  // namespace miru::config