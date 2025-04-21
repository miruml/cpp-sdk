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
namespace miru::client {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

struct RequestContext {
  http::verb method;
  std::string url;
  std::chrono::milliseconds timeout;

  RequestContext(http::verb method, std::string url, std::chrono::milliseconds timeout)
    : method(method), url(url), timeout(timeout) {}
};

std::string to_string(const RequestContext& context);

// Performs an HTTP GET and prints the response
class Session : public std::enable_shared_from_this<Session> {
  tcp::resolver resolver_;
  beast::tcp_stream stream_;
  beast::flat_buffer buffer_;  // (Must persist between reads)
  http::request<http::string_body> req_;
  http::response<http::string_body> res_;
  std::chrono::milliseconds timeout_;
  RequestContext context_;

 public:
  // Objects are constructed with a strand to
  // ensure that handlers do not execute concurrently.
  explicit Session(
    net::io_context& ioc,
    const http::request<http::string_body>& req,
    const std::chrono::milliseconds& timeout,
    const RequestContext& context
  )
    : resolver_(net::make_strand(ioc)),
      stream_(net::make_strand(ioc)),
      req_(req),
      timeout_(timeout),
      context_(context) {}

  const http::response<http::string_body>& res() const { return res_; }

  void execute(std::string port);
  void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
  void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
  void on_write(beast::error_code ec, std::size_t bytes_transferred);
  void on_read(beast::error_code ec, std::size_t bytes_transferred);
};


class HTTPClient {
 public:
  HTTPClient(
    const std::string& host = "localhost",
    const std::string& port = "443"
  )
    : host_(host), port_(port) {}
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

  http::response<http::string_body> execute(
    const http::request<http::string_body>& req,
    const std::chrono::milliseconds& timeout
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
  std::string port_;

  // friends
  friend class UnixSocketClient;
};

template <typename Stream>
void HTTPClient::send(
  Stream& stream, const http::request<http::string_body>& req,
  const RequestContext& error_context, http::response<http::string_body>& res
) {
  // check the response status
  http::write(stream, req);
  boost::beast::flat_buffer buffer;
  http::read(stream, buffer, res);

  // check the response status
  if (res.result() != http::status::ok) {
    THROW_REQUEST_FAILED(res.result_int(), error_context);
  }
}

}  // namespace miru::client
