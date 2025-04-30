#pragma once

// std
#include <stdexcept>
#include <string>

// internal
#include <miru/details/errors.hpp>

namespace miru::errors {

class InvalidTypeConversionError : public std::runtime_error {
 public:
  InvalidTypeConversionError(
    const std::string& value,
    const std::string& src_type,
    const std::string& dest_type,
    const std::string& message,
    const details::ErrorTrace& trace
  )
    : std::runtime_error(format_message(value, src_type, dest_type, message, trace)) {}

  static std::string format_message(
    const std::string& value,
    const std::string& src_type,
    const std::string& dest_type,
    const std::string& message,
    const details::ErrorTrace& trace
  ) {
    return "unable to convert value '" + value + "' from type '" + src_type +
           "' to type '" + dest_type + "': " + message + format_source_location(trace);
  }
};

#define THROW_INVALID_TYPE_CONVERSION(value, src_type, dest_type, message) \
  throw miru::errors::InvalidTypeConversionError(                          \
    value, src_type, dest_type, message, ERROR_TRACE                       \
  )

}  // namespace miru::errors