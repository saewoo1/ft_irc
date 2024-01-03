#ifndef PRIVATEMESSAGE_HPP
# define PRIVATEMESSAGE_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"
# include "../srcs/Channel.hpp"

// 해당 유저 한명에게 귓속말 보내기
class PrivateMessage : public Command {
    private:
        UserInfo &user;
        std::map<int, UserInfo> &allUser;
        std::map<std::string, Channel> &allChannels;

        std::vector<std::string> receivers;

        // 다수의 이름도 처리해야함;
        void splitComma(std::string params);
        void sendPersonalMsg();

        bool findUserNickName();
        bool validateFormat();
        // 채널 내의 사용자들에게 전송할 경우
        bool isChannelMsg();
        bool validateChannelMsg();
        void sendChannelMsg();

        bool isInChannel(UserInfo user, Channel channel);
        int getReceiverFd();
    public:
        PrivateMessage(Message *msg, UserInfo &user, std::map<int, UserInfo> &allUserInfo, 
                        std::map<std::string, Channel> &allchannels);
        void execute();
};

#endif