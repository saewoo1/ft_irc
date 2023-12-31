#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "../srcs/UserInfo.hpp"
# include "Communicate.hpp"
# include "../srcs/Channel.hpp"
# include <map>

class Nick : public Command {
    private:
            UserInfo &user;
            std::map<int, UserInfo> &allUserInfo;
            std::map<std::string, Channel> &channels;
            std::string password;

            bool checkForm();
            bool isDuplicateNickName();
            void updateUserNickName();
            bool isInChannel();
            void updateChannelUser(std::string newNickName);
    public:
            Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &users, std::map<std::string, Channel> &channels);
            ~Nick();

            void execute();
};
#endif