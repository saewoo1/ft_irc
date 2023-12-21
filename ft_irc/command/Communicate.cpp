#include "Communicate.hpp"

void Communicate::sendWarnning(int clientFd, std::string warning)
{
    warning += "\r\n";
    const char *warningtoChar = warning.c_str();

    int result = send(clientFd, const_cast<char *>(warningtoChar), std::strlen(warningtoChar), 0);

    if (result == -1) {
        throw new std::runtime_error("Error: send failed");
    }
}