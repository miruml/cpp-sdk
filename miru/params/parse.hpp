#pragma once

// internal
#include <miru/filesys/file.hpp>
#include <miru/params/parameter.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

miru::params::Parameter
parse_yaml_node(const std::string& name, const YAML::Node& node);

miru::params::Parameter
parse_yaml_array(const std::string& name, const YAML::Node& node);

miru::params::Parameter
parse_json_node(const std::string& name, const nlohmann::json& node);

miru::params::Parameter
parse_json_array(const std::string& name, const nlohmann::json& node);

miru::params::Parameter parse_structured_data(
  const std::variant<nlohmann::json, YAML::Node>& node
);

miru::params::Parameter
parse_file(const std::string& name, const miru::filesys::File& file);

}  // namespace miru::params