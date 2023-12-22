#include "Communicate.hpp"

void Communicate::sendToClient(int clientFd, std::string warning)
{
    warning += "\r\n";
    const char *msg = warning.c_str();

    int result = send(clientFd, const_cast<char *>(msg), std::strlen(msg), 0);

    if (result == -1) {
        throw new std::runtime_error("Error: send failed");
    }
}