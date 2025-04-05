#pragma once

// internal
#include <miru/config/errors.hpp>

// external
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <fmt/ranges.h>

namespace miru::config {

void dump_yaml(
    std::ostream& out,
    const YAML::Node& yaml,
    int indent = 2
);

YAML::Node get_yaml_param_node(
    const YAML::Node &node,
    const std::vector<std::string> &key
);

void dump_yaml_param(
    std::ostream& out,
    const YAML::Node& yaml,
    const std::vector<std::string>& key
);

template <typename T>
T get_yaml_param(
    const YAML::Node &yaml,
    const std::vector<std::string> &key
);

bool yaml_param_exists(
    const YAML::Node& node,
    const std::vector<std::string>& key
);

bool yaml_param_is_null(
    const YAML::Node& yaml,
    const std::vector<std::string>& key
);

void list_yaml_param_keys_helper(
    const YAML::Node& node,
    const std::string& delimiter,
    const std::string& prefix,
    std::vector<std::string>& keys
);

std::vector<std::string> list_yaml_param_keys(
    const YAML::Node& node,
    const std::string& delimiter = "/"
);

template <typename T>
T get_yaml_param(
    const YAML::Node &yaml,
    const std::vector<std::string> &key
) {
    YAML::Node node = get_yaml_param_node(yaml, key);
    // check if the current value is an object or not
    if (node.IsMap()) {
        throw NotAPrimitiveValue(fmt::format("{}", fmt::join(key, "/")));
    }
    return node.as<T>();
}

} // namespace miru::config