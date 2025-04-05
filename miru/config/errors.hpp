#pragma once

// std
#include <stdexcept>
#include <string>

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

} // namespace miru::config