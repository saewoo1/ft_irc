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

std::string Communicate::generateWarnMessage(UserInfo user, std::string errCode, std::string cmd, std::string param, std::string warnMessage) {
    std::string result = ":" + user.getServerName() + " " + user.getNickName() + " :" + cmd + " :" + param + " :" + warnMessage;
    sendToClient(user.getFd(), result); 
}

/**
 *  NICK ajfj
    :10.31.4.5 433 ajfj :NICK :Nickname is already in use
 *  에러메세지 정형화 시키기 
*/