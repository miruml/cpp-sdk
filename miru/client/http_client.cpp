// std
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

// internal
#include <miru/client/http_client.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>

namespace miru::client {

namespace http = boost::beast::http;

http::request<http::string_body> HTTPClient::build_request(
    const http::verb& method,
    const std::string& path 
) {
    http::request<http::string_body> req;
    req.method(method);
    req.target(path);
    req.version(11);
    req.set(http::field::host, host_);
    req.set(http::field::user_agent, "Miru Unix Client (BoostBeast)");
    return req;
}



} // namespace miru::client
