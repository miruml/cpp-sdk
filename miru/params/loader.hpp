#pragma once

// internal
#include <miru/params/parameter.hpp>

// external
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

namespace miru::params {

miru::params::Parameter load_yaml(const YAML::Node& node);
miru::params::Parameter load_json(const nlohmann::json& node);

}  // namespace miru::params