#pragma once

// std
#include <string>
#include <vector>

namespace miru::utils {

    // split a string by a delimiter
    std::vector<std::string> split(const std::string& str, char delimiter);

} // namespace miru::utils