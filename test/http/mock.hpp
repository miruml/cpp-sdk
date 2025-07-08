#pragma once

// internal
#include <http/models/BaseConfigInstance.h>
#include <http/models/HashSchemaSerializedRequest.h>

#include <http/client.hpp>
#include <http/socket_session.hpp>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace test::http {

namespace openapi = org::openapitools::server::model;

class MockAgentClient : public miru::http::AgentClientI {
 public:
  std::function<std::string()> hash_schema_func;
  std::function<openapi::BaseConfigInstance()> get_deployed_config_instance_func;

  // route specific functions
  std::string hash_schema(const openapi::HashSchemaSerializedRequest& config_schema
  ) const;
  openapi::BaseConfigInstance get_deployed_config_instance(
    const std::string& config_schema_digest,
    const std::string& config_type_slug
  ) const;
};

}  // namespace test::http