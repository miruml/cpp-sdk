#pragma once

// std
#include <chrono>
#include <filesystem>
#include <optional>
#include <string>

// internal
#include <miru/params/parameter.hpp>

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

// forward declare the implementation
class ConfigImpl;

// Config class
class Config {
 public:
  Config(std::unique_ptr<ConfigImpl> impl);
  ~Config();

  // Initialize the config from a file system source. The config will read its
  // configuration and schema file from the file system.
  static Config from_file(
    const std::filesystem::path& schema_file_path,
    const std::filesystem::path& config_file_path
  );

  // Initialize the config from an agent source. The config will read its configuration
  // and schema file from the on-device agent.
  static Config from_agent(
    const std::filesystem::path& schema_file_path,
    const FromAgentOptions& options = FromAgentOptions()
  );

  const ConfigSource get_source() const;
  const miru::params::Parameter& root_parameter() const;

 private:
  std::unique_ptr<ConfigImpl> impl_;

  // friends
  friend class ConfigImpl;
};

}  // namespace miru::config