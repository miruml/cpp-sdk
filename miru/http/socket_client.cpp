// std
#include <unistd.h>

#include <cstring>

// internal
#include <miru/http/socket_client.hpp>
#include <miru/http/socket_session.hpp>
#include <miru/http/utils.hpp>

// external
#include <miru/http/models/BaseConcreteConfig.h>
#include <miru/http/models/HashSchemaRequest.h>
#include <miru/http/models/RefreshLatestConcreteConfigRequest.h>

#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

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
  const openapi::HashSchemaRequest& config_schema
) const {
  std::string path = base_path() + "/config_schemas/hash";
  auto req = build_post_request(host(), path, config_schema.to_json().dump());
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return handle_json_response(res.first, res.second)["digest"].get<std::string>();
}

openapi::BaseConcreteConfig UnixSocketClient::get_latest_concrete_config(
  const std::string& config_schema_digest,
  const std::string& config_slug
) const {
  std::string path = base_path() + "/concrete_configs/latest?config_schema_digest=" +
                     config_schema_digest + "&config_slug=" + config_slug;
  auto req = build_get_request(host(), path);
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return openapi::BaseConcreteConfig::from_json(
    handle_json_response(res.first, res.second)
  );
}

openapi::BaseConcreteConfig UnixSocketClient::refresh_latest_concrete_config(
  const openapi::RefreshLatestConcreteConfigRequest& request
) const {
  std::string path = base_path() + "/concrete_configs/refresh_latest";
  auto req = build_post_request(host(), path, request.to_json().dump());
  std::chrono::milliseconds timeout = std::chrono::seconds(10);
  auto res = execute(req, timeout);
  return openapi::BaseConcreteConfig::from_json(
    handle_json_response(res.first, res.second)
  );
}

}  // namespace miru::http