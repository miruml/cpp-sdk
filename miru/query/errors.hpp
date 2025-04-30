#pragma once

// miru
#include <miru/query/filter.hpp>

// std
#include <stdexcept>
#include <string>

namespace miru::query {

class ParameterNotFoundError : public std::runtime_error {
 public:
  ParameterNotFoundError(
    const miru::query::SearchParamFilters& filters,
    const miru::errors::details::ErrorTrace& trace
  )
    : std::runtime_error(format_message(filters, trace)) {}

  static std::string format_message(
    const miru::query::SearchParamFilters& filters,
    const miru::errors::details::ErrorTrace& trace
  ) {
    return "Unable to find parameter with filters: " + to_string(filters) +
           miru::errors::details::format_source_location(trace);
  }
};

#define THROW_PARAMETER_NOT_FOUND(filters) \
  throw ParameterNotFoundError(filters, ERROR_TRACE)

class TooManyResultsError : public std::runtime_error {
 public:
  TooManyResultsError(
    const miru::query::SearchParamFilters& filters,
    const std::string& message,
    const miru::errors::details::ErrorTrace& error_trace
  )
    : std::runtime_error(format_message(filters, message, error_trace)) {}

  static std::string format_message(
    const miru::query::SearchParamFilters& filters,
    const std::string& message,
    const miru::errors::details::ErrorTrace& error_trace
  ) {
    return "Too many results: " + to_string(filters) + " " + message +
           miru::errors::details::format_source_location(error_trace);
  }
};

#define THROW_TOO_MANY_RESULTS(filters, message) \
  throw TooManyResultsError(filters, message, ERROR_TRACE)

}  // namespace miru::query