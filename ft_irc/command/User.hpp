#ifndef USER_HPP
# define USER_HPP

# include "Command.hpp"
# include "UserInfo.hpp"

class User : public Command {
    private :
            UserInfo user;

    public  :
            User(UserInfo &userInfo);
            ~User();

            void execute();
};
#endif