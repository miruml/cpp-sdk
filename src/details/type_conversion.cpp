// std
#include <algorithm>
#include <string>

// internal
#include <miru/details/type_conversion.hpp>

namespace miru::details::type_conversion {

bool yaml_string_to_bool(const std::string& str) {
  std::string lower = str;
  std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
  if (lower == "y" || lower == "yes" || lower == "true" || lower == "on") {
    return true;
  }
  if (lower == "n" || lower == "no" || lower == "false" || lower == "off") {
    return false;
  }
  THROW_INVALID_TYPE_CONVERSION(
    str, "string", "bool", "cannot interpret value as a boolean"
  );
}

int64_t string_to_int64(const std::string& str) {
  if (str.find(".") != std::string::npos) {
    THROW_INVALID_TYPE_CONVERSION(
      str,
      "string",
      "int64_t",
      "cannot interpret value as an integer: contains a decimal point"
    );
  }
  try {
    size_t pos = 0;
    int64_t result = std::stoll(str, &pos);
    if (pos != str.size()) {
      THROW_INVALID_TYPE_CONVERSION(
        str, "string", "int64_t", "contains invalid characters"
      );
    }
    return result;
  } catch (const std::exception& e) {
    THROW_INVALID_TYPE_CONVERSION(
      str,
      "string",
      "int64_t",
      "cannot interpret value as an integer: " + std::string(e.what())
    );
  }
}

double string_to_double(const std::string& str) {
  try {
    size_t pos = 0;
    double result = std::stod(str, &pos);
    if (pos != str.size()) {
      THROW_INVALID_TYPE_CONVERSION(
        str, "string", "double", "contains invalid characters"
      );
    }
    return result;
  } catch (const std::exception& e) {
    THROW_INVALID_TYPE_CONVERSION(
      str,
      "string",
      "double",
      "cannot interpret value as a double: " + std::string(e.what())
    );
  }
}

}  // namespace miru::details::type_conversion