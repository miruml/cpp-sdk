#pragma once

// std
#include <stdexcept>
#include <string>

namespace miru::errors {

struct ErrorTrace {
  const char* file;
  const char* function;
  int line;
};

std::string format_source_location(const ErrorTrace& trace);

class InvalidTypeConversionError : public std::runtime_error {
 public:
  InvalidTypeConversionError(
    const std::string& value,
    const std::string& src_type,
    const std::string& dest_type,
    const std::string& message,
    const ErrorTrace& trace)
    : std::runtime_error(format_message(value, src_type, dest_type, message, trace)) {}

  static std::string format_message(
    const std::string& value,
    const std::string& src_type,
    const std::string& dest_type,
    const std::string& message,
    const ErrorTrace& trace) {
    return "unable to convert value '" + value + "' from type '" + src_type +
           "' to type '" + dest_type + "': " + message + format_source_location(trace);
  }
};

#define ERROR_TRACE (miru::errors::ErrorTrace{__FILE__, __FUNCTION__, __LINE__})

#define THROW_INVALID_TYPE_CONVERSION(value, src_type, dest_type, message) \
  throw errors::InvalidTypeConversionError(                                \
    value, src_type, dest_type, message, ERROR_TRACE)

}  // namespace miru::errors