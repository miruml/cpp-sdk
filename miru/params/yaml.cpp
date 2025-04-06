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
) {
    out << yaml;
}

YAML::Node get_yaml_param_node(
    const YAML::Node &node,
    const std::vector<std::string> &key
) {
    const YAML::Node* current = &node;
    for (const auto &k : key)
    {
        const YAML::Node& next = (*current)[k];
        if (!next.IsDefined()) {
            throw ParamNotFound(fmt::format("{}", fmt::join(key, "/")));
        }
        current = &next;
    }
    return *current;
}

void dump_yaml_param(
    std::ostream& out,
    const YAML::Node& yaml,
    const std::vector<std::string>& key
) {
    dump_yaml(out, get_yaml_param_node(yaml, key));
}

bool yaml_param_exists(
    const YAML::Node& node,
    const std::vector<std::string>& key
) {
    const YAML::Node* current = &node;
    for (const auto& k : key) {
        const YAML::Node& next = (*current)[k];
        if (!next.IsDefined()) {
            return false;
        }
        current = &next;
    }
    return true;
}

bool yaml_param_is_null(
    const YAML::Node& yaml,
    const std::vector<std::string>& key
) {
    YAML::Node node = get_yaml_param_node(node, key);
    return node.IsNull();
}

void list_yaml_param_keys_helper(
    const YAML::Node& node,
    const std::string& delimiter,
    const std::string& prefix,
    std::vector<std::string>& keys
) {
    // is a leaf node
    if (!node.IsMap()) {
        keys.push_back(prefix);
        return;
    }

    // is an object
    for (const auto& item : node) {
        std::string sub_prefix = prefix + delimiter + item.first.Scalar();
        list_yaml_param_keys_helper(item.second, delimiter, sub_prefix, keys);
    }
}

std::vector<std::string> list_yaml_param_keys(
    const YAML::Node& node,
    const std::string& delimiter = "/"
) {
    std::vector<std::string> keys;
    list_yaml_param_keys_helper(node, delimiter, "", keys);
    return keys;
}

} // namespace miru::config