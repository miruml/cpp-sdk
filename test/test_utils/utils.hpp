#pragma once

#include <string>

namespace miru::test_utils {

// sanitize test names
std::string sanitize_test_name(const std::string& name);

}  // namespace miru::test_utils