#pragma once

// std
#include <string>

// internal
#include <miru/errors.hpp>
#include <miru/http/client_fwd.hpp>

// ================================== ERRORS ======================================= //
namespace miru::http::errors {

class InvalidContentTypeError : public std::runtime_error {
 public:
  InvalidContentTypeError(
    const std::string& actual_content_type, const std::string& expected_content_type,
    const miru::client::RequestContext& context, const miru::errors::ErrorTrace& trace
  )
    : std::runtime_error(
        format_message(actual_content_type, expected_content_type, context, trace)
      ) {}

  static std::string format_message(
    const std::string& actual_content_type, const std::string& expected_content_type,
    const miru::client::RequestContext& context, const miru::errors::ErrorTrace& trace
  );
};

#define THROW_INVALID_CONTENT_TYPE(                                  \
  actual_content_type, expected_content_type, context                \
)                                                                    \
  throw miru::http::errors::InvalidContentTypeError(                 \
    actual_content_type, expected_content_type, context, ERROR_TRACE \
  )

class RequestFailedError : public std::runtime_error {
 public:
  RequestFailedError(
    uint status, const miru::client::RequestContext& context,
    const miru::errors::ErrorTrace& trace
  )
    : std::runtime_error(format_message(status, context, trace)) {}

  static std::string format_message(
    uint status, const miru::client::RequestContext& context,
    const miru::errors::ErrorTrace& trace
  );
};

#define THROW_REQUEST_FAILED(status, context) \
  throw miru::http::errors::RequestFailedError(status, context, ERROR_TRACE)

}  // namespace miru::http::errors