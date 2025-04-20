#pragma once

// std
#include <string>

// internal
#include <miru/http/errors.hpp>

// external
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

// ================================== CLIENT ======================================= //
namespace miru::client {

namespace beast = boost::beast;
namespace http = beast::http;

struct RequestContext {
  const boost::beast::http::verb method;
  const std::string url;
  const std::chrono::milliseconds timeout;

  RequestContext(
    const boost::beast::http::verb method, const std::string url,
    const std::chrono::milliseconds timeout
  )
    : method(method), url(url), timeout(timeout) {}
};

std::string to_string(const RequestContext& context);

class HTTPClient {
 public:
  HTTPClient(const std::string& host = "localhost") : host_(host) {}
  ~HTTPClient() {}

  http::request<http::string_body> build_get_request(const std::string& path);

  http::request<http::string_body> build_post_request(
    const std::string& path, const std::string& body
  );

  std::string url(const std::string& path) const {
    std::stringstream ss;
    if (host_ == "localhost") {
      ss << "http://" << host_ << path;
    } else {
      ss << "https://" << host_ << path;
    }
    return ss.str();
  }

  template <typename Stream>
  void send(
    Stream& stream, const http::request<http::string_body>& req,
    const RequestContext& context, http::response<http::string_body>& res
  );

  // A minimal async send+read with a single timeout, for tcp_stream only.
  template <class Handler>
  void async_http_with_timeout(
    beast::tcp_stream& stream, http::request<http::string_body> req,
    http::response<http::string_body>& res, std::chrono::steady_clock::duration timeout,
    Handler&& handler
  );

 protected:
  http::request<http::string_body> build_request(
    const http::verb& method, const std::string& path, const std::string& body = ""
  );

  nlohmann::json unmarshal_json_response(
    const http::response<http::string_body>& res, const RequestContext& context
  );

 private:
  std::string host_;

  // friends
  friend class UnixSocketClient;
};

template <typename Stream>
void HTTPClient::send(
  Stream& stream, const http::request<http::string_body>& req,
  const RequestContext& error_context, http::response<http::string_body>& res
) {
  // check the response status
  if (res.result() != http::status::ok) {
    THROW_REQUEST_FAILED(res.result_int(), error_context);
  }
}

// A minimal async send+read with a single timeout, for tcp_stream only.
template <class Handler>
void HTTPClient::async_http_with_timeout(
  beast::tcp_stream& stream, http::request<http::string_body> req,
  http::response<http::string_body>& res, std::chrono::steady_clock::duration timeout,
  Handler&& handler
) {
  // 1) set the overall deadline
  stream.expires_after(timeout);

  // 2) allocate a shared buffer for the read
  auto buffer = std::make_shared<beast::flat_buffer>();

  // 3) async write
  http::async_write(
    stream, std::move(req),
    [&, buffer,
     handler = std::forward<Handler>(handler)](beast::error_code ec, std::size_t) {
      if (ec) return handler(ec);

      // 4) async read
      http::async_read(
        stream, *buffer, res,
        [&, buffer, handler = std::move(handler)](beast::error_code ec2, std::size_t) {
          // cancel the deadline so future ops aren’t affected
          stream.expires_never();

          if (ec2) return handler(ec2);

          if (res.result() != http::status::ok)
            return handler(
              make_error_code(beast::http::error::bad_status),
              "HTTP status " + std::to_string(res.result_int())
            );

          // success!
          handler({}, "");
        }
      );
    }
  );
}

}  // namespace miru::client
