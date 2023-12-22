#ifndef PRIVATEMESSAGE_HPP
#define PRIVATEMESSAGE_HPP

#include "Command.hpp"
#include "Communicate.hpp"
#include "../srcs/UserInfo.hpp"

// 해당 유저 한명에게 귓속말 보내기
class PrivateMessage : public Command {
    private:
        UserInfo &user;
        std::map<int, UserInfo> allUser;

        bool findUserNickName();
        bool validateFormat();
        int getReceiverFd();
        std::string generateSendFormat();
    public:
        PrivateMessage(Message *msg, UserInfo &user, std::map<int, UserInfo> allUserInfo);
        void execute();
};

#endif