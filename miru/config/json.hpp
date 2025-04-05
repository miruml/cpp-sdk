#pragma once

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
);

nlohmann::json get_json_node(
    const nlohmann::json& json,
    const std::vector<std::string>& key
);

void dump_json_node(
    std::ostream& out,
    const nlohmann::json& json,
    const std::vector<std::string>& key
);

template<typename T>
T get_json_node_as(
    const nlohmann::json& json,
    const std::vector<std::string>& key
);

bool json_node_exists(
    const nlohmann::json& json,
    const std::vector<std::string>& key
);

bool json_node_is_null(
    const nlohmann::json& json,
    const std::vector<std::string>& key
);

void list_json_nodes_helper(
    const nlohmann::json& json,
    const std::string& delimiter,
    const std::string& prefix,
    std::vector<std::string>& keys
);

template<typename T>
T get_json_node_as(
    const nlohmann::json& json,
    const std::vector<std::string>& key
) {
    nlohmann::json node = get_json_node(json, key);

    // check if the current value is an object or not
    if (node.is_object()) {
        throw NotAPrimitiveValue(fmt::format("{}", fmt::join(key, "/")));
    }
    return node.get<T>();
}

} // namespace miru::config