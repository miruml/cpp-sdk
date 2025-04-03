#pragma once

// std
#include <string>

// external
#include <sys/socket.h>
#include <sys/un.h>

namespace miru::socket {

// Client class
class SocketClient {
public:
    SocketClient(const std::string& socket_path = "/var/run/miru.sock");
    ~SocketClient();

    // generic member functions
    void send_request();

    // route specific functions
    void get_schema_digest();
    void get_concrete_config();

private:
    std::string socket_path_;
};

} // namespace miru::socket