// std
#include <unistd.h>

#include <cstring>

// internal
#include <miru/http/client.hpp>
#include <miru/http/errors.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>

namespace miru::client {

namespace http = boost::beast::http;

std::string to_string(const RequestContext& context) {
  return std::string(boost::beast::http::to_string(context.method)) + " " +
         context.url + " " + "(timeout: " + std::to_string(context.timeout.count()) +
         "ms)";
}

http::request<http::string_body> HTTPClient::build_request(
  const http::verb& method, const std::string& path, const std::string& body
) {
  http::request<http::string_body> req;
  req.method(method);
  req.target(path);
  req.version(11);
  req.set(http::field::host, host_);
  req.set(http::field::user_agent, "Miru Unix Client (BoostBeast)");

  if (!body.empty()) {
    req.set(http::field::content_type, "application/json");
    req.body() = body;
    req.prepare_payload();
  }

  return req;
}

http::request<http::string_body> HTTPClient::build_get_request(const std::string& path
) {
  return build_request(http::verb::get, path);
}

http::request<http::string_body> HTTPClient::build_post_request(
  const std::string& path, const std::string& body
) {
  return build_request(http::verb::post, path, body);
}

nlohmann::json HTTPClient::unmarshal_json_response(
  const http::response<http::string_body>& res, const RequestContext& req_context
) {
  // check the response body
  auto content_type = res.find(http::field::content_type);
  if (content_type == res.end() || content_type->value() != "application/json") {
    std::string actual_content_type =
      (content_type != res.end() ? std::string(content_type->value()) : "not set");
    THROW_INVALID_CONTENT_TYPE(actual_content_type, "application/json", req_context);
  }

  // parse the response body
  return nlohmann::json::parse(res.body());
}

}  // namespace miru::client
