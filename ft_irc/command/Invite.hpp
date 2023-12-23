#ifndef INVITE_HPP
# define INVITE_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"

class Invite : public Command {
	private	:
			UserInfo &user;
			UserInfo *guest;
			Channel *channel;
			std::map<std::string, Channel> &channelList;
			std::map<int, UserInfo>	&userList;
			bool isPresentGeust(std::string &geustName);
			bool isPresentChannel(std::string &channelName);
			bool isGuestInChannel(std::string &geustName);
			bool isOperator();
	public	:

			Invite(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList, std::map<int, UserInfo> &userList);
			void execute();
};
#endif