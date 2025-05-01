// std
#include <unistd.h>

#include <cstring>

// internal
#include <miru/http/errors.hpp>
#include <miru/http/details/utils.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>

namespace miru::http::details {

namespace http = boost::beast::http;
namespace openapi = org::openapitools::server::model;

http::request<http::string_body> build_request(
  const http::verb& method,
  const std::string& host,
  const std::string& path,
  const std::string& body
) {
  http::request<http::string_body> req;
  req.method(method);
  req.target(path);
  req.version(11);
  req.set(http::field::host, host);
  req.set(http::field::user_agent, "miru cpp sdk unix socket client");

  if (!body.empty()) {
    req.set(http::field::content_type, "application/json");
    req.body() = body;
    req.prepare_payload();
  }

  return req;
}

http::request<http::string_body>
build_get_request(const std::string& host, const std::string& path) {
  return build_request(http::verb::get, host, path);
}

http::request<http::string_body> build_post_request(
  const std::string& host,
  const std::string& path,
  const std::string& body
) {
  return build_request(http::verb::post, host, path, body);
}

nlohmann::json handle_json_response(
  const http::response<http::string_body>& res,
  const RequestDetails& details
) {
  // check the response status for errors
  if (res.result() != http::status::ok) {
    std::optional<openapi::ErrorResponse> error_response;
    try {
      error_response =
        openapi::ErrorResponse::from_json(nlohmann::json::parse(res.body()));
    } catch (const std::exception& e) {
      error_response = std::nullopt;
    }
    THROW_REQUEST_FAILED(res.result_int(), details, error_response);
  }

  return nlohmann::json::parse(res.body());
}

}  // namespace miru::http::details
