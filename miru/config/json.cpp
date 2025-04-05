// internal
#include <miru/config/errors.hpp>

// external
#include <nlohmann/json.hpp>
#include <fmt/ranges.h>

namespace miru::config {

void dump_json(
    std::ostream& out,
    const nlohmann::json& json,
    int indent = 2
) {
    out << json.dump(indent);
}

nlohmann::json get_json_node(
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

void dump_json_node(
    std::ostream& out,
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    dump_json(out, get_json_node(json, key));
}

bool json_node_exists(
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

bool json_node_is_null(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    nlohmann::json node = get_json_node(json, key);
    return node.is_null();
}

void list_json_nodes_helper(
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
        list_json_nodes_helper(item_value, delimiter, sub_prefix, keys);
    }
}

} // namespace miru::config
