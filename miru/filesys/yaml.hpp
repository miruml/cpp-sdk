#pragma once

// external
#include <yaml-cpp/yaml.h>

namespace miru::filesys {

template <typename T>
T get_yaml_param(
    const YAML::Node &node,
    const std::vector<std::string> &keys
) {
    for (const auto &k : keys)
    {
        node = node[k];
    }
    return node.as<T>();
}

} // namespace miru::filesys