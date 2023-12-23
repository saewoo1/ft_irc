#ifndef PING_HPP
# define PING_HPP

# include "Command.hpp"
# include "Communicate.hpp"
# include "../srcs/UserInfo.hpp"

class Ping : public Command {
    private:
        UserInfo &user;
    public:
        Ping(Message *msg, UserInfo &user);
        void execute();
};

# endif