#ifndef MODE_HPP
# define MODE_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"
# include <vector>

class Mode : public Command {
	private	:
			UserInfo &user;
			std::map<int, UserInfo> &userList;
			std::map<std::string, Channel> &channelList;
			Channel *channel;
			std::vector<std::string> changedModeOptions;
			std::vector<std::string> changedParam;
			size_t paramIndex;
			bool isPresentChannel(std::string &channelName);
			std::string	getModeStatus();
			bool isOperator();
			std::vector<std::string> getOptions(std::string &option);
			void executeOptions(std::vector<std::string> options);
			void executeInvite(std::string);
			void executeKey(std::string);
			void executeLimit(std::string);
			void executeOperator(std::string);
			void executeTopic(std::string);
			void executeNon(std::string);
			bool isPresentUser(std::string);
			void changeUserInfo(UserInfo &userInfo, bool type);
			UserInfo *findByNickName(std::string);
			void sendMsg();
			std::string deleteSignInOptions();
	public	:
			Mode(Message *msg, UserInfo &user, std::map<int, UserInfo> &userList,std::map<std::string, Channel> &channelList);
			void	execute();
};
#endif