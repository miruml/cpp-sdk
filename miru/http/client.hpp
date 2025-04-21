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

class BackendClientI {
 public:
  virtual ~BackendClientI() = default;

  // route specific functions
  virtual std::string hash_schema(const openapi::HashSchemaRequest& config_schema) = 0;
  virtual openapi::BaseConcreteConfig get_latest_concrete_config(
    const std::string& config_schema_digest,
    const std::string& config_slug
  ) = 0;
  virtual openapi::BaseConcreteConfig refresh_latest_concrete_config(
    const openapi::RefreshLatestConcreteConfigRequest& request
  ) = 0;
};

}  // namespace miru::http