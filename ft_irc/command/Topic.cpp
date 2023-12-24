#include "Topic.hpp"

Topic::Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList) : Command(msg), user(user), channels(channelList) {
}

// 실제로 존재하는 채널인지 param으로 검증,
// 중복되는 topic인지 검수하는 기능
bool Topic::isValidChannel()
{
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        //:10.31.4.5 403 j :TOPIC :#fjfjfj :No such channel
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

bool Topic::checkTopic(std::string topicName) {
    // 모든 채널을 순회하며 존재하는 topic인지 검증한다. 이거 필요 업센 시발
    std::map<std::string, Channel>::iterator it = channels.begin();
    for (; it != channels.end(); it++) {
        if (topicName == it->second.getTopic()) {
            // 이 에러코드 맞는건지 몰라여
            Communicate::generateWarnMessage(user, "403", getCmd(), getTrailing(), "is Duplicate Topic name");
            return false;
        }
    }
    return true;
}

Topic::~Topic() {
}

// 채널 내의 토픽을 보여주는 기능 -> 있으면 보여주고, 없으면 없다는 메세지 보여줘야됨
// TOPIC #saewoo 입력한 경우, topic이 있는지 검증합니다.
void Topic::showChannelTopic() {
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    // 만일 토픽이 세팅되어있지 않다면
    if (it->second.getTopic().empty()) {
        Communicate::sendMessage(user, "331", getCmd(), "No topic is set");
        // std::string result = ":" + user.getServerName() + " 331 " + user.getNickName() + " " + getParameters().at(0) + " :No topic is set";
        // Communicate::sendToClient(user.getFd(), result); 
        return ;
    }
    // 해당 채널의 topic을 보여줍니다.
    std::string result = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getServerName() + " " + getCmd() + " " + getParameters().at(0) + " :" + it->second.getTopic();
    Communicate::sendToClient(user.getFd(), result);
}

void Topic::updateTopic(std::string topicName) {

    std::map<std::string, Channel>::iterator it = channels.find(getParameters().at(0));
    it->second.setTopic(topicName);
    std::string result = ":" + user.getNickName() + "!" + user.getUserName() + "@" + user.getServerName() + " " + getCmd() + " " + getParameters().at(0) + " :" + topicName;
    Communicate::sendToClient(user.getFd(), result);
}

void Topic::execute() {
    // 실제로 존재하는 채널인지 검수하는 기능
    if (!isValidChannel()) {
        Communicate::generateWarnMessage(user, "403", getCmd(), getParameters().at(0), "No such channel");
        return;
    }
    // trailing이 존재하지 않는다면, 조회 기능으로 판별
    if (!isExistTopic()) {
        // 조회 기능
        std::cout << "trailing이 존재하지 않으므로, 조회를 시작합니다." << std::endl;
        showChannelTopic();
        return ;
    }
    // trailing이 존재한다면, 해당 Trailing으로 channel의 topic을 업데이트합니다.
    updateTopic(getTrailing());
}