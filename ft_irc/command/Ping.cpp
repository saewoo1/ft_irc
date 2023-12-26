#include "Ping.hpp"

Ping::Ping(Message *msg, UserInfo &user) : Command(msg), user(user) {
}

/**
 * PING -> 서버에서 클라이언트로, 클라이언트에서 서버로 두가지 케이스
 * 1. 서버에서 입력 -> ??? 얘 필수 아닌거같은디;
*/
void Ping::execute() {
	std::string trailing = "";

	std::istringstream iss(this->getOriginalMsg());
	std::string command, rest;
	std::getline(iss, command, ':');
	std::getline(iss, rest);

    if (rest != "")
        trailing = rest;
    std::string result = ":" + user.getHostName() + " PONG " + trailing;
    Communicate::sendToClient(user.getFd(), result);
}
