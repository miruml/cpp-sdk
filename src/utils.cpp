// std
#include <algorithm>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// internal
#include <utils.hpp>

// external
#include <boost/beast/core/detail/base64.hpp>

namespace miru::utils {

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

std::string base64_encode(const std::vector<uint8_t>& bytes) {
  // while this is technically a private implementation detail of boost, it is commonly
  // used in practice, stable, and avoids importing a new dependency
  std::size_t encoded_size = boost::beast::detail::base64::encoded_size(bytes.size());
  std::string result;
  result.resize(encoded_size);
  boost::beast::detail::base64::encode(&result[0], bytes.data(), bytes.size());

  return result;
}

}  // namespace miru::utils