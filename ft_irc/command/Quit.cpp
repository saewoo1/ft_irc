#include "Quit.hpp"

Quit::Quit(Message *msg, UserInfo &user, std::map<std::string, Channel> &allChannel, std::map<int, UserInfo> &allUsers, std::vector<pollfd> &pollfds) 
				: Command(msg), user(user), allChannel(allChannel), allUsers(allUsers), pollfds(pollfds) {
}

void Quit::execute() {
	// 일단 호출자에게 Error ~ 메세지 출력
	std::string result = "ERROR :Closing Link:" + user.getHostName(); + " (Quit:" + getTrailing() + ")";
	Communicate::sendToClient(user.getFd(), result);

	// 채널 내의 모든 유저들에게 떠난다는 메세지 보내기
	std::map<std::string, bool>::iterator it = user.channels.begin();

	for (; it != user.channels.end(); it++) {
		std::string channelName = it->first;

		std::map<std::string, Channel>::iterator channelIt = allChannel.find(channelName);
		if (channelIt != allChannel.end()) {
			Channel &channel = channelIt->second;

			channel.users.erase(user.getNickName());
			channel.operators.erase(user.getNickName());
			channel.invite.erase(user.getNickName());

			std::map<std::string, UserInfo>::iterator userIt = channel.users.begin();
			for (; userIt != channel.users.end(); userIt++) {
				UserInfo &userInChat = userIt->second;

				if (userInChat.getFd() == user.getFd()) {
					continue;
				}
				std::string bye = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getHostName() + " PART " + channelName + " :" + getTrailing();
				Communicate::sendToClient(userInChat.getFd(), bye);
			}
		}
	}
	user.setActive(false);
	allUsers.erase(user.getFd());
	close(user.getFd());
	for (std::vector<pollfd>::iterator it = pollfds.begin(); it != pollfds.end(); ++it) {
		if (it->fd == user.getFd()) {
			pollfds.erase(it);
			break;
		}
	}
}

// QUIT 명령어 입력 시 속한 채널의 이용자에게 메세지를 보내고, 접속 종료하는 기능.
// server.quitServer와 같은 기능인가..응슷응
// void Quit::execute() {
// 	std::string cause = "";

// 	std::istringstream iss(getOriginalMsg());
// 	std::string cmd, rest;
// 	std::getline(iss, cmd, ':');
// 	std::getline(iss, rest);

// 	if (!rest.empty()) {
// 		cause = rest;
// 	} else {
// 		cause = "leaving";
// 	}

// 	std::string msg = "Error : Closing link: (" + user.getUserName() + "@" + user.getServerName() + ") [Quit:" + cause + "]";
// 	Communicate::sendToClient(user.getFd(), msg);

// 	// 유저가 속한 모든 채널을 순회
// 	for (std::map<std::string, bool>::iterator it = user.channels.begin(); it != user.channels.end(); it++) {
// 		std::string channelName = it->first;
// 		// 전체 채널 중 유저가 속한 채널을 하나 따온다
// 		std::map<std::string, Channel>::iterator chanIt = allChannel.find(channelName);

// 		Channel &channel = chanIt->second;
// 		// 유저가 속한 채널에서 유저를 지운다. 
// 		channel.users.erase(user.getNickName());
// 		channel.operators.erase(user.getNickName());
// 		channel.invite.erase(user.getNickName());
// 		for (std::map<std::string, UserInfo>::iterator userIt = channel.users.begin(); userIt != channel.users.end(); userIt++) {
// 			UserInfo &user = userIt->second;
// 			if (this->user.getFd() == user.getFd()) {
// 				continue;
// 			}
// 			std::string channelMsg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getServerName() + " QUIT :Quit: " + cause;
// 			Communicate::sendToClient(user.getFd(), channelMsg);
// 		}
// 	}
// 	user.setActive(false);
// }


/**
 * 
 * QUIT
 * 클라이언트가 서버 및 다른 사용자에게 연결을 끊고 있음을 알린다.
 * 해제 이유는 선택사항
 * QUIT :이유
 * 
 * 클라이언트가 속한 모든 채널에 종료 메세지를 보내고, 연결을 끊는다.
*/