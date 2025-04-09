#pragma once

// internal
#include <miru/params/parameter.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

miru::params::Parameter load_yaml(const std::string& name, const YAML::Node& node,
                                  const std::string& delimiter = "/");
miru::params::Parameter load_yaml_array(const std::string& name, const YAML::Node& node,
                                        const std::string& delimiter = "/");

miru::params::Parameter load_json(const std::string& name, const nlohmann::json& node,
                                  const std::string& delimiter = "/");
miru::params::Parameter load_json_array(const std::string& name,
                                        const nlohmann::json& node,
                                        const std::string& delimiter = "/");

miru::params::Parameter load_structured_data(
    const std::variant<nlohmann::json, YAML::Node>& node);

}  // namespace miru::params