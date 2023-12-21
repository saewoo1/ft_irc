#include "UserInfo.hpp"


int UserInfo::getFd() const
{
    return fd;
}

std::string UserInfo::getNickName() const
{
    return nickName;
}

std::string UserInfo::getUserName() const
{
    return userName;
}

std::string UserInfo::getHostName() const
{
    return hostName;
}

std::string UserInfo::getServerName() const
{
    return serverName;
}

std::string UserInfo::getRealName() const
{
    return realName;
}

bool UserInfo::getNick() const {
    return nick;
}

bool UserInfo::getUser() const {
    return user;
}

bool UserInfo::getPass() const {
    return pass;
}

bool UserInfo::getActive() const
{
    return active;
}

void UserInfo::setFd(int fd) {
    this->fd = fd;
}

void UserInfo::setNickName(std::string nickName) {
    this->nickName = nickName;
}

void UserInfo::setUserName(std::string userName) {
    this->userName = userName;
}

void UserInfo::setHostName(std::string hostName) {
    this->hostName = hostName;
}

void UserInfo::setRealName(std::string realName) {
    this->realName = realName;
}

void UserInfo::setServerName(std::string serverName) {
    this->serverName = serverName;
}

UserInfo::UserInfo() : nickName(""), userName(""), hostName(""), serverName(""), realName(""), pass(false), nick(false), user(false), active(false) {}

std::ostream &operator<<(std::ostream &os, const UserInfo &obj) {
    os << "!!!!!USERINFO!!!!!" << std::endl;

    os << "fd : " << obj.getFd() << std::endl;
    os << "nickname : " << obj.getNickName() << std::endl;
	os << "username : " << obj.getUserName() << std::endl;
	os << "hostname : " << obj.getHostName() << std::endl;
	os << "servername : " << obj.getServerName() << std::endl;
	os << "realname : " << obj.getRealName() << std::endl
	   << std::endl;
	os << "end!!!!!!!!!" << std::endl;

	return os;

}