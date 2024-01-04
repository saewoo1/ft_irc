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
        Communicate::sendToClient(user.getFd(), warning);

        return ;
    }

    if (user.getActive()) {

        std::string warning = "462 :You may not reregister";
        Communicate::sendToClient(user.getFd(), warning);

        return ;
    }

    if (getParameters().at(0) == password) {
        // 유저의 로그인 성공 표시 on, 서버 측에만 패스워드 맞았다고 출력
        user.checkPass();
        std::cout << "client " << user.getFd() <<" password complete" << std::endl;
        
    } else {
        std::string msg = "464 :Password incorrect";
        Communicate::sendToClient(user.getFd(), msg);
        close(user.getFd());
        return ;
    }
}