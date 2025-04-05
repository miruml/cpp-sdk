// std
#include <sstream>

// internal
#include <miru/utils.hpp>

namespace miru::utils {

    // split a string by a delimiter
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

} // namespace miru::utils