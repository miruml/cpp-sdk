#pragma once

// std
#include <string>

// internal
#include <http/models/ErrorResponse.h>

#include <http/socket_session.hpp>
#include <miru/details/errors.hpp>

// ================================== ERRORS ======================================= //
namespace miru::http {

namespace openapi = org::openapitools::server::model;

std::string to_string(const openapi::ErrorResponse& error_response);

class InvalidContentTypeError : public std::runtime_error {
 public:
  InvalidContentTypeError(
    const std::string& actual_content_type,
    const std::string& expected_content_type,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(
        format_message(actual_content_type, expected_content_type, details, trace)
      ) {}

  static std::string format_message(
    const std::string& actual_content_type,
    const std::string& expected_content_type,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_INVALID_CONTENT_TYPE(                                  \
  actual_content_type, expected_content_type, details                \
)                                                                    \
  throw miru::http::InvalidContentTypeError(                         \
    actual_content_type, expected_content_type, details, ERROR_TRACE \
  )

class ConnectionError : public std::runtime_error {
 public:
  ConnectionError(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(error_message, details, trace)) {}

  static std::string format_message(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_CONNECTION_ERROR(error_message, details) \
  throw miru::http::ConnectionError(error_message, details, ERROR_TRACE)

class WriteError : public std::runtime_error {
 public:
  WriteError(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(error_message, details, trace)) {}

  static std::string format_message(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_WRITE_ERROR(error_message, details) \
  throw miru::http::WriteError(error_message, details, ERROR_TRACE)

class ReadError : public std::runtime_error {
 public:
  ReadError(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(error_message, details, trace)) {}

  static std::string format_message(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_READ_ERROR(error_message, details) \
  throw miru::http::ReadError(error_message, details, ERROR_TRACE)

class ShutdownError : public std::runtime_error {
 public:
  ShutdownError(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(error_message, details, trace)) {}

  static std::string format_message(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_SHUTDOWN_ERROR(error_message, details) \
  throw miru::http::ShutdownError(error_message, details, ERROR_TRACE)

class RequestFailedError : public std::runtime_error {
 public:
  RequestFailedError(
    uint status,
    const miru::http::RequestDetails& details,
    const std::optional<openapi::ErrorResponse>& error_response,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(status, details, error_response, trace)) {}

  static std::string format_message(
    uint status,
    const miru::http::RequestDetails& details,
    const std::optional<openapi::ErrorResponse>& error_response,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_REQUEST_FAILED(status, details, error_response) \
  throw miru::http::RequestFailedError(status, details, error_response, ERROR_TRACE)

class RequestTimeoutError : public std::runtime_error {
 public:
  RequestTimeoutError(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(error_message, details, trace)) {}

  static std::string format_message(
    const std::string& error_message,
    const miru::http::RequestDetails& details,
    const miru::details::errors::ErrorTrace& trace
  );
};

#define THROW_REQUEST_TIMEOUT(error_message, context) \
  throw miru::http::RequestTimeoutError(error_message, context, ERROR_TRACE)

}  // namespace miru::http