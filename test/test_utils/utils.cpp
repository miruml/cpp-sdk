// std
#include <string>
#include <algorithm>


namespace miru::test_utils {

std::string sanitize_test_name(const std::string& name) {
    std::string sanitized = name;
    std::replace(sanitized.begin(), sanitized.end(), '/', '_');
    std::replace(sanitized.begin(), sanitized.end(), '.', '_');
    std::replace(sanitized.begin(), sanitized.end(), ' ', '_');
    return sanitized;
}

} // namespace miru::test_utils