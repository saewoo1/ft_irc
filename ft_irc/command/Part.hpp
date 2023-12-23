#ifndef PART_HPP
# define PART_HPP

# include "Command.hpp"
# include "../srcs/UserInfo.hpp"
# include "Communicate.hpp"
# include "../srcs/Channel.hpp"

class Part : public Command {
    private	:
			UserInfo &user;
			std::map<int, UserInfo> allUserInfo;
			std::map<std::string, Channel> &channelList;
			Channel *partChannel;

	public	:
			Part(Message *msg, UserInfo &user, std::map<int, UserInfo> &allUserInfo, std::map<std::string, Channel> &channels);
			void execute();
			bool isPresentChannel(std::string &channelName);
			void partUser(std::string &channelName);
			void earseUserInUserInfo(std::string &channelName);
			int earseChannelInUser(std::string &channelName);
};
#endif