#pragma once

// miru
#include <miru/details/errors.hpp>
#include <miru/query/filter.hpp>

// std
#include <stdexcept>
#include <string>

namespace miru::query::details {

class ParameterNotFoundError : public std::runtime_error {
 public:
  ParameterNotFoundError(
    const miru::query::SearchParamFilters& filters,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(filters, trace)) {}

  static std::string format_message(
    const miru::query::SearchParamFilters& filters,
    const miru::details::errors::ErrorTrace& trace
  ) {
    return "Unable to find parameter with filters: " + to_string(filters) +
           miru::details::errors::format_source_location(trace);
  }
};

#define THROW_PARAMETER_NOT_FOUND(filters) \
  throw miru::query::details::ParameterNotFoundError(filters, ERROR_TRACE)

class TooManyResultsError : public std::runtime_error {
 public:
  TooManyResultsError(
    const miru::query::SearchParamFilters& filters,
    const std::string& message,
    const miru::details::errors::ErrorTrace& error_trace
  )
    : std::runtime_error(format_message(filters, message, error_trace)) {}

  static std::string format_message(
    const miru::query::SearchParamFilters& filters,
    const std::string& message,
    const miru::details::errors::ErrorTrace& error_trace
  ) {
    return "Too many results: " + to_string(filters) + " " + message +
           miru::details::errors::format_source_location(error_trace);
  }
};

#define THROW_TOO_MANY_RESULTS(filters, message) \
  throw miru::query::details::TooManyResultsError(filters, message, ERROR_TRACE)

}  // namespace miru::query::details