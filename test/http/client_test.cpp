// internal
#include <chrono>
#include <miru/config/config.hpp>
#include <miru/config/errors.hpp>
#include <miru/http/client.hpp>
#include <miru/query/query.hpp>
#include <miru/query/ros2.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace test::client {

namespace http = boost::beast::http;

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
    req.find(http::field::user_agent)->value(), "Miru Unix Client (BoostBeast)");
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
    req.find(http::field::user_agent)->value(), "Miru Unix Client (BoostBeast)");
}

// =============================== SEND REQUEST ==================================== //
TEST(HTTPClient, SendGetRequest) {
  miru::client::HTTPClient client("httpbin.org", "80");
  std::string path = "/get";
  http::request<http::string_body> req = client.build_get_request(path);

  http::response<http::string_body> res = client.execute(
    req, std::chrono::seconds(10)
  );
  EXPECT_EQ(res.result(), http::status::ok);
}

TEST(HTTPClient, SendPostRequest) {
  miru::client::HTTPClient client("httpbin.org", "80");
  std::string path = "/post";
  http::request<http::string_body> req =
    client.build_post_request(path, "{\"key\":\"value\"}");

  http::response<http::string_body> res = client.execute(
    req, std::chrono::seconds(10)
  );
  EXPECT_EQ(res.result(), http::status::ok);
}

TEST(HTTPClient, Timeout) {
  miru::client::HTTPClient client;
  std::string path = "/get";
  http::request<http::string_body> req = client.build_get_request(path);

  boost::asio::io_context ioc;
  auto session = std::make_shared<miru::client::Session>(
    ioc,
    req,
    std::chrono::seconds(10),
    miru::client::RequestContext(req.method(), client.url(path), std::chrono::seconds(10))
  );
  session->execute("443");
  ioc.run();
}

TEST(HTTPClient, EndpointNotFound) {
  miru::client::HTTPClient client("httpstat.us");
  std::string path = "/404";
  http::request<http::string_body> req = client.build_get_request(path);

  EXPECT_THROW(
    client.execute(req, std::chrono::seconds(10)),
    miru::http::errors::RequestFailedError);
}

}  // namespace test::client