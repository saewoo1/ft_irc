#ifndef NICK_HPP
# define NICK_HPP

# include "Command.hpp"
# include "UserInfo.hpp"
# include <map>

class Nick : public Command {
    private :
            UserInfo user;
            std::map<int, UserInfo> infoOfUser;

            int checkForm();
    public  :
            Nick(UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser);
            ~Nick();

            void execute();
};
#endif