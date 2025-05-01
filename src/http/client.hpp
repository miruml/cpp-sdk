#pragma once

// internal
#include <http/models/BaseConcreteConfig.h>
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/RefreshLatestConcreteConfigRequest.h>

#include <http/socket_session.hpp>

namespace miru::http {

namespace openapi = org::openapitools::server::model;

class BackendClientI {
 public:
  virtual ~BackendClientI() = default;

  // route specific functions
  virtual std::string hash_schema(
    const openapi::HashSchemaSerializedRequest& config_schema
  ) const = 0;
  virtual openapi::BaseConcreteConfig get_latest_concrete_config(
    const std::string& config_schema_digest,
    const std::string& config_slug
  ) const = 0;
  virtual openapi::BaseConcreteConfig refresh_latest_concrete_config(
    const openapi::RefreshLatestConcreteConfigRequest& request
  ) const = 0;
};

}  // namespace miru::http