#pragma once

// std
#include <chrono>
#include <filesystem>
#include <optional>

// internal
#include <miru/params/parameter.hpp>

namespace miru::config {

// used to determine where to source the config instance from
enum class ConfigInstanceSource {
  Agent,
  FileSystem,
};

struct FromAgentOptions {
 public:
  FromAgentOptions()
    : num_retries(3),  // try to load from the agent 3 times
      retry_delay(std::chrono::milliseconds(500)),  // wait 500ms between retries
      default_instance_file_path() {}

  uint32_t num_retries;
  std::chrono::milliseconds retry_delay;
  std::optional<std::filesystem::path> default_instance_file_path;
};

// forward declare the implementation
class ConfigInstanceImpl;

class ConfigInstance {
 public:
  ConfigInstance(std::unique_ptr<ConfigInstanceImpl> impl);
  ~ConfigInstance();

  // Initialize the config instance from a file system source. The config instance and
  // its file will be read from the file system.
  static ConfigInstance from_file(
    const std::filesystem::path& schema_file_path,
    const std::filesystem::path& instance_file_path
  );

  // Initialize the config instance from the on-device agent. The config instance will
  // be retrieve from the agent while the the schema file is read from the file system.
  static ConfigInstance from_agent(
    const std::filesystem::path& schema_file_path,
    const FromAgentOptions& options = FromAgentOptions()
  );

  const ConfigInstanceSource get_source() const;
  const miru::params::Parameter& root_parameter() const;

 private:
  std::unique_ptr<ConfigInstanceImpl> impl_;

  // friends
  friend class ConfigImpl;
};

}  // namespace miru::config