#pragma once

// std
#include <string>

namespace miru::details::errors {

struct ErrorTrace {
  const char* file;
  const char* function;
  int line;
};

std::string format_source_location(const ErrorTrace& trace);

#define ERROR_TRACE \
  (miru::details::errors::ErrorTrace{__FILE__, __FUNCTION__, __LINE__})

}  // namespace miru::details::errors