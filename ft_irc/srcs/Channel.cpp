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
    this->userCount = static_cast<size_t>(static_cast<int>(this->userCount + count));
}

void Channel::setInviteMode(bool mode)
{
	this->inviteMode = mode;
}

void Channel::setKeyMode(bool mode)
{
	this->keyMode = mode;
}

void Channel::setLimitMode(bool mode)
{
	this->limitMode = mode;
}

void Channel::setTopicMode(bool mode)
{
	this->topicMode = mode;
}

void Channel::changeInviteMode()
{
	if (this->inviteMode) {
		this->inviteMode = false;
	} else {
		this->inviteMode = true;
	}
}

void Channel::changeTopicMode()
{
	if (this->topicMode) {
		this->topicMode = false;
	} else {
		this->topicMode = true;
	}
}

// 채팅창 인원 증가
void Channel::increaseUserCount() {
    this->userCount = static_cast<size_t>(static_cast<int>(this->userCount) + 1);
}

// 강퇴
void Channel::decreaseUserCount() {
    this->userCount = static_cast<size_t>(static_cast<int>(this->userCount) - 1);
}

bool Channel::isOperator(std::string nickName)
{
    std::map<std::string, UserInfo>::iterator it = this->operators.find(nickName);

	if (it == operators.end()) {
		return false;
	}
	return true;
}

std::ostream &operator<<(std::ostream &os, Channel &obj)
{
    os << "[Channel]" << std::endl;
	os << "🧤name:\t" << obj.getName() << std::endl;
	os << "🧤password\t" << obj.getKey() << " [" << obj.getKeyMode() << "]" << std::endl;
	os << "🧤topic\t" << obj.getTopic() << std::endl;
	os << "🧤limit\t" << obj.getLimit() << " [" << obj.getLimitMode() << "]" << std::endl;
	os << "🧤user_count\t" <<  obj.getUserCount() << std::endl;
	os << "🧤invite_mode" << obj.getInviteMode() << std::endl;
	os << "🧤key_mode" << obj.getKeyMode() << std::endl;
	os << "🧤limit_mode" << obj.getLimitMode() << std::endl;
	os << "🧤topic_mode" << obj.getTopicMode() << std::endl;

	os << "[Invite list]" << std::endl;
	for (std::map<std::string, UserInfo>::iterator it = obj.invite.begin(); it != obj.invite.end(); it++) {
		os << "🖐" + it->first << std::endl;
	}
	
	os << "[User list]" << std::endl;
	for (std::map<std::string, UserInfo>::iterator it = obj.users.begin(); it != obj.users.end(); it++) {
		os << "🖐" + it->first << std::endl;
	}

	os << "[Admin list]" << std::endl;
	for (std::map<std::string, UserInfo>::iterator it = obj.operators.begin(); it != obj.operators.end(); it++) {
		os << "🖐" + it->first << std::endl;
	}

	return os;
}
