// std
#include <iostream>

// internal
#include <miru/client/unix_socket.hpp>

// external
#include <boost/asio.hpp>

int main() {
    miru::client::UnixSocketClient client;
    client.get_status();
    return 0;
}