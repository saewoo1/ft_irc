#include "Login.hpp"

bool Login::isEmpty() {
	// USER -> userName(필수) hostname serverName :realName
	if (userInfo.getHostName().empty() || userInfo.getNickName().empty() 
	|| userInfo.getRealName().empty() || userInfo.getServerName().empty()) {
		return true;
	}
	return false;
}

// 닉네임 중복 검수 -> 이미 로그인 성공한 닉네임이 존재하는지?
bool Login::isDuplicateNick() {
	std::map<int, UserInfo>::iterator it;
	for (it = allUsers.begin(); it != allUsers.end(); it++) {
		if (it->second.getNickName() == userInfo.getNickName() && it->second.getActive()) {
			std::string errMsg = ":" + userInfo.getNickName() + " is Duplicate Name...";
			sendMsg(userInfo.getFd(), errMsg); // 해당 소켓에게 메세지 날리기
			return true;
		}
	}
	return false;
}

Login::Login(UserInfo &userInfo, std::map<int, UserInfo> &allUsers, std::string serverName) : userInfo(userInfo), allUsers(allUsers), serverName(serverName) {
	if (!isDuplicateNick() && !isEmpty()) {
		sendMsg(userInfo.getFd(), "접속성공");
	}
	throw std::runtime_error("접속 불가");
}

void sendMsg(int fd, std::string str)
{
    str += "\r\n";
	const char *reply = str.c_str();

	std::cout << str;

	int result = send(fd, const_cast<char *>(reply), strlen(reply), 0);

	if (result == -1)
		throw std::runtime_error("Error: send error");
}
