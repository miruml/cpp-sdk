#pragma once

// internal
#include <http/models/BaseConfigInstance.h>
#include <http/models/HashSchemaSerializedRequest.h>

#include <http/client.hpp>

namespace miru::http {

namespace openapi = org::openapitools::server::model;
namespace http = boost::beast::http;

class UnixSocketClient : public AgentClientI {
 public:
  UnixSocketClient(const std::string& socket_path = "/run/miru/miru.sock")
    : socket_path_(socket_path), base_path_("/v1"), host_("localhost"), port_("80") {}
  ~UnixSocketClient() {}

  std::pair<http::response<http::string_body>, RequestDetails> execute(
    const http::request<http::string_body>& req,
    const std::chrono::milliseconds& timeout
  ) const;

  const std::string& base_path() const { return base_path_; }
  const std::string& host() const { return host_; }
  const std::string& port() const { return port_; }

  nlohmann::json test_route();

  // route specific functions
  std::string hash_schema(const openapi::HashSchemaSerializedRequest& config_schema
  ) const;
  openapi::BaseConfigInstance get_deployed_config_instance(
    const std::string& config_schema_digest,
    const std::string& config_type_slug
  ) const;

 private:
  std::string socket_path_;
  std::string base_path_;
  std::string host_;
  std::string port_;
};

}  // namespace miru::http