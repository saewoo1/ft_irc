#include "Communicate.hpp"

void Communicate::sendToClient(int clientFd, std::string warning)
{
    warning += "\r\n";
    const char *msg = warning.c_str();

    std::cout << warning;
    int result = send(clientFd, const_cast<char *>(msg), std::strlen(msg), 0);
    
    if (result == -1) {
        throw new std::runtime_error("Error: send failed");
    }
}

void Communicate::generateWarnMessage(UserInfo user, std::string errCode, std::string cmd, std::string param, std::string warnMessage) {
    std::string result = ":" + user.getHostName() + " " + errCode + " " + user.getNickName() + " :" + cmd + " :" + param + " :" + warnMessage;
    sendToClient(user.getFd(), result);
}

void Communicate::sendMessage(UserInfo user, std::string errCode, std::string cmd, std::string result) {
    std::string message = ":" + user.getHostName() + " " + errCode + " " + user.getNickName() + " :" + cmd + " :" + result;
    sendToClient(user.getFd(), message);
}
