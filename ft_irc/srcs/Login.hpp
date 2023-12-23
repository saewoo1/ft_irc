#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "UserInfo.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

class Login {
	private:
		UserInfo &userInfo;
		std::map<int, UserInfo> &allUsers;

		std::string serverName;
		bool isEmpty();
		bool isDuplicateNick();
        bool isValidUserInfo();
		void generateLoginPage();
	public:
		Login(UserInfo &userInfo, std::map<int, UserInfo> &allUsers, std::string serverName);
};
#endif