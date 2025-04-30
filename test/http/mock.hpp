#pragma once

// internal
#include <miru/http/details/models/BaseConcreteConfig.h>
#include <miru/http/details/models/HashSchemaSerializedRequest.h>
#include <miru/http/details/models/RefreshLatestConcreteConfigRequest.h>

#include <miru/http/details/client.hpp>
#include <miru/http/details/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::http::details {

namespace openapi = org::openapitools::server::model;

class MockBackendClient : public BackendClientI {
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

}  // namespace miru::http::details