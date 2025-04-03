// std
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

// internal
#include <miru/unix_socket_client.hpp>

namespace miru::socket {

SocketClient::SocketClient(const std::string& socket_path)
    : socket_path_(socket_path) {}

} // namespace miru::socket
