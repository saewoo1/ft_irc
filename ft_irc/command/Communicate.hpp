#ifndef COMMUNICATE_HPP
# define COMMUNICATE_HPP

# include "../srcs/Server.hpp"
# include <vector>
# include <string>
class Communicate {
    public  :
            static void sendToClient(int clientFd, std::string warning);
};
#endif