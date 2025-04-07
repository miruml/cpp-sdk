#pragma once

// internal
#include <miru/client/http_client.hpp>
#include <miru/client/models/BaseConcreteConfig.h>
#include <miru/client/models/HashSchemaRequest.h>

// external
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::client {

namespace openapi = org::openapitools::server::model;
namespace http = boost::beast::http;

class UnixSocketClient {
public:
    UnixSocketClient(const std::string& socket_path = "/tmp/miru.sock")
        : client_("localhost"), socket_path_(socket_path) {}
    ~UnixSocketClient() {}

    void send_sync_request(
        const http::request<http::string_body>& req,
        http::response<http::string_body>& res
    );

    std::string base_path() const { return "/v1"; }

    // route specific functions
    void test_route(); 
    std::string hash_schema(const openapi::HashSchemaRequest& config_schema);
    openapi::BaseConcreteConfig get_concrete_config(
        const std::string& config_schema_digest,
        const std::string& config_slug
    );

private:
    HTTPClient client_;
    std::string socket_path_;
};

} // namespace miru::client