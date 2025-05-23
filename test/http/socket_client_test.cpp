// internal
#include <http/models/HashSchemaSerializedRequest.h>

#include <http/socket_client.hpp>
#include <http/socket_session.hpp>
#include <http/utils.hpp>
#include <test/test_utils/testdata.hpp>

// external
#include <gtest/gtest.h>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace test::http {

namespace http = boost::beast::http;
namespace openapi = org::openapitools::server::model;

// ============================== BUILD GET REQUEST ================================ //
TEST(HTTPClient, BuildGetRequest) {
  http::request<http::string_body> req =
    miru::http::build_get_request("localhost", "/test");

  // request
  EXPECT_EQ(req.method(), http::verb::get);
  EXPECT_EQ(req.target(), "/test");
  EXPECT_EQ(req.version(), 11);
  EXPECT_EQ(req.body(), "");
  EXPECT_EQ(req.find(http::field::host)->value(), "localhost");
}

// ============================= BUILD POST REQUEST ================================ //
TEST(HTTPClient, BuildPostRequest) {
  http::request<http::string_body> req =
    miru::http::build_post_request("localhost", "/test", "{\"key\":\"value\"}");

  // request
  EXPECT_EQ(req.method(), http::verb::post);
  EXPECT_EQ(req.target(), "/test");
  EXPECT_EQ(req.version(), 11);
  EXPECT_EQ(req.body(), "{\"key\":\"value\"}");
  EXPECT_EQ(req.find(http::field::host)->value(), "localhost");
}

// =============================== SANDBOX TESTS =================================== //
TEST(HTTPClient, DISABLED_Sandbox) {
  // test route
  miru::http::UnixSocketClient client;
  auto res = client.test_route();

  // hash schema
  miru::filesys::File schema_file(
    miru::test_utils::config_schemas_testdata_dir().file("motion-control.json")
  );

  std::string schema = schema_file.read_string();
  openapi::HashSchemaSerializedRequest request;
  request.format.value = openapi::HashSerializedConfigSchemaFormat::
    eHashSerializedConfigSchemaFormat::HASH_SERIALIZED_CONFIG_SCHEMA_FORMAT_JSON;
  request.schema = schema;

  res = client.hash_schema(request);
  std::cout << res << std::endl;
}

}  // namespace test::http