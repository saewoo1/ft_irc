#ifndef USER_HPP
# define USER_HPP

# include "Command.hpp"
# include "../srcs/UserInfo.hpp"
# include "Communicate.hpp"

class User : public Command {
    private:
        UserInfo &user;

    public:
        User(Message *msg, UserInfo &userInfo);
        ~User();
        void execute();
};
#endif