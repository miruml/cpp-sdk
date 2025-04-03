// std
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

// internal
#include <miru/client/http_client.hpp>
#include <miru/client/unix_socket.hpp>

// external
#include <boost/asio.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/beast.hpp>

namespace miru::client {

namespace http = boost::beast::http;

void UnixSocketClient::send_sync_request(
    const http::request<http::string_body>& req,
    http::response<http::string_body>& res
) {
    asio::io_context ioc;
    boost::asio::local::stream_protocol::socket socket(ioc);
    socket.connect(boost::asio::local::stream_protocol::endpoint(socket_path_));
    client_.send_sync_request(socket, req, res);
}

void UnixSocketClient::get_status() {
    http::request<http::string_body> req = client_.build_request(
        http::verb::get,
        base_path() + "/status"
    );
    http::response<http::string_body> res;
    send_sync_request(req, res);
    std::cout << res << std::endl;
}

} // namespace miru::client