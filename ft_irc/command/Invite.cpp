#include "Invite.hpp"

Invite::Invite(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList, std::map<int, UserInfo> &userList) : Command(msg), user(user), channelList(channelList), userList(userList)
{
}

void Invite::execute()
{
    if (!this->user.getActive()) {
        return ;
    }
    
    // INVITE 닉네임, 채널명이 아닐경우 예외 처리
    if (this->getParameters().size() != 2) {
		std::string msg = ":" + this->user.getNickName() + " 461 INVITE :Not enough parameters";
		Communicate::sendToClient(this->user.getFd(), msg);
		return ;
	}

    // 초대 받을 유저 확인
    if (!isPresentGeust(this->getParameters().at(0))) {
        return ;
    }

    // 채널이 존재하는지 확인
	if (!isPresentChannel(this->getParameters().at(1))) {
		return ;
	}
	// 채널 안에 geust가 존재하는지 확인
	if (!isGuestInChannel(this->getParameters().at(0))) {
		return ;
	}
	//host가 operator 인지 확인
	if (!isOperator()) {
		return ;
	}

	// 체널 invite에 guest를 넣고 341 반환, 초대받는 대상에게는 메시지 반환
	this->channel->invite.insert(std::make_pair(this->getParameters().at(0), *this->guest));
	std::string msg = ":" + this->user.getHostName() + " 341 " + this->user.getNickName() + " " + this->getParameters().at(0) + \
				" " + this->getParameters().at(1);
	Communicate::sendToClient(this->user.getFd(), msg); 

	msg.clear();
	msg += ":" + this->user.getNickName() + "!" + this->guest->getNickName() + "@" + this->user.getServerName() + " INVITE " \
			+ this->guest->getNickName() + " :" + this->getParameters().at(1);
	Communicate::sendToClient(this->guest->getFd(), msg);
}

bool Invite::isPresentGeust(std::string &guestName)
{
    std::map<int, UserInfo>::iterator it = this->userList.begin();

    for ( ; it != this->userList.end(); it++) {
        if (it->second.getNickName() == guestName) {
            this->guest = &it->second;
            return true;
        }
    }

    if (it == this->userList.end()) {
        std::string msg = ":" + this->user.getHostName() + " 401 " + this->user.getNickName() + " :INVITE " + \
                        ":" + guestName + " :No such nick/channel";
        Communicate::sendToClient(this->user.getFd(), msg);
    }
    return false;
}

bool Invite::isPresentChannel(std::string &channelName)
{
    std::map<std::string, Channel>::iterator it = this->channelList.find(channelName);

	if (it == this->channelList.end()) {
		std::string msg = ":" + this->user.getHostName() + " 403 " + channelName + " :No such channel";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
	this->channel = &it->second;
	return true;
}

bool Invite::isGuestInChannel(std::string &guestName)
{
	std::cout << "channel people " << this->channel->users.size() << std::endl;

	std::map<std::string, UserInfo>::iterator it = this->channel->users.find(guestName);
	if (it != this->channel->users.end()) {
		std::string msg = ":" + this->user.getHostName() + " 443 " + guestName + " " + this->channel->getName() + " :is already on channel";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
    return true;
}

bool Invite::isOperator()
{
	std::map<std::string, UserInfo>::iterator it = this->channel->operators.find(this->user.getNickName());
	if (it == this->channel->operators.end()) {
		std::string msg = ":" + this->user.getHostName() + " 481 " + this->channel->getName() + " :Permission Denied- You're not an IRC operator";
		Communicate::sendToClient(this->user.getFd(), msg);
		return false;
	}
    return true;
}
