#pragma once

// internal
#include <http/models/BaseConfigInstance.h>
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/RefreshLatestConfigInstanceRequest.h>

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
  virtual openapi::BaseConfigInstance get_latest_config_instance(
    const std::string& config_schema_digest,
    const std::string& config_slug
  ) const = 0;
  virtual openapi::BaseConfigInstance refresh_latest_config_instance(
    const openapi::RefreshLatestConfigInstanceRequest& request
  ) const = 0;
};

}  // namespace miru::http