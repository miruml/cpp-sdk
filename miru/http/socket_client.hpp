#pragma once

// internal
#include <miru/http/models/BaseConcreteConfig.h>
#include <miru/http/models/HashSchemaRequest.h>
#include <miru/http/models/RefreshLatestConcreteConfigRequest.h>
#include <miru/http/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::http {

namespace openapi = org::openapitools::server::model;
namespace http = boost::beast::http;

class UnixSocketClient {
 public:
  UnixSocketClient(const std::string& socket_path = "/tmp/miru.sock")
    : socket_path_(socket_path), base_path_("/v1"), host_("localhost"), port_("80") {}
  ~UnixSocketClient() {}

  std::pair<http::response<http::string_body>, RequestDetails> execute(
    const http::request<http::string_body>& req,
    const std::chrono::milliseconds& timeout);

  const std::string& base_path() const { return base_path_; }
  const std::string& host() const { return host_; }
  const std::string& port() const { return port_; }

  nlohmann::json test_route();

  // route specific functions
  std::string hash_schema(const openapi::HashSchemaRequest& config_schema);
  openapi::BaseConcreteConfig get_latest_concrete_config(
    const std::string& config_schema_digest,
    const std::string& config_slug
  );
  openapi::BaseConcreteConfig refresh_latest_concrete_config(
    const openapi::RefreshLatestConcreteConfigRequest& request
  );

 private:
  std::string socket_path_;
  std::string base_path_;
  std::string host_;
  std::string port_;
};

}  // namespace miru::http