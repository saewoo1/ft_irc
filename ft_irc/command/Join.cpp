#include "Join.hpp"

Join::Join(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList) : Command(msg), user(user), channelList(channelList)
{
}

int Join::validateJoinExecute(const std::string &channelName, const std::string &password)
{
	// (void)password;
    if (this->user.channels.size() >= 10) {
        std::string waring = ":" + this->user.getHostName() + "405" + this->user.getNickName() + " " + channelName + \
                                " : You have so many Channels! you're so mean";
        Communicate::sendToClient(this->user.getFd(), waring);
        return 1;
    }
    std::map<std::string, Channel>::iterator it = channelList.find(channelName);

    if (it == channelList.end()) {
        if (channelName[0] != '#') {
			std::string msg = ":" + this->user.getServerName() + " 403 " + this->user.getNickName() + " :JOIN :" + channelName + \
							" No such channel\n";
			Communicate::sendToClient(this->user.getFd(), msg);
            return 1;
        }
		// std::cout << "없는 채널이니?" << std::endl;
        createNewChannel(channelName);
    } else {
		// std::cout << "있는 채널이니?" << std::endl;
        joinChannel(channelName, password);
    }
	return 0;
}

void Join::createNewChannel(std::string channelName)
{
	// 새로운 채널을 만듦. userInfo, 채널명
    Channel newChannel(this->user, channelName);
	// std::cout << "채널명?" << channelName << std::endl;
	// Server측 채널 리스트에도 해당 사항을 업데이트 한다.
    channelList.insert(std::make_pair(channelName, newChannel));
	// for (std::map<std::string, Channel>::iterator it = channelList.begin(); it != channelList.end(); it++) {
	// 	std::cout << "업데이트 되었낭?" << it->second.getName() << std::endl;
	// }

    newChannel.operators.insert(std::make_pair(this->user.getNickName(), this->user));
    newChannel.users.insert(std::make_pair(this->user.getNickName(), this->user));

    this->user.channels.insert(std::make_pair(newChannel.getName(), true));
    std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + \
                    " " + "JOIN :" + newChannel.getName() + "\n";
    msg += ":" + this->user.getServerName() + " 353 " + this->user.getNickName() + " = " + newChannel.getName() + \
				" :@" + this->user.getNickName() + "\n";
	msg += ":" + this->user.getServerName() + " 366 " + this->user.getNickName() + " " + newChannel.getName() + \
				" :End of NAMES list\n";
	
	Communicate::sendToClient(this->user.getFd(), msg);
}

void Join::joinChannel(std::string channelName, const std::string &password)
{
	std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);
	this->existed = &it->second;

	std::map<std::string, bool>::iterator userIter = this->user.channels.find(channelName);
	if (userIter != this->user.channels.end()) {
		std::string msg = ":" + this->user.getServerName() + " 443 " + this->user.getNickName() + " " + channelName + " :is already on channel\n";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}

	if (checkJoinConditions(password)) {
		this->existed->users.insert(std::make_pair(this->user.getNickName(), this->user));
		this->user.channels.insert(std::make_pair(this->existed->getName(), false));

		this->existed->setUserCount(1);
		if (this->existed->getInviteMode()) {
			this->existed->invite.erase(this->user.getNickName());
		}

		std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + \
						+ " Join :" + this->existed->getName() + "\n";
		msg += "353 " + this->user.getNickName() + "=" + this->existed->getName() + ":";
		for (std::map<std::string, UserInfo>::iterator it = this->existed->users.begin(); it != this->existed->users.end(); it++) {
			UserInfo userInfo = it->second;
			std::map<std::string, UserInfo>::iterator op = this->existed->operators.find(this->user.getNickName());
			if (op == this->existed->operators.end()) {
				msg += " " + userInfo.getNickName();
			} else {
				msg += " @" + userInfo.getNickName();
			}
		}
		msg += "\n";
		msg += "366" + this->user.getNickName() + " " + this->existed->getName() + " :End of NAMES list\n";

		Communicate::sendToClient(this->user.getFd(), msg);
		for (std::map<std::string, UserInfo>::iterator it = this->existed->users.begin(); it != this->existed->users.end(); it++) {
			UserInfo userInfo = it->second;

			if (userInfo.getFd() == this->user.getFd()) {
				continue;
			}
			std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + \
							" Join :" + this->existed->getName() + "\n";
			Communicate::sendToClient(userInfo.getFd(), msg);
		}
	}
}

bool Join::checkJoinConditions(const std::string &password)
{
	if (this->existed->getLimitMode() && \
		this->existed->getLimit() <= static_cast<long long>(this->existed->users.size())) {
			std::string msg = ":" + this->user.getHostName() + " 471" + this->user.getNickName() + " " + this->existed->getName() + \
						" :Cannot join channel (+l)";
		Communicate::sendToClient(this->user.getFd(), msg);		
    	return false;
	}

	if ((this->existed->getKeyMode() && password.empty()) || (this->existed->getKeyMode() && password != this->existed->getKey())) {
		std::string msg = ":" + this->user.getHostName() + " 475" + this->user.getNickName() + " " + this->existed->getName() + \
						" :Cannot join channel (+k)";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}

	if (this->existed->getInviteMode() && this->existed->invite.find(this->user.getNickName()) == this->existed->invite.end()) {
		std::string msg = ":" + this->user.getHostName() + " 473" + this->user.getNickName() + " " + this->existed->getName() + \
						":Cannot join channel (+i)";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
	return true;

}

void Join::execute() {
	if (!this->user.getActive()) {
		return ;
	}
	std::string channelName;
	std::string password;

	if (this->getParameters().size() < 1) {
		std::string msg = ":" + this->user.getNickName() + " 461 JOIN :Not enough parameters\n";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}
	std::cout << this->getParameters().at(0) << std::endl;

	channelName = this->getParameters().at(0);
	if (this->getParameters().size() == 2) {
		password = this->getParameters().at(1);
	} else {
		password = "";
	}

	validateJoinExecute(channelName, password);
}

