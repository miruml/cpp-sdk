#pragma once

// std
#include <stdexcept>
#include <string>

namespace miru::query {

class ParameterNotFoundError : public std::runtime_error {
 public:
  ParameterNotFoundError(const std::string& message)
    : std::runtime_error("Parameter not found: " + message) {}
};

class TooManyResultsError : public std::runtime_error {
 public:
  TooManyResultsError(const std::string& message)
    : std::runtime_error("Too many results: " + message) {}
};

}  // namespace miru::query