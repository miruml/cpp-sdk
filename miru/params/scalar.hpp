#pragma once

// std
#include <cstdint>
#include <string>
#include <type_traits>

// internal
#include <miru/params/errors.hpp>
#include <miru/params/type.hpp>
#include <miru/utils.hpp>

namespace miru::params {



class Scalar {
 public:
  Scalar(const std::string &value) : value_(value) {}

  bool operator==(const Scalar &other) const { return value_ == other.value_; }
  bool operator!=(const Scalar &other) const { return value_ != other.value_; }

  bool as_bool() const;
  int64_t as_int() const;
  double as_double() const;
  const std::string &as_string() const { return value_; }

  // using parameter type enum

  template <ParameterType type>
  typename std::enable_if<type == ParameterType::PARAMETER_BOOL, bool>::type as(
  ) const {
    return as_bool();
  }

  template <ParameterType type>
  typename std::enable_if<type == ParameterType::PARAMETER_INTEGER, int64_t>::type as(
  ) const {
    return as_int();
  }

  template <ParameterType type>
  typename std::enable_if<type == ParameterType::PARAMETER_DOUBLE, double>::type as(
  ) const {
    return as_double();
  }

  template <ParameterType type>
  typename std::
    enable_if<type == ParameterType::PARAMETER_STRING, const std::string &>::type
    as() const {
    return as_string();
  }

  // using c++ primitive types

  template <typename type>
  constexpr typename std::enable_if<std::is_same<type, bool>::value, bool>::type as(
  ) const {
    return as_bool();
  }

  template <typename type>
  typename std::enable_if<
    std::is_integral<type>::value && !std::is_same<type, bool>::value,
    int64_t>::type
  as() const {
    // for types returning integers other than int64 we'll use the string conversion
    // function which conducts additional checks for converting int64 to the target
    // type
    try {
      return miru::utils::string_as<type>(value_);
    } catch (const std::exception &e) {
      THROW_INVALID_SCALAR_CONVERSION(
        value_, to_string(ParameterType::PARAMETER_INTEGER), e.what()
      );
    }
  }

  template <typename type>
  typename std::enable_if<std::is_floating_point<type>::value, double>::type as(
  ) const {
    // for types returning doubles we'll use the string conversion function which
    // conducts additional checks for converting double to the target type
    try {
      return miru::utils::string_as<type>(value_);
    } catch (const std::exception &e) {
      THROW_INVALID_SCALAR_CONVERSION(
        value_, to_string(ParameterType::PARAMETER_DOUBLE), e.what()
      );
    }
  }

  template <typename type>
  typename std::
    enable_if<std::is_convertible<type, std::string>::value, const std::string &>::type
    as() const {
    return as_string();
  }

 private:
  std::string value_;
};

std::string to_string(const Scalar &scalar);
std::ostream &operator<<(std::ostream &os, const Scalar &scalar);

}  // namespace miru::params
