#pragma once

// std
#include <string>

// external
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

// ================================== CLIENT ======================================= //
namespace miru::http {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
using stream_protocol = net::local::stream_protocol;

struct RequestDetails {
  http::verb method;
  std::string socket_path;
  std::string url;
  std::chrono::milliseconds timeout;

  RequestDetails(
    http::verb method,
    const std::string& socket_path,
    const std::string& url,
    const std::chrono::milliseconds& timeout
  )
    : method(method), socket_path(socket_path), url(url), timeout(timeout) {}
};

std::string to_string(const RequestDetails& details);

class SocketSession : public std::enable_shared_from_this<SocketSession> {
  beast::basic_stream<
    stream_protocol,
    net::any_io_executor,
    beast::unlimited_rate_policy> stream_;
  beast::flat_buffer buffer_;
  std::string socket_path_;
  http::request<http::string_body> req_;
  http::response<http::string_body> res_;
  std::chrono::milliseconds timeout_;

 public:
  // Objects are constructed with a strand to
  // ensure that handlers do not execute concurrently.
  explicit SocketSession(
    net::io_context& ioc,
    const std::string& socket_path,
    const http::request<http::string_body>& req,
    const std::chrono::milliseconds& timeout)
    : stream_(net::make_strand(ioc)),
      socket_path_(socket_path),
      req_(req),
      timeout_(timeout) {}

  const http::response<http::string_body>& res() const { return res_; }
  RequestDetails details();

  void execute();
  void on_connect(beast::error_code ec);
  void on_write(beast::error_code ec, std::size_t bytes_transferred);
  void on_read(beast::error_code ec, std::size_t bytes_transferred);
};

}  // namespace miru::http