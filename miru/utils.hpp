#pragma once

// std
#include <algorithm>
#include <limits>
#include <string>
#include <typeinfo>
#include <vector>

// internal
#include <miru/exceptions.hpp>

namespace miru::utils {

// ================================ STRING UTILITIES =================================

bool has_prefix(const std::string_view &str, const std::string_view &prefix);
bool has_suffix(const std::string_view &str, const std::string_view &suffix);
std::string remove_trailing(const std::string &str, const std::string &chars);
void assert_unique_strings(const std::vector<std::string> &strings);

template <typename T>
std::string to_string(const std::vector<T>& values) {
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
std::string to_string(const std::vector<std::pair<std::string, T>>& values) {
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

// ================================ NUMBER CASTING ================================= //
template <typename type>
constexpr typename std::enable_if<
    std::is_integral<type>::value && !std::is_same<type, bool>::value, type>::type
int64_as(const int64_t &value) {
  // need to handle unsigned types separately
  if (std::is_unsigned<type>::value && value < 0) {
    throw utils::InvalidTypeConversion(
        std::to_string(value), "int64_t",
        "integer (type '" + std::string(typeid(type).name()) + "')",
        "value is negative for unsigned type");
  }

  // check for overflow with the target integer type
  if (value > std::numeric_limits<type>::max() ||
      value < std::numeric_limits<type>::lowest()) {
    throw utils::InvalidTypeConversion(
        std::to_string(value), "int64_t",
        "integer (type '" + std::string(typeid(type).name()) + "')",
        "value outside target integer range [" +
            std::to_string(std::numeric_limits<type>::lowest()) + ", " +
            std::to_string(std::numeric_limits<type>::max()) + "]");
  }
  return static_cast<type>(value);
}

template <typename type>
constexpr typename std::enable_if<std::is_floating_point<type>::value, type>::type
double_as(const double &value) {
  // check for overflow with the target floating point type
  if (value > std::numeric_limits<type>::max() ||
      value < std::numeric_limits<type>::lowest()) {
    throw utils::InvalidTypeConversion(
        std::to_string(value), "double",
        "floating point (type '" + std::string(typeid(type).name()) + "')",
        "value outside target floating point range [" +
            std::to_string(std::numeric_limits<type>::lowest()) + ", " +
            std::to_string(std::numeric_limits<type>::max()) + "]");
  }
  return static_cast<type>(value);
}

// ================================== STRING CONVERSIONS =============================
// //
bool yaml_string_to_bool(const std::string &str);
int64_t string_to_int64(const std::string &str);
double string_to_double(const std::string &str);

template <typename type>
constexpr typename std::enable_if<std::is_same<type, bool>::value, bool>::type
string_as(const std::string &str) {
  return yaml_string_to_bool(str);
}

template <typename type>
constexpr typename std::enable_if<
    std::is_integral<type>::value && !std::is_same<type, bool>::value, int64_t>::type
string_as(const std::string &str) {
  return int64_as<type>(string_to_int64(str));
}

template <typename type>
constexpr typename std::enable_if<std::is_floating_point<type>::value, double>::type
string_as(const std::string &str) {
  return double_as<type>(string_to_double(str));
}

template <typename type>
constexpr typename std::enable_if<std::is_convertible<type, std::string>::value,
                                  const std::string &>::type
string_as(const std::string &str) {
  return str;
}

// ============================ STRING ARRAY CONVERSIONS =========================== //
template <typename T>
struct is_primitive_type : std::false_type {};

template <>
struct is_primitive_type<bool> : std::true_type {};
template <>
struct is_primitive_type<int64_t> : std::true_type {};
template <>
struct is_primitive_type<double> : std::true_type {};
template <>
struct is_primitive_type<std::string> : std::true_type {};

template <typename T>
struct is_convertible_from_string
    : std::bool_constant<is_primitive_type<T>::value || std::is_same_v<T, bool> ||
                         std::is_convertible_v<T, int64_t> ||
                         std::is_convertible_v<T, double> ||
                         std::is_convertible_v<T, std::string>> {};

template <typename T>
typename std::enable_if<is_convertible_from_string<T>::value, std::vector<T>>::type
string_array_as(const std::vector<std::string> &strings) {
  std::vector<T> dest;
  dest.reserve(strings.size());
  std::transform(strings.begin(), strings.end(), std::back_inserter(dest),
                 [](const std::string &s) { return string_as<T>(s); });
  return dest;
}

}  // namespace miru::utils