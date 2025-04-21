// std
#include "miru/http/errors.hpp"

#include <string>

// internal
#include <miru/errors.hpp>
#include <miru/http/socket_session.hpp>

namespace miru::http::errors {

namespace openapi = org::openapitools::server::model; 

std::string to_string(const openapi::ErrorResponse& error_response) {
  return error_response.to_json().dump();
}

std::string InvalidContentTypeError::format_message(
  const std::string& actual_content_type,
  const std::string& expected_content_type,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) + "' returned content type '" +
         actual_content_type + "' when '" + expected_content_type + "' was expected" +
         miru::errors::format_source_location(trace);
}

std::string ConnectionError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to connect to the miru agent: " + error_message +
         miru::errors::format_source_location(trace);
}

std::string WriteError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to write: " + error_message +
         miru::errors::format_source_location(trace);
}

std::string ReadError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to read: " + error_message +
         miru::errors::format_source_location(trace);
}

std::string ShutdownError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) +
         "' failed to shutdown: " + error_message +
         miru::errors::format_source_location(trace);
}

std::string RequestFailedError::format_message(
  uint status,
  const miru::http::RequestDetails& details,
  const std::optional<openapi::ErrorResponse>& error_response,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) + "' returned status '" +
         std::to_string(status) + "' with error response: " +
         (error_response ? to_string(*error_response) : "unknown") +
         miru::errors::format_source_location(trace);
}

std::string RequestTimeoutError::format_message(
  const std::string& error_message,
  const miru::http::RequestDetails& details,
  const miru::errors::ErrorTrace& trace) {
  return "Request '" + miru::http::to_string(details) +
         "' timed out: " + error_message + miru::errors::format_source_location(trace);
}

}  // namespace miru::http::errors
