#pragma once

// std
#include <string>

// internal
#include <miru/details/errors.hpp>
#include <miru/params/type.hpp>

namespace miru::params::details {

/// Indicate the parameter type does not match the expected type.
class InvalidParameterValueTypeError : public std::runtime_error {
 public:
  /// Construct an instance.
  /**
   * \param[in] expected the expected parameter type.
   * \param[in] actual the actual parameter type.
   * \param[in] error_trace the error trace.
   */
  InvalidParameterValueTypeError(
    ParameterType expected,
    ParameterType actual,
    const miru::details::errors::ErrorTrace& error_trace
  )
    : std::runtime_error(format_message(expected, actual, error_trace)) {}

  static std::string format_message(
    ParameterType expected,
    ParameterType actual,
    const miru::details::errors::ErrorTrace& error_trace
  ) {
    return "expected [" + to_string(expected) + "] got [" + to_string(actual) + "]" +
           miru::details::errors::format_source_location(error_trace);
  }
};

#define THROW_INVALID_PARAMETER_VALUE_TYPE(expected, actual)   \
  throw miru::params::details::InvalidParameterValueTypeError( \
    expected, actual, ERROR_TRACE                              \
  )

/// Thrown if passed parameter value is invalid.
class InvalidParameterValueError : public std::runtime_error {
  // Inherit constructors from runtime_error.
  using std::runtime_error::runtime_error;
};

/// Thrown if requested parameter type is invalid.
/**
 * Essentially the same as rclcpp::ParameterTypeException, but with parameter
 * name in the error message.
 */
class InvalidParameterTypeError : public std::runtime_error {
 public:
  /// Construct an instance.
  /**
   * \param[in] name the name of the parameter.
   * \param[in] message custom exception message.
   */
  InvalidParameterTypeError(
    const std::string& name,
    const std::string& message,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(name, message, trace)) {}

  static std::string format_message(
    const std::string& name,
    const std::string& message,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "parameter '" + name + "' has invalid type: " + message +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_INVALID_PARAMETER_TYPE(name, message) \
  throw miru::params::details::InvalidParameterTypeError(name, message, ERROR_TRACE)

class InvalidScalarConversionError : public std::runtime_error {
 public:
  InvalidScalarConversionError(
    const std::string& value,
    const std::string& dest_type,
    const std::string& message,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(value, dest_type, message, trace)) {}

  static std::string format_message(
    const std::string& value,
    const std::string& dest_type,
    const std::string& message,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "unable to convert scalar '" + value + "' to " + dest_type + ": " + message +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_INVALID_SCALAR_CONVERSION(value, dest_type, message) \
  throw miru::params::details::InvalidScalarConversionError(       \
    value, dest_type, message, ERROR_TRACE                         \
  )

}  // namespace miru::params::details