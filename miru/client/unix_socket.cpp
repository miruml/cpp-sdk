// std
#include <unistd.h>

#include <cstring>

// internal
#include <miru/client/http_client.hpp>
#include <miru/client/unix_socket.hpp>

// external
#include <miru/client/models/BaseConcreteConfig.h>
#include <miru/client/models/HashSchemaRequest.h>

#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::client {

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace openapi = org::openapitools::server::model;

void UnixSocketClient::send_sync_request(const http::request<http::string_body>& req,
                                         http::response<http::string_body>& res) {
  asio::io_context ioc;
  boost::asio::local::stream_protocol::socket socket(ioc);
  socket.connect(boost::asio::local::stream_protocol::endpoint(socket_path_));
  client_.send_sync_request(socket, req, res);
}

void UnixSocketClient::test_route() {
  http::request<http::string_body> req =
      client_.build_get_request(base_path() + "/test");
  http::response<http::string_body> res;
  send_sync_request(req, res);
}

std::string UnixSocketClient::hash_schema(
    const openapi::HashSchemaRequest& config_schema) {
  http::request<http::string_body> req = client_.build_post_request(
      base_path() + "/config_schemas/hash", config_schema.to_json());
  http::response<http::string_body> res;
  send_sync_request(req, res);
  return client_.parse_json_response(res)["digest"].get<std::string>();
}

openapi::BaseConcreteConfig UnixSocketClient::get_concrete_config(
    const std::string& config_schema_digest, const std::string& config_slug) {
  http::request<http::string_body> req = client_.build_get_request(
      base_path() + "/concrete_configs/latest?config_schema_digest=" +
      config_schema_digest + "&config_slug=" + config_slug);
  http::response<http::string_body> res;
  send_sync_request(req, res);
  return openapi::BaseConcreteConfig::from_json(client_.parse_json_response(res));
}

}  // namespace miru::client