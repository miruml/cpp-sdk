#pragma once

// internal
#include <http/models/BaseConcreteConfig.h>
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/RefreshLatestConcreteConfigRequest.h>

#include <http/client.hpp>
#include <http/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace test::http {

namespace openapi = org::openapitools::server::model;

class MockBackendClient : public miru::http::BackendClientI {
 public:
  std::function<std::string()> hash_schema_func;
  std::function<openapi::BaseConcreteConfig()> get_latest_concrete_config_func;
  std::function<openapi::BaseConcreteConfig()> refresh_latest_concrete_config_func;

  // route specific functions
  std::string hash_schema(const openapi::HashSchemaSerializedRequest& config_schema
  ) const;
  openapi::BaseConcreteConfig get_latest_concrete_config(
    const std::string& config_schema_digest,
    const std::string& config_slug
  ) const;
  openapi::BaseConcreteConfig refresh_latest_concrete_config(
    const openapi::RefreshLatestConcreteConfigRequest& request
  ) const;
};

}  // namespace test::http