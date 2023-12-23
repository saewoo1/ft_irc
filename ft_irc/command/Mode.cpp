#include "Mode.hpp"

Mode::Mode(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList, std::map<std::string, Channel> &channelList) : Command(msg), user(user), userList(userList), channelList(channelList)
{
}

void Mode::execute()
{
    //유저가 active 한지
    if (!this->user.getActive()) {
        return ;
    }

    // 명령어가 잘 들어왔는지
    if (this->getParameters().size() == 0) {
        std::string msg = ":" + this->user.getNickName() + " 461 MODE :Not enough parameters\n";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
    }

    //채널이 존재하는지
    if (!isPresentChannel(this->getParameters().at(0))) {
        return ;
    }

    //채팅방에 적용된 mode 확인하기
    if (this->getParameters().size() == 1) {
        std::string status = getModeStatus();
        std::string msg = ":" + this->user.getServerName() + " 324 " + this->user.getNickName() + " " + this->channel->getName() + " " + status + "\n";
        Communicate::sendToClient(this->user.getFd(), msg);
        return ;
    }
    // mode 설정하기 ex -i -l -k etc
    if (this->getParameters()[1].size() < 2) {
        return ;
    }

    if (this->getParameters()[1][0] != '-' && this->getParameters()[1][0] != '+') {
        return ;
    }
}


bool Mode::isPresentChannel(std::string &channelName)
{
    std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);

	if (it == this->channelList.end()) {
		std::string msg = ":" + this->user.getServerName() + " 403 " + channelName + " :No such channel\n";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
	this->channel = &it->second;
	return true;
}

std::string Mode::getModeStatus()
{
    std::string status;
    bool key = false;
    bool limit = false;

    if (this->channel->getInviteMode()) {
        status += "i";
    }
    if (this->channel->getKeyMode()) {
        status += "k";
        key = true;
    }
    if (this->channel->getLimitMode()) {
        status += "l";
        limit = true;
    }
    if (this->channel->getTopicMode()) {
        status += "t";
    }

    if (key) {
        status += " " + this->channel->getKey();
    }
    if (limit) {
        std::stringstream ss;
        ss << this->channel->getLimit();
        status += " " + ss.str();
    }

    return status;
}
