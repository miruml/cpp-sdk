// std
#include <string>

// internal
#include <miru/details/errors.hpp>

namespace miru::details::errors {

std::string format_source_location(const ErrorTrace& trace) {
  return "\n at " + std::string(trace.file) + ":" + std::to_string(trace.line) +
         " in " + std::string(trace.function);
}

}  // namespace miru::details::errors