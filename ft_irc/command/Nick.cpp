#include "Nick.hpp"

int Nick::checkForm()
{
    if (getParameters()[0].size() >= 10) {
        std::string warning = "432 " + getParameters().at(0) + " :Error nickname";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendWarnning(user.getFd(), warning);
        return 1;
    }
    return 0;
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
        Communicate::sendWarnning(user.getFd(), warning);
    }

    if (!checkForm()) {
        //switch nick Status
        user.checkNick();
        user.setNickName(getParameters()[0]);
        std::cout << "set nickName compeleted";
    }
}
