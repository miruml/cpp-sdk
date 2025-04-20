// std
#include <iostream>

// internal
#include <miru/http/socket_client.hpp>

int main() {
    miru::client::UnixSocketClient client;

    std::cout << "Test Route ..." << std::endl;
    client.test_route();
    std::cout << "Success!\n" << std::endl;

    std::cout << "Hash Schema ..." << std::endl;
    std::string hash = client.hash_schema(
        nlohmann::json::parse(
            R"({"type": "object", "properties": {"name": {"type": "string"}}, "required": ["name"]})"
        )
    );
    std::cout << "Success!\nHashed Schema: " << hash << "\n" << std::endl;

    std::cout << "Get Concrete Config ..." << std::endl;
    auto config = client.get_latest_concrete_config(
        hash,
        "motion-control"
    );
    int indent = 2;
    std::cout << "Success!\nConcrete Config: \n" << config.to_json().dump(indent) << std::endl;
    return 0;
}