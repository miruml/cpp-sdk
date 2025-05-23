// std
#include <string>

// internal
#include <http/errors.hpp>
#include <http/socket_session.hpp>
#include <miru/details/errors.hpp>

namespace miru::http {

namespace openapi = org::openapitools::server::model;

std::string to_string(const openapi::ErrorResponse& error_response) {
  std::stringstream ss;
  ss << "[" << error_response.error.code << "] " << error_response.error.message;
  return ss.str();
}

std::string InvalidContentTypeError::format_message(
  const std::string& actual_content_type,
  const std::string& expected_content_type,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) + "' returned content type '" +
         actual_content_type + "' when '" + expected_content_type + "' was expected" +
         miru::details::errors::format_source_location(trace);
}

std::string ConnectionError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to connect to the miru agent: " + error_message +
         miru::details::errors::format_source_location(trace);
}

std::string WriteError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to write: " + error_message +
         miru::details::errors::format_source_location(trace);
}

std::string ReadError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to read: " + error_message +
         miru::details::errors::format_source_location(trace);
}

std::string ShutdownError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to shutdown: " + error_message +
         miru::details::errors::format_source_location(trace);
}

std::string RequestFailedError::format_message(
  uint status,
  const miru::http::RequestDetails& details,
  const std::optional<openapi::ErrorResponse>& error_response,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) + "' returned status '" +
         std::to_string(status) + "' with error response: " +
         (error_response ? to_string(*error_response) : "unknown") +
         miru::details::errors::format_source_location(trace);
}

std::string RequestTimeoutError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::details::errors::ErrorTrace& trace
) {
  return "Request '" + miru::http::to_string(details) +
         "' timed out: " + error_message +
         miru::details::errors::format_source_location(trace);
}

}  // namespace miru::http
