#include "Command.hpp"

Command::Command()
{
}

int Command::getFd() const
{
    return this->fd;
}

std::string Command::getMsg() const
{
    return this->msg;
}

std::string Command::getCmd() const
{
    return this->cmd;
}

void Command::setCmd(std::string &cmd)
{
    this->cmd = cmd;
}
