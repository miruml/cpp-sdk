#pragma once

// std
#include <string>

// internal
#include <miru/http/errors.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

// ================================== CLIENT ======================================= //
namespace miru::http::details {

namespace beast = boost::beast;
namespace http = beast::http;
using tcp = boost::asio::ip::tcp;

http::request<http::string_body> build_request(
  const http::verb& method,
  const std::string& host,
  const std::string& path,
  const std::string& body = ""
);

http::request<http::string_body>
build_get_request(const std::string& host, const std::string& path);

http::request<http::string_body> build_post_request(
  const std::string& host,
  const std::string& path,
  const std::string& body
);

nlohmann::json handle_json_response(
  const http::response<http::string_body>& res,
  const RequestDetails& details
);

}  // namespace miru::http::details
