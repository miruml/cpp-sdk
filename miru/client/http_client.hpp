#pragma once

// std
#include <string>

// external
#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace miru::client {

namespace asio = boost::asio;
namespace http = boost::beast::http;

class HTTPClient {
public:
    HTTPClient(const std::string& host = "localhost") : host_(host) {}
    ~HTTPClient() {}

    http::request<http::string_body> build_request(
        const http::verb& method,
        const std::string& path
    );

    template <typename Stream>
    void send_sync_request(
        Stream& stream,
        const http::request<http::string_body>& req,
        http::response<http::string_body>& res
    );

    template <typename Stream>
    void send_async_request(
        Stream& stream,
        const http::request<http::string_body>& req,
        std::function<void(boost::system::error_code, http::response<http::string_body>)> callback
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
}

} // namespace miru::socket