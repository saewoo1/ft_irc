#include "Login.hpp"
#include "../command/Communicate.hpp"

/**
 * 1. USER를 입력한다
 *  USER username hostName serverName :realName 세팅
 * 2. NICK 입력
 *  기존의 nickName들과 중복 검수
 *  NICK userNickName 세팅
 * @return
 */
bool Login::isValidUserInfo() {
	// USER -> userName(필수) : trailing
    // NICK -> nickname
    std::cout << "nickName : " << userInfo.getNickName() << std::endl;
    std::cout << "userName : " << userInfo.getUserName() << std::endl;
	if (userInfo.getNickName().empty() || userInfo.getUserName().empty()) {
		return false;
	}
	return true;
}

Login::Login(UserInfo &userInfo, std::map<int, UserInfo> &allUsers, std::string serverName) : userInfo(userInfo), allUsers(allUsers), serverName(serverName) {
	// 단순 닉네임 변경사항이라면, 접속 성공 띄우지 마셈
	if (isValidUserInfo() && userInfo.getNick() == false) {
        userInfo.setActive(true); // 유저 등록
		Communicate::sendToClient(userInfo.getFd(), "접속성공");
        return;
	}
}
