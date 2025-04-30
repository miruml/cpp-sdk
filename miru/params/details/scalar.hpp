#pragma once

// std
#include <algorithm>
#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

// internal
#include <miru/params/scalar.hpp>

namespace miru::params::details {

template <typename T>
struct is_scalar_type : std::false_type {};

template <>
struct is_scalar_type<bool> : std::true_type {};
template <>
struct is_scalar_type<int64_t> : std::true_type {};
template <>
struct is_scalar_type<double> : std::true_type {};
template <>
struct is_scalar_type<std::string> : std::true_type {};

template <typename T>
struct is_convertible_to_scalar_type
  : std::bool_constant<
      is_scalar_type<T>::value || std::is_same_v<T, bool> ||
      std::is_convertible_v<T, int64_t> || std::is_convertible_v<T, double> ||
      std::is_convertible_v<T, std::string>> {};

template <typename T>
typename std::enable_if<is_convertible_to_scalar_type<T>::value, std::vector<T>>::type
scalar_array_as(const std::vector<Scalar> &scalars) {
  std::vector<T> dest;
  dest.reserve(scalars.size());
  std::transform(
    scalars.begin(),
    scalars.end(),
    std::back_inserter(dest),
    [](const Scalar &s) { return s.as<T>(); }
  );
  return dest;
}

}  // namespace miru::params::details