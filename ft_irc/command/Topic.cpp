#include "Topic.hpp"

Topic::Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList) : Command(msg), user(user), channels(channelList) {
}

// 실제로 존재하는 채널인지 param으로 검증,
// 중복되는 topic인지 검수하는 기능
bool Topic::isValidChannel() {

    if (getParameters().size() == 0) {
        std::string msg = ":" + user.getHostName() + " 461 " + user.getNickName() + " TOPIC :Not enough parameters";
        Communicate::sendToClient(user.getFd(), msg);

        return false;
    }

    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
       std::string msg = ":" + user.getHostName() + " 403 " + user.getNickName() + " " + channelName + " :No such channel";
       Communicate::sendToClient(user.getFd(), msg);

       return false;
    }
    return true;
}

// topic이 비어있다면 false 반환, 조회로 판별한다
bool Topic::isExistTopic() {
    if (getTrailing().empty()) {
        return false;
    }
    return true;
}

Topic::~Topic() {
}

// 채널 내의 토픽을 보여주는 기능 -> 있으면 보여주고, 없으면 없다는 메세지 보여줘야됨
// TOPIC #saewoo 입력한 경우, topic이 있는지 검증합니다.
void Topic::showChannelTopic(Channel channel) {

    // 만일 토픽이 세팅되어있지 않다면
    if (channel.getTopic().empty()) {
        std::string result = ":" + user.getHostName() + " 331 " + user.getNickName() + " " + channel.getName() + " :No topic is set";
        Communicate::sendToClient(user.getFd(), result); 
        return ;
    }
    // 해당 채널의 topic을 보여줍니다.
    std::string result = ":" + user.getHostName() + " 332 " + user.getNickName() + " " + channel.getName() + " :" + channel.getTopic();
    Communicate::sendToClient(user.getFd(), result);
}

/**
 * 변경 시점 updateTopic 하는 순간 channel의 topic은 무조건 trailing임.
 * setTopic을 한 후에 들어온 user들은 어떻게 하지?
*/
void Topic::updateTopic(Channel &channel) {
    channel.setTopic(getTrailing());
    std::map<std::string, UserInfo>::iterator it = channel.users.begin();

    for (; it != channel.users.end(); it++) {
        UserInfo userInChannel = it->second;
        std::string msg = ":" + user.getNickName() + "!" + userInChannel.getUserName() + "@" + user.getHostName() + " TOPIC " + channel.getName() + " :" + getTrailing();
        Communicate::sendToClient(userInChannel.getFd(), msg);
    }
}

// 채널 내에 sender가 존재하는지 검증
bool Topic::isUserInChannel() {
    std::map<std::string, bool>::iterator it = user.channels.find(getParameters().at(0));

    if (it == user.channels.end()) {
        std::string msg = ":" + user.getHostName() + " 442 " + user.getNickName() + " " + getParameters().at(0) + " :You're not on that channel";
        Communicate::sendToClient(user.getFd(), msg);
        return false;
    }
    return true;
}

bool Topic::isDuplicateTopic(Channel channel) {
    if (channel.getTopic() == getTrailing()) {
        return true;
    }
    return false;
}


bool Topic::isOperator(Channel channel) {
    std::string username = user.getNickName();

    if (channel.operators.find(username) == channel.operators.end()) {
        return false;
    }

    return true;
}

bool Topic::isTopicModeOn(Channel channel) {

    if (!channel.getTopicMode() || !isOperator(channel)) {
        std::string msg = ":" + user.getHostName() + " 482 " + user.getNickName() + " " + channel.getName() + " :You do not have access to change the topic on this channel";
        return false;
    }
    return true;
}

void Topic::execute() {
    std::map<std::string, Channel>::iterator it = channels.find(getParameters().at(0));
    Channel &channel = it->second;

    if (!user.getActive()) {
        return;
    }

    // 실제로 존재하는 채널인지 검수하는 기능
    if (!isValidChannel()) {
        return;
    }
    // trailing이 존재하지 않는다면, 조회 기능으로 판별
    if (!isExistTopic()) {
        // 조회 기능
        showChannelTopic(channel);
        return ;
    }
    if (!isUserInChannel() || !isTopicModeOn(channel)) {
        return ;
    }

    // trailing이 존재한다면, 해당 Trailing으로 channel의 topic을 업데이트합니다.
    if (!isDuplicateTopic(channel)) {
        updateTopic(channel);
    }
}