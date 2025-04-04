#pragma once

// external
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>
#include <fmt/ranges.h>

namespace miru::config {

class ParamNotFound : public std::runtime_error {
    public:
        ParamNotFound(
            const std::string& param_path
        ) : runtime_error("Parameter '" + param_path + "' not found") {}
};

class NotAPrimitiveValue : public std::runtime_error {
    public:
        NotAPrimitiveValue(
            const std::string& param_path
        ) : runtime_error("Found object for parameter '" + param_path + "' when a primitive value (4, true, 'hello', etc.) was expected") {}
};

void dump_json(
    std::ostream& out,
    const nlohmann::json& json,
    int indent = 2
) {
    out << json.dump(indent);
}

void dump_yaml(
    std::ostream& out,
    const YAML::Node& yaml,
    int indent = 2
) {
    out << yaml;
}

nlohmann::json get_json_param_node(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    const nlohmann::json* current = &json;

    for (const auto& k : key) {
        if (!current->contains(k)) {
            throw ParamNotFound(fmt::format("{}", fmt::join(key, "/")));
        }
        current = &current->at(k);
    }
    return *current;
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

void dump_json_param(
    std::ostream& out,
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    dump_json(out, get_json_param_node(json, key));
}

void dump_yaml_param(
    std::ostream& out,
    const YAML::Node& yaml,
    const std::vector<std::string>& key
) {
    dump_yaml(out, get_yaml_param_node(yaml, key));
}

template<typename T>
T get_json_param(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    nlohmann::json node = get_json_param_node(json, key);

    // check if the current value is an object or not
    if (node.is_object()) {
        throw NotAPrimitiveValue(fmt::format("{}", fmt::join(key, "/")));
    }
    return node.get<T>();
}

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

bool json_param_exists(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    const nlohmann::json* current = &json;
    for (const auto& k : key) {
        if (!current->contains(k)) {
            return false;
        }
        current = &current->at(k);
    }
    return true;
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

bool json_param_is_null(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    nlohmann::json node = get_json_param_node(json, key);
    return node.is_null();
}

bool yaml_param_is_null(
    const YAML::Node& node,
    const std::vector<std::string>& key
) {
    YAML::Node node = get_yaml_param_node(node, key);
    return node.IsNull();
}


void list_json_param_keys_helper(
    const nlohmann::json& json,
    const std::string& delimiter,
    const std::string& prefix,
    std::vector<std::string>& keys
) {
    // is a leaf node
    if (!json.is_object()) {
        keys.push_back(prefix);
        return;
    }

    // is an object
    for (const auto& [item_key, item_value] : json.items()) {
        std::string sub_prefix = prefix + delimiter + item_key;
        list_json_param_keys_helper(item_value, delimiter, sub_prefix, keys);
    }
}

std::vector<std::string> list_json_param_keys(
    const nlohmann::json& json,
    const std::string& delimiter = "/"
) {
    std::vector<std::string> keys;
    list_json_param_keys_helper(json, delimiter, "", keys);
    return keys;
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