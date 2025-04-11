#pragma once

// std
#include <stdexcept>
#include <string>

namespace miru::query {

class ParameterNotFound : public std::runtime_error {
 public:
  ParameterNotFound(const std::string& message)
      : std::runtime_error("Parameter not found: " + message) {}
};

class TooManyResults : public std::runtime_error {
 public:
  TooManyResults(const std::string& message)
      : std::runtime_error("Too many results: " + message) {}
};

}  // namespace miru::query