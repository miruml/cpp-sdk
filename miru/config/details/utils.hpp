#pragma once

// std
#include <string>

// internal
#include <miru/config/config.hpp>
// external
#include <yaml-cpp/yaml.h>

#include <nlohmann/json.hpp>

namespace miru::config {

const std::string MIRU_CONFIG_SLUG_FIELD = "$miru_config_slug";

std::string read_schema_config_slug(const miru::filesys::details::File& schema_file);

}  // namespace miru::config