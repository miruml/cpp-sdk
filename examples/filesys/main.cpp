// std
#include <iostream>

// internal
#include <miru/filesys/file.hpp>

int main() {
    std::cout << "Hello, World!" << std::endl;

    miru::filesys::File file("./deleteme.txt");

    std::cout << "reading json" << std::endl;
    std::cout << file.read_json() << std::endl;

    return 0;
}