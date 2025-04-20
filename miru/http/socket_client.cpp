// std
#include <unistd.h>

#include <cstring>

// internal
#include <miru/http/client.hpp>
#include <miru/http/socket_client.hpp>

// external
#include <miru/http/models/BaseConcreteConfig.h>
#include <miru/http/models/HashSchemaRequest.h>
#include <miru/http/models/RefreshLatestConcreteConfigRequest.h>

#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::client {

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace openapi = org::openapitools::server::model;

void UnixSocketClient::send(
  const http::request<http::string_body>& req, const RequestContext& error_context,
  http::response<http::string_body>& res
) {
  asio::io_context ioc;
  boost::asio::local::stream_protocol::socket socket(ioc);
  socket.connect(boost::asio::local::stream_protocol::endpoint(socket_path_));
  client_.send(socket, req, error_context, res);
}

nlohmann::json UnixSocketClient::test_route() {
  std::string path = base_path() + "/test";
  http::request<http::string_body> req = client_.build_get_request(path);
  http::response<http::string_body> res;
  RequestContext error_context(
    req.method(), client_.url(path), std::chrono::seconds(10)
  );
  send(req, error_context, res);
  return client_.unmarshal_json_response(res, error_context);
}

std::string UnixSocketClient::hash_schema(
  const openapi::HashSchemaRequest& config_schema
) {
  http::request<http::string_body> req = client_.build_post_request(
    base_path() + "/config_schemas/hash", config_schema.to_json()
  );
  std::string path = base_path() + "/config_schemas/hash";
  http::response<http::string_body> res;
  RequestContext error_context(
    req.method(), client_.url(path), std::chrono::seconds(10)
  );
  send(req, error_context, res);
  return client_.unmarshal_json_response(res, error_context)["digest"].get<std::string>(
  );
}

openapi::BaseConcreteConfig UnixSocketClient::get_latest_concrete_config(
  const std::string& config_schema_digest, const std::string& config_slug
) {
  std::string path = base_path() + "/concrete_configs/latest?config_schema_digest=" +
                     config_schema_digest + "&config_slug=" + config_slug;
  http::request<http::string_body> req = client_.build_get_request(path);
  RequestContext error_context(
    req.method(), client_.url(path), std::chrono::seconds(10)
  );
  http::response<http::string_body> res;
  send(req, error_context, res);
  return openapi::BaseConcreteConfig::from_json(
    client_.unmarshal_json_response(res, error_context)
  );
}

openapi::BaseConcreteConfig UnixSocketClient::refresh_latest_concrete_config(
  const openapi::RefreshLatestConcreteConfigRequest& request
) {
  std::string path = base_path() + "/concrete_configs/refresh_latest";
  http::request<http::string_body> req =
    client_.build_post_request(path, request.to_json());
  RequestContext error_context(
    req.method(), client_.url(path), std::chrono::seconds(10)
  );
  http::response<http::string_body> res;
  send(req, error_context, res);
  return openapi::BaseConcreteConfig::from_json(
    client_.unmarshal_json_response(res, error_context)
  );
}

}  // namespace miru::client