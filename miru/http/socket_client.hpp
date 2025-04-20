#pragma once

// internal
#include <miru/http/models/BaseConcreteConfig.h>
#include <miru/http/models/HashSchemaRequest.h>
#include <miru/http/models/RefreshLatestConcreteConfigRequest.h>

#include <miru/http/client.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::client {

namespace openapi = org::openapitools::server::model;
namespace http = boost::beast::http;

class UnixSocketClient {
 public:
  UnixSocketClient(const std::string& socket_path = "/tmp/miru.sock")
    : client_("localhost"), socket_path_(socket_path) {}
  ~UnixSocketClient() {}

  void send(
    const http::request<http::string_body>& req, const RequestContext& error_context,
    http::response<http::string_body>& res
  );

  std::string base_path() const { return "/v1"; }

  nlohmann::json test_route();

  // route specific functions
  std::string hash_schema(const openapi::HashSchemaRequest& config_schema);
  openapi::BaseConcreteConfig get_latest_concrete_config(
    const std::string& config_schema_digest, const std::string& config_slug
  );
  openapi::BaseConcreteConfig refresh_latest_concrete_config(
    const openapi::RefreshLatestConcreteConfigRequest& request
  );

 private:
  HTTPClient client_;
  std::string socket_path_;
};

}  // namespace miru::client