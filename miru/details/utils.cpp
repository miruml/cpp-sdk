// std
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// miru
#include <miru/details/errors.hpp>
#include <miru/errors.hpp>

namespace miru::utils::details {

bool has_prefix(const std::string_view& str, const std::string_view& prefix) {
  return str.size() >= prefix.size() &&
         std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool has_suffix(const std::string_view& str, const std::string_view& suffix) {
  return str.size() >= suffix.size() &&
         std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string remove_trailing(const std::string& str, const std::string& chars) {
  size_t last = str.find_last_not_of(chars);
  return (last != std::string::npos) ? str.substr(0, last + 1) : "";
}

std::string to_string(const std::vector<std::string>& strings) {
  std::stringstream ss;
  ss << "[";
  bool is_first = true;
  for (const auto& item : strings) {
    if (!is_first) {
      ss << ", ";
    }
    ss << item;
    is_first = false;
  }
  ss << "]";
  return ss.str();
}

void assert_unique_strings(const std::vector<std::string>& strings) {
  std::unordered_map<std::string, int> counts;
  for (const auto& str : strings) {
    counts[str]++;
  }

  std::vector<std::string> duplicates;
  for (const auto& [str, count] : counts) {
    if (count > 1) {
      duplicates.push_back(
        "'" + str + "' (appears " + std::to_string(count) + " times)"
      );
    }
  }

  if (!duplicates.empty()) {
    throw std::invalid_argument("duplicates found: " + to_string(duplicates));
  }
}

std::string to_string(const std::string& value) { return value; }

std::string to_string(const std::string_view& value) { return std::string(value); }

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

}  // namespace miru::utils