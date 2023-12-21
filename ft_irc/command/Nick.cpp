#include "Nick.hpp"

bool Nick::checkForm()
{
    if (getParameters()[0].size() >= 10) {
        std::string warning = "432 " + getParameters().at(0) + " :Error nickname";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendToClient(user.getFd(), warning);
        return false;
    }
    return true;
}

Nick::Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser) : Command(msg), user(userInfo), infoOfUser(infoOfUser)
{
}

Nick::~Nick()
{
}

void Nick::execute()
{
    if (user.getActive() || !user.getPass()) {
        return ;
    }

    if (getParameters().size() < 1) {
        std::string warning = "431 :No nickname given";
        Communicate::sendToClient(user.getFd(), warning);
        close(user.getFd());
    }

    if (checkForm()) {
        //switch nick Status
        user.checkNick();
        user.setNickName(getParameters()[0]);
        std::cout << "set nickName compeleted";
    }
}
