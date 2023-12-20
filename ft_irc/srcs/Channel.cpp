#include "Channel.hpp"

Channel::Channel(UserInfo &user, std::string name) : key(""), topic(""), limit(-1), userCount(1), inviteMode(false), keyMode(false), limitMode(false), topicMode(true)
{
	if (name.length() > 200)
		this->name = name.substr(0, 200);
	else
		this->name = name;

	operators[user.getNickName()] = user;
	users[user.getNickName()] = user;
}


std::string Channel::getName() const {
	return name;
}
std::string Channel::getKey() const
{
	return key;
}
std::string Channel::getTopic() const {
	return topic;
}
long long Channel::getLimit() const
{
	return limit;
}
size_t Channel::getUserCount() const{
	return userCount;
}
bool Channel::getInviteMode() const
{
	return inviteMode;
}
bool Channel::getKeyMode() const
{
	return keyMode;
}
bool Channel::getLimitMode() const
{
	return limitMode;
}
bool Channel::getTopicMode() const
{
	return topicMode;
}

void Channel::setKey(std::string key)
{
	this->key = key;
}
void Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

void Channel::setLimit(size_t limit)
{
	this->limit = limit;
}

void Channel::setUserCount(int count) {
    this->userCount = count;
}

// 채팅창 인원 증가
void Channel::increaseUserCount() {
    this->userCount = static_cast<size_t>(static_cast<int>(this->userCount) + 1);
}

// 강퇴
void Channel::decreaseUserCount() {
    this->userCount = static_cast<size_t>(static_cast<int>(this->userCount) - 1);
}