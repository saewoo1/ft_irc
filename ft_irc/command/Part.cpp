#include "Part.hpp"

Part::Part(Message *msg, UserInfo &user, std::map<int, UserInfo> &allUserInfo, std::map<std::string, Channel> &channelList) : Command(msg), user(user), allUserInfo(allUserInfo), channelList(channelList)
{
}

void Part::execute()
{
    if (!this->user.getActive()) {
        return ;
    }
    if (this->getParameters().size() < 1) {
		std::string msg = "461 " + this->user.getNickName() + " PART :Not enough parameters";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}
    if (!isPresentChannel(this->getParameters().at(0))) {
        std::string msg = ":" + user.getHostName() + " 403 " + user.getNickName() + " " + getParameters().at(0) + " :No such channel";
        Communicate::sendToClient(this->user.getFd(), msg);
        return ;
    }
    partUser(this->getParameters().at(0));
}

bool Part::isPresentChannel(std::string &channelName)
{
    for (std::map<std::string, Channel>::iterator it = this->channelList.begin(); it != this->channelList.end(); it++) {
        if (it->second.getName() == channelName) {
            return true;
        }
    }
    return false;

}

void Part::partUser(std::string &channelName)
{
    if (!earseChannelInUser(channelName)) {
        return ;
    }

    earseUserInUserInfo(channelName);

    std::string msg;
    if (this->getTrailing().empty()) {
        msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + " PART :" + this->getParameters().back();
        Communicate::sendToClient(this->user.getFd(), msg);
    } else {
        std::string partMsg = this->getTrailing();
        msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + " PART " + channelName + \
                            " :" + partMsg;
        Communicate::sendToClient(this->user.getFd(), msg);
    }
    partChannel = &(this->channelList.find(channelName))->second;
    partChannel->setUserCount(-1);
    for (std::map<std::string, UserInfo>::iterator it = partChannel->users.begin(); it != partChannel->users.end(); it++) {
        Communicate::sendToClient(it->second.getFd(), msg);
    }
    
    if (partChannel->getUserCount() == 0) {
        std::map<std::string, Channel>::iterator it = this->channelList.find(partChannel->getName());
        channelList.erase(it);
    }
}

int Part::earseChannelInUser(std::string &channelName)
{
    for (std::map<std::string, bool>::iterator it = this->user.channels.begin(); it != this->user.channels.end(); it++) {
        if (it->first == channelName) {
            user.channels.erase(it);
            return 1;
        }
    }

    std::string msg = ":" + this->user.getHostName() + " 442 " + this->user.getNickName() + " " + channelName + " :You're not on that channel";
    Communicate::sendToClient(this->user.getFd(), msg);
    return 0;
}

void Part::earseUserInUserInfo(std::string &channelName)
{
    for (std::map<std::string, Channel>::iterator iterChannels = this->channelList.begin(); iterChannels != this->channelList.end(); iterChannels++) {
        if (iterChannels->second.getName() == channelName) {
            Channel *part = &iterChannels->second;
            for (std::map<std::string, UserInfo>::iterator it = part->users.begin(); it != part->users.end(); it++) {
                std::cout << it->first << ", " << this->user.getNickName() << std::endl;
                if (it->first == this->user.getNickName()) {
                    std::string name = it->first;
                    part->users.erase(it);
                    part->operators.erase(name);
                    return ;
                }
            }
        }
    }
}

