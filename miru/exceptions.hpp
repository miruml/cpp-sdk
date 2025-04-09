#pragma once

// std
#include <string>
#include <stdexcept>

namespace miru::utils {

class InvalidTypeConversion : public std::runtime_error {
public:
    InvalidTypeConversion(
        const std::string & value,
        const std::string & src_type,
        const std::string & dest_type,
        const std::string & message
    )
    : std::runtime_error("unable to convert value '" + value + "' from type '" + src_type + "' to type '" + dest_type + "': " + message)
    {}
};

} // namespace miru