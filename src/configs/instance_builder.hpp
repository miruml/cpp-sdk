#pragma once

// std
#include <optional>
#include <string>

// internal
#include <configs/instance_impl.hpp>
#include <filesys/file.hpp>
#include <miru/configs/instance.hpp>
#include <miru/params/parameter.hpp>

namespace miru::config {

class ConfigInstanceBuilder {
 public:
  ConfigInstanceBuilder() {}
  ConfigInstanceBuilder& with_schema_file(const miru::filesys::File& schema_file);
  ConfigInstanceBuilder& with_config_type_slug(const std::string& config_type_slug);
  ConfigInstanceBuilder& with_source(miru::config::ConfigInstanceSource source);
  ConfigInstanceBuilder& with_data(const miru::params::Parameter& data);
  ConfigInstanceBuilder& with_schema_digest(const std::string& schema_digest);
  ConfigInstanceBuilder& with_config_instance_file(
    const miru::filesys::File& config_instance_file
  );
  ConfigInstanceImpl build();

 private:
  // required
  std::optional<miru::filesys::File> schema_file_;
  std::optional<std::string> config_type_slug_;
  std::optional<miru::config::ConfigInstanceSource> source_;
  std::optional<miru::params::Parameter> data_;

  // only needed if sourcing from the agent
  std::optional<std::string> schema_digest_;

  // only needed if sourcing from the file system
  std::optional<miru::filesys::File> config_instance_file_;
};

}  // namespace miru::config