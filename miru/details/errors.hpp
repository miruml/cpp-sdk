#pragma once

// std
#include <string>

namespace miru::errors::details {

struct ErrorTrace {
  const char* file;
  const char* function;
  int line;
};

std::string format_source_location(const ErrorTrace& trace);

#define ERROR_TRACE (miru::errors::details::ErrorTrace{__FILE__, __FUNCTION__, __LINE__})

}  // namespace miru::errors