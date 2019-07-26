

#include <iostream>
#include <cstring>
#include "HttpClient.h"

int main(int argc, char **argv) {
    std::cout << "Hello, World!" << std::endl;
    try {
        auto const host = "reqres.in";
        auto const port = 443;
        auto const target = "/api/users";

        HttpClient client(host, port);
        client.request(target, {{"name", "morpheus"},
                                {"job",  "leader"}});
    }
    catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}