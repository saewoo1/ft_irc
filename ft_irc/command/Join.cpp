#include "Join.hpp"

Join::Join(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList) : Command(msg), user(user), channelList(channelList)
{
}

int Join::validateJoinExecute(const std::string &channelName, const std::string &password)
{
    if (this->user.channels.size() >= 10) {
        std::string waring = ":" + this->user.getHostName() + "405" + this->user.getNickName() + " " + channelName + \
                                " :You have joined too many channels";
        Communicate::sendToClient(this->user.getFd(), waring);
        return 1;
    }
    std::map<std::string, Channel>::iterator it = channelList.find(channelName);

    if (it == channelList.end()) {
        if (channelName[0] != '#') {
			std::string msg = ":" + this->user.getServerName() + " 403 " + this->user.getNickName() + " :JOIN :" + channelName + \
							" No such channel";
			Communicate::sendToClient(this->user.getFd(), msg);
            return 1;
        }
        createNewChannel(channelName);
    } else {
        joinChannel(channelName, password);
    }
	return 0;
}

void Join::createNewChannel(std::string channelName)
{
    Channel newChannel(this->user, channelName);
    channelList.insert(std::make_pair(channelName, newChannel));

    newChannel.operators.insert(std::make_pair(this->user.getNickName(), this->user));
    newChannel.users.insert(std::make_pair(this->user.getNickName(), this->user));

    this->user.channels.insert(std::make_pair(newChannel.getName(), true));
    std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + \
                    " " + "JOIN :" + newChannel.getName() + "\n";
    msg += ":" + this->user.getServerName() + " 353 " + this->user.getNickName() + " = " + newChannel.getName() + \
				" :@" + this->user.getNickName() + "\n";
	msg += ":" + this->user.getServerName() + " 366 " + this->user.getNickName() + " " + newChannel.getName() + \
				" :End of NAMES list";
	
	Communicate::sendToClient(this->user.getFd(), msg);
}

void Join::joinChannel(std::string channelName, const std::string &password)
{
	std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);
	// 채널 리스트에서 채널을 찾아온 뒤, topic이 존재한다면 같이 set 넣어줘야함
	this->existed = &it->second;
	if (!it->second.getTopic().empty()) {
		this->existed->setTopic(it->second.getTopic());
	}

	std::map<std::string, bool>::iterator userIter = this->user.channels.find(channelName);
	if (userIter != this->user.channels.end()) {
		std::string msg = ":" + this->user.getServerName() + " 443 " + this->user.getNickName() + " " + channelName + " :is already on channel";
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
		msg += "353 " + this->user.getNickName() + " = " + this->existed->getName() + " :";
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
		msg += "366 " + this->user.getNickName() + " " + this->existed->getName() + " :End of NAMES list";

		Communicate::sendToClient(this->user.getFd(), msg);
		for (std::map<std::string, UserInfo>::iterator it = this->existed->users.begin(); it != this->existed->users.end(); it++) {
			UserInfo userInfo = it->second;

			if (userInfo.getFd() == this->user.getFd()) {
				continue;
			}
			std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + \
							" Join :" + this->existed->getName();
			Communicate::sendToClient(userInfo.getFd(), msg);
		}
	}
}

bool Join::checkJoinConditions(const std::string &password)
{
	if (this->existed->getLimitMode() && \
		this->existed->getLimit() <= static_cast<long long>(this->existed->users.size())) {
			std::string msg = ":" + this->user.getHostName() + " 471 " + this->user.getNickName() + " " + this->existed->getName() + \
						" :Cannot join channel (+l)";
		Communicate::sendToClient(this->user.getFd(), msg);		
    	return false;
	}

	if ((this->existed->getKeyMode() && password.empty()) || (this->existed->getKeyMode() && password != this->existed->getKey())) {
		std::string msg = ":" + this->user.getHostName() + " 475 " + this->user.getNickName() + " " + this->existed->getName() + \
						" :Cannot join channel (+k)";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}

	if (this->existed->getInviteMode() && this->existed->invite.find(this->user.getNickName()) == this->existed->invite.end()) {
		std::string msg = ":" + this->user.getHostName() + " 473 " + this->user.getNickName() + " " + this->existed->getName() + \
						" :Cannot join channel (+i)";
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
		std::string msg = ":" + this->user.getNickName() + " 461 JOIN :Not enough parameters";
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

