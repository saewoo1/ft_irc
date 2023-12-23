#include "Topic.hpp"

Topic::Topic(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList) : Command(msg), user(user), channels(channelList) {}

// 실제로 존재하는 채널인지 param으로 검증,
// 중복되는 topic인지 검수하는 기능
bool Topic::isValidChannel()
{
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        //:10.31.4.5 403 j :TOPIC :#fjfjfj :No such channel
        Communicate::generateWarnMessage(user, "403", getCmd(), getParameters().at(0), "No such channel");
    }
    return false;
}

// topic이 비어있다면 false 반환
bool Topic::validate() {
    return !getTrailing().empty();
}

// TOPIC이 다른 채널의 topic과 중복되는지 검증
bool Topic::checkTopic(std::string topicName) {
    // 모든 채널을 순회하며 존재하는 topic인지 검증한다.
    std::string topicName = getTrailing();
    std::map<std::string, Channel>::iterator it = channels.begin();
    for (; it != channels.end(); it++) {
        if (topicName == it->second.getTopic()) {
            Communicate::generateWarnMessage(user, "402", getCmd(), getTrailing(), "is Duplicate Topic name");
            return false;
        }
    }
    return true;
}

Topic::~Topic() {
}
/**
 * 
예를 들어, 채널 #example의 토픽을 조회하려면 다음과 같이 TOPIC 명령어를 사용할 수 있습니다:
TOPIC #example
또는 토픽을 변경하려면:

TOPIC #example :새로운 토픽 내용
이 명령을 통해 해당 채널의 토픽이 변경됩니다.
 * 
*/

// 실제로 존재하는 채널인지 검수하는 기능
