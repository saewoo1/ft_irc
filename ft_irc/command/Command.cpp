#include "Command.hpp"

Command::Command(Message *msg) {
    fd = msg->getFd();
    origin = msg->getOrigin();
    cmd = msg->getCmd();
    params = msg->getParams();
    trailing = msg->getTrailing();
}

Command::~Command()
{
}

Command::~Command()
{
}

int Command::getFd() const
{
    return this->fd;
}

std::string Command::getOriginalMsg() const
{
    return this->originalMsg;
}

std::string Command::getCmd() const
{
    return this->cmd;
}

std::vector<std::string> Command::getParameters() const
{
    return this->parameters;
}

std::string Command::getTrailing() const
{
    return this->trailing;
}

void Command::setCmd(std::string &cmd)
{
    this->cmd = cmd;
}

void Command::setParameters(std::string &parameter)
{
    this->parameters.push_back(parameter);
}

void Command::setTrailing(std::string &trailing)
{
    this->trailing = trailing;
}
