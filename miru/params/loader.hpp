#pragma once

// internal
#include <miru/params/parameter.hpp>
#include <miru/params/composite.hpp>

// external
#include <yaml-cpp/yaml.h>
#include <nlohmann/json.hpp>

namespace miru::params {

miru::params::Object load_yaml(const YAML::Node& node);
miru::params::Object load_json(const nlohmann::json& node);

}  // namespace miru::params