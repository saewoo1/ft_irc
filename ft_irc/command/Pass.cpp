#include "Pass.hpp"

Pass::Pass(Message *msg, UserInfo &userInfo, std::string &password) : Command(msg), user(userInfo), password(password)
{
}

Pass::~Pass()
{
}

void Pass::execute()
{
    std::cout << "in Pass execute..." << std::endl;
    std::cout << "password is... " << password << std::endl;
    std::cout << "params at 0: " << getParameters().at(0) << std::endl;
    std::cout << "clear" << std::endl;

    if (getParameters().size() < 1) {
        std::cout << "size is less then 1" << std::endl;

        std::string warning = "461 PASS :Not enough parameters";
        sendMsg(user.getFd(), warning);
        return ;
    }

    if (user.getActive()) {
        std::cout << "user is Active" << std::endl;

        std::string warning = "462 :You may not reregister";
        sendMsg(user.getFd(), warning);
        return ;
    }

    if (getParameters().at(0) == password) {
        user.checkPass();
        std::string result = "password complete";
        sendMsg(user.getFd(), result);
    } else {
        std::string warning = "464 :Password incorrect";
        sendMsg(user.getFd(), warning);
        return ;
    }
}

void sendMsg(int fd, std::string str)
{
    str += "\r\n";
	const char *reply = str.c_str();

	std::cout << str;

	int result = send(fd, const_cast<char *>(reply), strlen(reply), 0);

	if (result == -1)
		throw std::runtime_error("Error: send error");
}