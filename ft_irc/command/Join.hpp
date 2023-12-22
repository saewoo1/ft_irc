#ifndef JOIN_HPP
# define JOIN_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"

class Join : public Command {
	private	:
			UserInfo &user;
			std::map<std::string, Channel> &channelList;
			Channel *existed;
	public	:
			Join(Message *msg, UserInfo &user, std::map<std::string, Channel> &channelList);
			int	validateJoinExecute(const std::string &channelName, const std::string &password);
			bool checkJoinConditions(const std::string &password);
			void createNewChannel(std::string channelName);
			void joinChannel(std::string channelName, const std::string &password);
			void execute();

};
#endif