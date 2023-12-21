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

        std::string warning = "461 PASS :Not enough parameters";
        sedMsgToClient(user.getFd(), warning);
        close(user.getFd());
    }

    if (user.getActive()) {

        std::string warning = "462 :You may not reregister";
        sedMsgToClient(user.getFd(), warning);
        close(user.getFd());

        return ;
    }

    if (getParameters().at(0) == password) {
        // 유저의 로그인 성공 표시 on, 서버 측에만 패스워드 맞았다고 출력
        user.checkPass();
        std::cout << user.getFd() <<" password complete" << std::endl;
        
    } else {
        std::string warning = "464 :Password incorrect";
        sedMsgToClient(user.getFd(), warning);
        close(user.getFd());

        return ;
    }
}

void sedMsgToClient(int fd, std::string str)
{
    str += "\r\n";
	const char *reply = str.c_str();

	std::cout << str;

	int result = send(fd, const_cast<char *>(reply), strlen(reply), 0);

	if (result == -1)
		throw std::runtime_error("Error: send error");
}