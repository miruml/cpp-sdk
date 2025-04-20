// std
#include "miru/http/errors.hpp"

#include <string>

// internal
#include <miru/errors.hpp>
#include <miru/http/client.hpp>

namespace miru::http::errors {

std::string InvalidContentTypeError::format_message(
  const std::string& actual_content_type, const std::string& expected_content_type,
  const miru::client::RequestContext& context, const miru::errors::ErrorTrace& trace
) {
  return "Request '" + miru::client::to_string(context) + "' returned content type '" +
         actual_content_type + "' when '" + expected_content_type + "' was expected" +
         miru::errors::format_source_location(trace);
}

std::string RequestFailedError::format_message(
  uint status, const miru::client::RequestContext& context,
  const miru::errors::ErrorTrace& trace
) {
  return "Request '" + miru::client::to_string(context) + "' returned status '" +
         std::to_string(status) + "'" + miru::errors::format_source_location(trace);
}

}  // namespace miru::http::errors
