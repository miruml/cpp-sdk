#pragma once

// std
#include <optional>
#include <string>

// internal
#include <miru/filesys/file.hpp>
#include <miru/params/parameter.hpp>
#include <miru/query/ros2_fwd.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

const std::string MIRU_CONFIG_SLUG_FIELD_ID = "$miru_config_slug";

// used to determine where to source the config from
enum class ConfigSource {
  Agent,
  FileSystem,
};

// Config class
class Config {
 public:
  // Initialize the config from a file system source. The config will read its
  // configuration and schema file from the file system.
  static Config from_file(
    const std::string& schema_file_path,
    const std::string& config_file_path
  );

  // Initialize the config from an agent source. The config will read its
  // configuration and schema file from the on-device agent.
  static Config from_agent(const std::string& schema_file_path);

  const miru::params::Parameter& root_parameter() const {
    return parameters_;
  }

  miru::query::ROS2StyleQuery ros2() const;

 private:
  friend class ConfigBuilder;

  Config(
    const miru::filesys::File& schema_file,
    const std::string& config_slug,
    ConfigSource source,
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
  ConfigSource source_;
  miru::params::Parameter parameters_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_file_;
};

std::string read_schema_config_slug(const miru::filesys::File& schema_file);

class ConfigBuilder {
 public:
  ConfigBuilder() {}
  ConfigBuilder& with_schema_file(const miru::filesys::File& schema_file);
  ConfigBuilder& with_config_slug(const std::string& config_slug);
  ConfigBuilder& with_source(ConfigSource source);
  ConfigBuilder& with_data(const miru::params::Parameter& data);
  ConfigBuilder& with_schema_digest(const std::string& schema_digest);
  ConfigBuilder& with_config_file(const miru::filesys::File& config_file);
  Config build();

 private:
  // required
  std::optional<miru::filesys::File> schema_file_;
  std::optional<std::string> config_slug_;
  std::optional<ConfigSource> source_;
  std::optional<miru::params::Parameter> data_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_file_;
};

}  // namespace miru::config