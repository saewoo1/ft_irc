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

void Communicate::generateWarnMessage(UserInfo user, std::string errCode, std::string cmd, std::string param, std::string warnMessage) {
    std::string result = ":" + user.getHostName() + " " + errCode + " " + user.getNickName() + " :" + cmd + " :" + param + " :" + warnMessage;
    sendToClient(user.getFd(), result);
}

/**
 * 실패 양식 - 알 수 없는 커맨드일 경우
 * :서버명 에러코드 닉네임 :커맨드 :에러메세지
 * 
 * 실패 양식 - 
*/
void Communicate::sendMessage(UserInfo user, std::string errCode, std::string cmd, std::string result) {
    std::string message = ":" + user.getHostName() + " " + errCode + " " + user.getNickName() + " :" + cmd + " :" + result;
    sendToClient(user.getFd(), message);
}


/**
 *  NICK ajfj
    :10.31.4.5 433 ajfj :NICK :Nickname is already in use
 *  에러메세지 정형화 시키기 
*/