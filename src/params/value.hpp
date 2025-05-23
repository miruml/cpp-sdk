#pragma once

// std
#include <string>

// internal
#include <miru/params/composite.hpp>
#include <miru/params/scalar.hpp>
#include <miru/params/type.hpp>
#include <params/errors.hpp>

// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::params {

std::string to_string(const ParameterValue &value, const int indent);

std::string to_string(
  const std::vector<ParameterValue> &array,
  const int indent,
  const bool with_newlines
);

std::string to_string(const miru::params::Map &map, const int indent);

}  // namespace miru::params