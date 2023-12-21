#include "Message.hpp"

Message::Message(int fd, std::string cmd) : fd(fd), origin(cmd), cmd(""), trailing("") {
	splitMessage();
}

void Message::splitMessage() {
	std::istringstream iss(origin);
	std::string token;

	iss >> cmd;
	std::streampos start;

	while (iss >> token && token[0] != ':' && token.find('\r') == std::string::npos && token.find('\n') == std::string::npos) {
		setParameters(token);

		// 현 위치
		start = iss.tellg();
	}

	std::string left;
	if (start != -1) {
		iss.seekg(start);
		std::getline(iss, left);

		// :prefix COMMAND pram1 param2 :trailing -> trailing을 저장하기 위함
		// :sender PRIVMSG #channel :This is a message with a trailing part.
		if (left[1] == ':') {
			setTrailing(left.substr(2));
		}
	}
}

int Message::getFd() const
{
	return fd;
}

std::vector<std::string> Message::getParams() const
{
	return param;
}

std::string Message::getTrailing() const
{
	return trailing;
}

std::string Message::getOrigin() const
{
	return origin;
}

std::string Message::getCmd() const
{
	return cmd;
}

void Message::setParameters(std::string str) {
	this->param.push_back(str);
}

void Message::setTrailing(std::string str) {
	this->trailing = str;
}

