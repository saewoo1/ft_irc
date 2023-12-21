#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "../srcs/UserInfo.hpp"
# include "Communicate.hpp"
# include <map>

class Nick : public Command {
    private:
            UserInfo user;
            std::map<int, UserInfo> allUserInfo;

            bool checkForm();
            void checkDuplicateNickName();
    public:
            Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser);
            ~Nick();

            void execute();
};
#endif