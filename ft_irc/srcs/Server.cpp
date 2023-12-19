#include "Server.hpp"

Server::Server(int ac, char **av) : serverName("saewoo") {
    if (ac != 3) {
        throw std::invalid_argument("Error invalid ac count");
    }

    portNumber = convertPort(av[1]);
    std::string password(av[2]);
    this->password = password;

    openServer();
    pushServerPollfd();
}