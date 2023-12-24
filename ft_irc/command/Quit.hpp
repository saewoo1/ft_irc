#ifndef QUIT_HPP
# define QUIT_HPP

#include "Command.hpp"
#include "../srcs/UserInfo.hpp"
#include "../srcs/Channel.hpp"
#include "../srcs/Server.hpp"

class Quit : public Command {
	private:
		UserInfo &user;
	public:
		size_t i;
		std::map<std::string, Channel> &allChannel;
		std::map<int, UserInfo> &allUsers;
		std::vector<pollfd> &pollfds;

		Quit(Message *msg, UserInfo &user, std::map<std::string, Channel> &allChannel, std::map<int, UserInfo> &allUsers, std::vector<pollfd> &pollfds);
		void execute();
};

#endif