#pragma once

// std
#include <cstdint>
#include <string>
#include <vector>

namespace miru::utils {

// ================================ STRING UTILITIES =================================

bool has_prefix(const std::string_view &str, const std::string_view &prefix);
bool has_suffix(const std::string_view &str, const std::string_view &suffix);
std::string remove_trailing(const std::string &str, const std::string &chars);
void assert_unique_strings(const std::vector<std::string> &strings);

std::string to_string(const std::vector<std::string> &strings);
std::string to_string(const std::string &value);
std::string to_string(const std::string_view &value);

template <typename T>
std::string to_string(const std::vector<T> &values) {
  if (values.empty()) {
    return "[]";
  }

  std::string result = "[";
  for (size_t i = 0; i < values.size(); ++i) {
    result += to_string(values[i]);
    if (i < values.size() - 1) {
      result += ", ";
    }
  }
  result += "]";
  return result;
}

template <typename T>
std::string to_string(const std::vector<std::pair<std::string, T>> &values) {
  if (values.empty()) {
    return "[]";
  }

  std::string result = "[";
  for (size_t i = 0; i < values.size(); ++i) {
    result += to_string(values[i]);
    if (i < values.size() - 1) {
      result += ", ";
    }
  }
  result += "]";
  return result;
}

// ================================ BASE64 ENCODING ==================================
// //
std::string base64_encode(const std::vector<uint8_t> &bytes);

}  // namespace miru::utils