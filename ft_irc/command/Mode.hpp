#ifndef MODE_HPP
# define MODE_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"

class Mode : public Command {
	private	:
			UserInfo &user;
			std::map<int, UserInfo> &userList;
			std::map<std::string, Channel> &channelList;
			Channel *channel;
			bool isPresentChannel(std::string &channelName);
			std::string	getModeStatus();
	public	:
			Mode(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList,std::map<std::string, Channel> &channelList);
			void	execute();
};
#endif