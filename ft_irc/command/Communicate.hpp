#ifndef COMMUNICATE_HPP
# define COMMUNICATE_HPP

# include "../srcs/Server.hpp"
# include <vector>
# include <string>
class Communicate {
    public  :
            static void sendWarnning(int clientFd, std::string warning);
};
#endif