#ifndef KICK_HPP
# define KICK_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"

class Kick : public Command {
    private	:
			UserInfo &user;
			UserInfo *kickUser;
			std::map<int, UserInfo> &userList;
			std::map<std::string, Channel> &channelList;
			Channel *kickChannel;
			bool	isPresentKickUser(std::string &kickName);
			bool	isPresentChannel(std::string &channelName);
			bool	isUserInKickChannel();
			bool	isKickUserInKickChannel();
			bool	isOperator();
	public	:
			Kick(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList, std::map<std::string, Channel> &channelList);
			void execute();
};
#endif