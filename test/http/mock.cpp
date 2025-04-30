// internal
#include "test/http/mock.hpp"

#include <miru/http/details/models/BaseConcreteConfig.h>
#include <miru/http/details/models/HashSchemaSerializedRequest.h>
#include <miru/http/details/models/RefreshLatestConcreteConfigRequest.h>

#include <miru/http/details/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::http {

std::string MockBackendClient::hash_schema(
  const openapi::HashSchemaSerializedRequest& config_schema
) const {
  return hash_schema_func();
}

openapi::BaseConcreteConfig MockBackendClient::get_latest_concrete_config(
  const std::string& config_schema_digest,
  const std::string& config_slug
) const {
  return get_latest_concrete_config_func();
}

openapi::BaseConcreteConfig MockBackendClient::refresh_latest_concrete_config(
  const openapi::RefreshLatestConcreteConfigRequest& request
) const {
  return refresh_latest_concrete_config_func();
}

}  // namespace miru::http