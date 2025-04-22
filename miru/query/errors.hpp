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
    const std::string& message
  )
    : std::runtime_error("Unable to find parameter with filters: " + to_string(filters) + " " + message) {}
};

class TooManyResultsError : public std::runtime_error {
 public:
  TooManyResultsError(const std::string& message)
    : std::runtime_error("Too many results: " + message) {}
};

}  // namespace miru::query