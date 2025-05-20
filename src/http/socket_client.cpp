// std
#include <unistd.h>

#include <cstring>

// internal
#include <http/models/BaseConfigInstance.h>
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/RefreshLatestConfigInstanceRequest.h>

#include <http/socket_client.hpp>
#include <http/socket_session.hpp>
#include <http/utils.hpp>

namespace miru::http {

namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace openapi = org::openapitools::server::model;

std::pair<http::response<http::string_body>, RequestDetails> UnixSocketClient::execute(
  const http::request<http::string_body>& req,
  const std::chrono::milliseconds& timeout
) const {
  asio::io_context ioc;
  auto session = std::make_shared<SocketSession>(ioc, socket_path_, req, timeout);
  RequestDetails details = session->details();
  session->execute();
  ioc.run();

  return std::make_pair(session->res(), details);
}

nlohmann::json UnixSocketClient::test_route() {
  std::string path = base_path() + "/test";
  auto req = build_get_request(host(), path);
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return handle_json_response(res.first, res.second);
}

std::string UnixSocketClient::hash_schema(
  const openapi::HashSchemaSerializedRequest& config_schema
) const {
  std::string path = base_path() + "/config_schemas/hash/serialized";
  auto req = build_post_request(host(), path, config_schema.to_json().dump());
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return handle_json_response(res.first, res.second)["digest"].get<std::string>();
}

openapi::BaseConfigInstance UnixSocketClient::get_latest_config_instance(
  const std::string& config_schema_digest,
  const std::string& config_slug
) const {
  std::string path = base_path() + "/config_instances/latest?config_schema_digest=" +
                     config_schema_digest + "&config_slug=" + config_slug;
  auto req = build_get_request(host(), path);
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return openapi::BaseConfigInstance::from_json(
    handle_json_response(res.first, res.second)
  );
}

openapi::BaseConfigInstance UnixSocketClient::refresh_latest_config_instance(
  const openapi::RefreshLatestConfigInstanceRequest& request
) const {
  std::string path = base_path() + "/config_instances/refresh_latest";
  auto req = build_post_request(host(), path, request.to_json().dump());
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return openapi::BaseConfigInstance::from_json(
    handle_json_response(res.first, res.second)
  );
}

}  // namespace miru::http