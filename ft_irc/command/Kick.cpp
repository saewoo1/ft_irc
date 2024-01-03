#include "Kick.hpp"

Kick::Kick(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList, std::map<std::string, Channel> &channelList) : Command(msg), user(user), userList(userList), channelList(channelList)
{
}

void Kick::execute()
{
    if (!this->user.getActive()) {
        return ;
    }
    //파라미터가 있는지
    if (this->getParameters().size() < 2) {
		std::string msg = ":" + this->user.getNickName() + " 461 KICK :Not enough parameters";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}
    //존재하는 채널인지 확인
    if (!isPresentChannel(this->getParameters().at(0))) {
        return ;
    }
    //존재하는 유저인지 //kick 대상이 존재하는지
    if (!isPresentKickUser(this->getParameters().at(1))) {
        return ;
    }
    //user가 채널에 있는지 확인
    if (!isUserInKickChannel()) {
        return ;
    }
    // kick 대상이 채널에 있는지 확인
    if (!isKickUserInKickChannel()) {
        return ;
    }
    //operator인 지 확인
    if (!isOperator()) {
        return ;
    }
    this->kickChannel->users.erase(this->kickUser->getNickName());
    this->kickUser->channels.erase(this->kickChannel->getName());
    std::string msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + " KICK " + \
                    this->kickChannel->getName() + " " + this->kickUser->getNickName() + " :";
    if (this->getTrailing().length() >= 1) {
        msg += this->getTrailing();
    }
    std::string partMsg = ":" + this->kickUser->getNickName() + "!" + this->kickUser->getUserName() + "@" + this->kickUser->getServerName() + " PART " + this->kickChannel->getName();

    Communicate::sendToClient(this->kickUser->getFd(), partMsg);
    Communicate::sendToClient(this->kickUser->getFd(), msg);
    for (std::map<std::string, UserInfo>::iterator it = this->kickChannel->users.begin(); it != this->kickChannel->users.end(); it++) {
        Communicate::sendToClient(it->second.getFd(), msg);
    }
}

bool Kick::isPresentKickUser(std::string &kickName)
{
    std::map<int, UserInfo>::iterator it = this->userList.begin();

    for ( ; it != this->userList.end(); it++) {
        if (it->second.getNickName() == kickName) {
            this->kickUser = &it->second;
            return true;
        }
    }

    if (it == this->userList.end()) {
        std::string msg = ":" + this->user.getServerName() + " 401 " + this->user.getNickName() + " " + this->user.getRealName() + " :No such nick";
        Communicate::sendToClient(this->user.getFd(), msg);
    }
    return false;
}

bool Kick::isPresentChannel(std::string &channelName)
{
    std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);

	if (it == this->channelList.end()) {
		std::string msg = ":" + this->user.getServerName() + " 403 " + channelName + " :No such channel";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
	this->kickChannel = &it->second;
	return true;
}

bool Kick::isUserInKickChannel()
{
    std::map<std::string, UserInfo>::iterator it = this->kickChannel->users.find(this->user.getNickName());

    if (it == this->kickChannel->users.end()) {
        std::string msg = ":" + this->user.getServerName() + " 442 " + this->user.getNickName() + " " + this->kickChannel->getName() + " :You're not on that channel";
        Communicate::sendToClient(this->user.getFd(), msg);
        return false;
    }
    return true;
}

bool Kick::isKickUserInKickChannel()
{
    std::map<std::string, UserInfo>::iterator it = this->kickChannel->users.find(this->kickUser->getNickName());

    if (it == this->kickChannel->users.end()) {
        std::string msg = ":" + this->user.getServerName() + " 441 " + this->kickUser->getNickName() + " " + this->kickChannel->getName() + " :They aren't on that channel";
        Communicate::sendToClient(this->user.getFd(), msg);
        return false;
    }
    return true;
}

bool Kick::isOperator()
{
    std::map<std::string, UserInfo>::iterator it = this->kickChannel->operators.find(this->user.getNickName());
    if (it == this->kickChannel->operators.end()) {
        std::string msg = ":" + this->user.getServerName() + " 482 " + this->user.getNickName() + " " + this->kickChannel->getName() + " :You're not channel operator";
        Communicate::sendToClient(this->user.getFd(), msg);
        return false;
    }
    return true;
}
