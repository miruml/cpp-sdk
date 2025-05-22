// internal
#include "test/http/mock.hpp"

#include <http/models/BaseConfigInstance.h>
#include <http/models/HashSchemaSerializedRequest.h>
#include <http/models/RefreshLatestConfigInstanceRequest.h>

#include <http/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace test::http {

std::string MockBackendClient::hash_schema(
  const openapi::HashSchemaSerializedRequest& config_schema
) const {
  return hash_schema_func();
}

openapi::BaseConfigInstance MockBackendClient::get_latest_config_instance(
  const std::string& config_schema_digest,
  const std::string& config_type_slug
) const {
  return get_latest_config_instance_func();
}

openapi::BaseConfigInstance MockBackendClient::refresh_latest_config_instance(
  const openapi::RefreshLatestConfigInstanceRequest& request
) const {
  return refresh_latest_config_instance_func();
}

}  // namespace test::http