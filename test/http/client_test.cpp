// internal
#include <miru/config/config.hpp>
#include <miru/config/errors.hpp>
#include <miru/http/client.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

namespace test::client {

namespace http = boost::beast::http;

using https_stream = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

https_stream prepare_https_stream(const http::request<http::string_body>& req) {
  boost::asio::io_context ioc;
  boost::asio::ssl::context ctx(boost::asio::ssl::context::tlsv12_client);

  // Create endpoint
  boost::asio::ip::tcp::resolver resolver(ioc);
  auto const results = resolver.resolve(req.find(http::field::host)->value(), "443");

  // Create and connect stream
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream(ioc, ctx);
  boost::asio::connect(stream.next_layer(), results.begin(), results.end());

  // Perform SSL handshake
  stream.handshake(boost::asio::ssl::stream_base::client);
  return stream;
}

boost::asio::ip::tcp::socket prepare_http_stream(
  const http::request<http::string_body>& req
) {
  boost::asio::io_context ioc;

  // Create endpoint
  boost::asio::ip::tcp::resolver resolver(ioc);
  auto const results = resolver.resolve(
    req.find(http::field::host)->value(),
    "80"  // Standard HTTP port instead of 443
  );

  // Create and connect socket
  boost::asio::ip::tcp::socket socket(ioc);
  boost::asio::connect(socket, results.begin(), results.end());

  return socket;
}

// ============================== BUILD GET REQUEST ================================ //
TEST(HTTPClient, BuildGetRequest) {
  miru::client::HTTPClient client;
  http::request<http::string_body> req = client.build_get_request("/test");

  // request
  EXPECT_EQ(req.method(), http::verb::get);
  EXPECT_EQ(req.target(), "/test");
  EXPECT_EQ(req.version(), 11);
  EXPECT_EQ(req.body(), "");
  EXPECT_EQ(req.find(http::field::host)->value(), "localhost");
  EXPECT_EQ(
    req.find(http::field::user_agent)->value(), "Miru Unix Client (BoostBeast)"
  );
}

// ============================= BUILD POST REQUEST ================================ //
TEST(HTTPClient, BuildPostRequest) {
  miru::client::HTTPClient client;
  http::request<http::string_body> req =
    client.build_post_request("/test", "{\"key\":\"value\"}");

  // request
  EXPECT_EQ(req.method(), http::verb::post);
  EXPECT_EQ(req.target(), "/test");
  EXPECT_EQ(req.version(), 11);
  EXPECT_EQ(req.body(), "{\"key\":\"value\"}");
  EXPECT_EQ(req.find(http::field::host)->value(), "localhost");
  EXPECT_EQ(
    req.find(http::field::user_agent)->value(), "Miru Unix Client (BoostBeast)"
  );
}

// =============================== SEND REQUEST ==================================== //
TEST(HTTPClient, SendGetRequest) {
  miru::client::HTTPClient client("example.com");
  std::string path = "/";
  http::request<http::string_body> req = client.build_get_request(path);
  http::response<http::string_body> res;

  https_stream stream = prepare_https_stream(req);
  miru::client::RequestContext error_context(
    req.method(), client.url(path), std::chrono::seconds(10)
  );
  client.send(stream, req, error_context, res);
  EXPECT_EQ(res.result(), http::status::ok);
}

TEST(HTTPClient, SendPostRequest) {
  miru::client::HTTPClient client("httpbin.org");
  std::string path = "/post";
  http::request<http::string_body> req =
    client.build_post_request(path, "{\"key\":\"value\"}");
  http::response<http::string_body> res;

  https_stream stream = prepare_https_stream(req);
  miru::client::RequestContext error_context(
    req.method(), client.url(path), std::chrono::seconds(10)
  );
  client.send(stream, req, error_context, res);
  EXPECT_EQ(res.result(), http::status::ok);
}

TEST(HTTPClient, Timeout) {
  miru::client::HTTPClient client("httpstat.us");
  std::string path = "/200?sleep=1000";
  http::request<http::string_body> req = client.build_get_request(path);
  http::response<http::string_body> res;

  https_stream stream = prepare_https_stream(req);
  miru::client::RequestContext error_context(
    req.method(), client.url(path), std::chrono::milliseconds(100)
  );

  EXPECT_THROW(
    client.async_http_with_timeout(
      stream, req, res, std::chrono::seconds(10),
      [](auto ec, auto) { EXPECT_EQ(ec, boost::asio::error::timed_out); }
    ),
    miru::http::errors::RequestFailedError
  );
}

TEST(HTTPClient, EndpointNotFound) {
  miru::client::HTTPClient client("httpstat.us");
  std::string path = "/404";
  http::request<http::string_body> req = client.build_get_request(path);
  http::response<http::string_body> res;

  https_stream stream = prepare_https_stream(req);
  miru::client::RequestContext error_context(
    req.method(), client.url(path), std::chrono::seconds(10)
  );

  EXPECT_THROW(
    client.send(stream, req, error_context, res), miru::http::errors::RequestFailedError
  );
}

}  // namespace test::client