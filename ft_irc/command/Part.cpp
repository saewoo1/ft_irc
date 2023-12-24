#include "Part.hpp"

Part::Part(Message *msg, UserInfo &user, std::map<int, UserInfo> &allUserInfo, std::map<std::string, Channel> &channelList) : Command(msg), user(user), allUserInfo(allUserInfo), channelList(channelList)
{
}

void Part::execute()
{
    if (!this->user.getActive()) {
        return ;
    }
    // std::cout << "11111111" << std::endl;
    if (this->getParameters().size() < 1) {
		std::string msg = ":" + this->user.getNickName() + " 461 PART :Not enough parameters\n";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}
    // std::cout << "22222222" << std::endl;
    if (!isPresentChannel(this->getParameters().at(0))) {
        std::string msg = ":" + user.getHostName() + " 403 " + user.getNickName() + " " + getParameters().at(0) + " :No such channel\n";
        Communicate::sendToClient(this->user.getFd(), msg);
        return ;
    }
    // std::cout << "3333333" << std::endl;
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
    std::cout << "44444444444" << std::endl;
    if (!earseChannelInUser(channelName)) {
        return ;
    }
    std::cout << "3333333" << std::endl;

    earseUserInUserInfo(channelName);
    std::cout << "55555555" << std::endl;

    std::string msg;
    if (this->getTrailing().empty()) {
        msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + "PART " + channelName + "\n";
        Communicate::sendToClient(this->user.getFd(), msg);
    } else {
        std::string partMsg = this->getTrailing();
        msg = ":" + this->user.getNickName() + "!" + this->user.getUserName() + "@" + this->user.getServerName() + "PART " + channelName + \
                            " :" + partMsg + "\n";
        Communicate::sendToClient(this->user.getFd(), msg);
    }
std::cout << "888888888888" << std::endl;
    partChannel = &(this->channelList.find(channelName))->second;
    for (std::map<std::string, UserInfo>::iterator it = partChannel->users.begin(); it != partChannel->users.end(); it++) {
        Communicate::sendToClient(it->second.getFd(), msg);
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

    std::string msg = ":" + this->user.getHostName() + " 442 " + this->user.getNickName() + " " + channelName + " :You're not on that channel\n";
    Communicate::sendToClient(this->user.getFd(), msg);
    return 0;
}

void Part::earseUserInUserInfo(std::string &channelName)
{
    for (std::map<std::string, Channel>::iterator iterChannels = this->channelList.begin(); iterChannels != this->channelList.end(); iterChannels++) {
        if (iterChannels->second.getName() == channelName) {
            std::cout << "55555555" << std::endl;

            Channel part = iterChannels->second;
            for (std::map<std::string, UserInfo>::iterator it = part.users.begin(); it != part.users.end(); it++) {
                if (it->first == this->user.getNickName()) {
                    std::cout << "?SDFSDFS?" << std::endl;
                    std::string name = it->first;
                    part.users.erase(name);
                    part.operators.erase(name);
                    return ;
                }
            }
        }
    }
}
