#ifndef COMMUNICATE_HPP
# define COMMUNICATE_HPP

# include "../srcs/Server.hpp"
# include <vector>
# include <string>
# include "Command.hpp"

class Communicate : public Command {
    public  :
            static void sendToClient(int clientFd, std::string warning);
            static void generateWarnMessage(UserInfo user, std::string errCode,
                                                    std::string cmd, std::string param, std::string warnMessage);
            static void sendMessage(UserInfo user, std::string errCode, std::string cmd, std::string result);

};
#endif