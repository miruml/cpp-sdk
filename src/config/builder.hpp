#pragma once

// std
#include <optional>
#include <string>

// internal
#include <config/config_impl.hpp>
#include <filesys/file.hpp>
#include <miru/config/config.hpp>
#include <miru/params/parameter.hpp>

namespace miru::config {

class ConfigBuilder {
 public:
  ConfigBuilder() {}
  ConfigBuilder& with_schema_file(const miru::filesys::File& schema_file);
  ConfigBuilder& with_config_slug(const std::string& config_slug);
  ConfigBuilder& with_source(miru::config::ConfigSource source);
  ConfigBuilder& with_data(const miru::params::Parameter& data);
  ConfigBuilder& with_schema_digest(const std::string& schema_digest);
  ConfigBuilder& with_config_file(const miru::filesys::File& config_file);
  ConfigImpl build();

 private:
  // required
  std::optional<miru::filesys::File> schema_file_;
  std::optional<std::string> config_slug_;
  std::optional<miru::config::ConfigSource> source_;
  std::optional<miru::params::Parameter> data_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_file_;
};

}  // namespace miru::config