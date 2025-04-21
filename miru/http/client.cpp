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


// Start the asynchronous operation
void Session::execute(std::string port) {
  // Look up the domain name
  resolver_.async_resolve(
      req_.find(http::field::host)->value(), port,
      beast::bind_front_handler(&Session::on_resolve, shared_from_this())
    );
}

void Session::on_resolve(beast::error_code ec, tcp::resolver::results_type results) {
  if (ec) {
    THROW_RESOLVE_ERROR(ec.what(), context_);
    }

    // Set a timeout on the operation
    stream_.expires_after(timeout_);

    // Make the connection on the IP address we get from a lookup
    stream_.async_connect(
      results, beast::bind_front_handler(&Session::on_connect, shared_from_this())
    );
  }

  void Session::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type) {
    if (ec) THROW_CONNECTION_ERROR(ec.what(), context_);

    // Set a timeout on the operation
    stream_.expires_after(timeout_);

    // Send the HTTP request to the remote host
    http::async_write(
      stream_, req_, beast::bind_front_handler(&Session::on_write, shared_from_this())
    );
  }

  void Session::on_write(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) THROW_WRITE_ERROR(ec.what(), context_);

    // Set a timeout on the operation
    stream_.expires_after(timeout_);

    // Receive the HTTP response
    http::async_read(
      stream_, buffer_, res_,
      beast::bind_front_handler(&Session::on_read, shared_from_this())
    );
  }

  void Session::on_read(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);

    if (ec) THROW_READ_ERROR(ec.what(), context_);

    // Gracefully close the socket
    stream_.socket().shutdown(tcp::socket::shutdown_both, ec);

    // not_connected happens sometimes so don't bother reporting it.
    if (ec && ec != beast::errc::not_connected) THROW_SHUTDOWN_ERROR(ec.what(), context_);

    // if we get here then the connection is closed gracefully
  }


http::request<http::string_body> HTTPClient::build_request(
  const http::verb& method,
  const std::string& path,
  const std::string& body) {
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

http::request<http::string_body> HTTPClient::build_get_request(
  const std::string& path) {
  return build_request(http::verb::get, path);
}

http::request<http::string_body> HTTPClient::build_post_request(
  const std::string& path,
  const std::string& body) {
  return build_request(http::verb::post, path, body);
}

http::response<http::string_body> HTTPClient::execute(
  const http::request<http::string_body>& req,
  const std::chrono::milliseconds& timeout
) {
  boost::asio::io_context ioc;
  auto session = std::make_shared<Session>(
    ioc,
    req,
    timeout,
    RequestContext(req.method(), url(req.target()), timeout)
  );
  session->execute(port_);
  ioc.run();
  return session->res();
}

nlohmann::json HTTPClient::unmarshal_json_response(
  const http::response<http::string_body>& res,
  const RequestContext& req_context) {
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
