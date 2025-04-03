#pragma once

// std
#include <string>

// external
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>

namespace miru::client {

namespace asio = boost::asio;
namespace http = boost::beast::http;

class HTTPClient {
public:
    HTTPClient(const std::string& host = "localhost") : host_(host) {}
    ~HTTPClient() {}

    http::request<http::string_body> build_request(
        const http::verb& method,
        const std::string& path,
        const std::string& body = ""
    );

    http::request<http::string_body> build_get_request(const std::string& path);
    http::request<http::string_body> build_post_request(
        const std::string& path, const std::string& body
    );

    nlohmann::json parse_json_response(const http::response<http::string_body>& res);

    template <typename Stream>
    void send_sync_request(
        Stream& stream,
        const http::request<http::string_body>& req,
        http::response<http::string_body>& res
    );

private:
    std::string host_;
};

template <typename Stream>
void HTTPClient::send_sync_request(
    Stream& stream,
    const http::request<http::string_body>& req,
    http::response<http::string_body>& res
) {
    http::write(stream, req);
    boost::beast::flat_buffer buffer;
    http::read(stream, buffer, res);

    // check the response status
    if (res.result() != http::status::ok) {
        throw std::runtime_error(
            "HTTP request failed with status: " + std::to_string(res.result_int())
        );
    }
}

} // namespace miru::socket