#include "PrivateMessage.hpp"

PrivateMessage::PrivateMessage(Message *msg, UserInfo &user, std::map<int, UserInfo> &allUserInfo, std::map<std::string, Channel> &allChannels) : Command(msg), user(user), allUser(allUserInfo), allChannels(allChannels) {
}

bool PrivateMessage::isChannelMsg() {
    if (getParameters().at(0)[0] == '#') {
        return true;
    }
    return false;
}

bool PrivateMessage::validateChannelMsg() {
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = allChannels.find(channelName);

    return (it != allChannels.end()) && (it->second.getUserCount() != 0);
}

void PrivateMessage::sendChannelMsg() {
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = allChannels.find(channelName);
    Channel &channel = it->second;

    std::map<std::string, UserInfo> usersInChannel = channel.users;
    std::map<std::string, UserInfo>::iterator userIt = usersInChannel.begin();


    std::string msg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + 
                        user.getServerName() + " PRIVMSG " +  + " " + 
                            channel.getName() + " :" + getTrailing();

    /**
     * 에러코드 수정해야됨
    */
    if (!isInChannel(user, channel)) {
        Communicate::sendToClient(user.getFd(), "you're not in channel");
        return ;
    }
    for (; userIt != usersInChannel.end(); userIt++) {
        if (userIt->second.getFd() != user.getFd()) {
            Communicate::sendToClient(userIt->second.getFd(), msg);
        }
    }
}

bool PrivateMessage::isInChannel(UserInfo user, Channel channel) {
    if (channel.users.find(user.getNickName()) == channel.users.end()) {
        return false;
    }
    return true;
}

void PrivateMessage::execute() {
    if (!validateFormat()) {
        Communicate::sendMessage(user, "412", "PRIVMSG", "No text to send");
        return;
    }

    if (isChannelMsg() && validateChannelMsg()) {
        sendChannelMsg();
        return ;
    }
   
    if (getReceiverFd() == -1) {
        Communicate::sendMessage(user, "404", "PRIVMSG", "No such nick/channel");
        return;
    }
    
    Communicate::sendToClient(getReceiverFd(), generateSendFormat());

}

int PrivateMessage::getReceiverFd() {
    std::string receiverName = getParameters().at(0);

    std::map<int, UserInfo>::iterator allUserIter = allUser.begin();
    for (; allUserIter != allUser.end(); allUserIter++) {
        if (allUserIter->second.getNickName() == receiverName) {
            return allUserIter->second.getFd();
        }
    }
    return -1;
}

std::string PrivateMessage::generateSendFormat() {
    std::string receiverNickName = "";
    std::map<int, UserInfo>::iterator it;
    for (it = allUser.begin(); it != allUser.end(); it++) {
        if (it->second.getNickName() == getParameters().at(0) && it->second.getActive()) {
            receiverNickName = it->second.getNickName();
        }
    }
    
    std::string msg = ":" + user.getNickName() + "!" + user.getUserName() + "@" + 
        user.getServerName() + " PRIVMSG " +  + " " + 
            receiverNickName + " :" + getTrailing();
    return msg;
}

bool PrivateMessage::validateFormat() {
    if (getParameters().size() != 1 || getTrailing().empty()) {
        return false;
    }
    return true;
}

bool PrivateMessage::findUserNickName() {
    std::map<int, UserInfo>::iterator it;
    for (it = allUser.begin(); it != allUser.end(); it++) {
        if (it->second.getNickName() == getParameters().at(0) && it->second.getActive()) {
            return true;
        }
    }
    return false;
}