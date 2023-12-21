#include "Pass.hpp"

Pass::Pass(Message *msg, UserInfo &userInfo, std::string &password) : Command(msg), user(userInfo), password(password)
{
}

Pass::~Pass()
{
}

void Pass::execute()
{
    if (getParameters().size() < 1) {
        std::string warning = "461 PASS :Not enough parameters";
        Communicate::sendWarnning(user.getFd(), warning);
        return ;
    }

    if (user.getActive()) {
        std::string warning = "462 :You may not reregister";
        Communicate::sendWarnning(user.getFd(), warning);
        return ;
    }

    if (getParameters().at(1) == password) {
        user.checkPass();
        std::cout << "password completed" << std::endl;
    } else {
        std::string warning = "464 :Password incorrect";
        Communicate::sendWarnning(user.getFd(), warning);
        return ;
    }
}
