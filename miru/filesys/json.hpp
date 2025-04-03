#pragma once

// external
#include <nlohmann/json.hpp>

namespace miru::filesys {

template<typename T>
T get_json_param(
    const nlohmann::json& json,
    const std::vector<std::string>& keys
) {
    const nlohmann::json* current = &json;
    for (const auto& k : keys) {
        current = &current->at(k);  // drill down
    }
    return current->get<T>();
}

} // namespace miru::filesys
