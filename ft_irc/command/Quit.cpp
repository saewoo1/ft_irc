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
	// user의 정보를 false로 돌리고, 모든 정보를 지웁니다.
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