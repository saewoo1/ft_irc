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

Nick::Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser) : Command(msg), user(userInfo), allUserInfo(infoOfUser) {

}

Nick::~Nick()
{
}

void Nick::execute()
{
    checkDuplicateNickName();
    // 활성화 되어있거나, PASSWORD 충족 못하면?
    if (user.getActive() || !user.getPass()) {
        Communicate::sendToClient(user.getFd(), "이미 등록된 유저이거나, 패스워드를 입력하기 전에 NICK 명령어 사용");
        return ;
    }

    // 닉네임을 입력하지 않은 경우
    if (getParameters().size() < 1) {
        std::string warning = "431 :No nickname given";
        Communicate::sendToClient(user.getFd(), warning);
//        close(user.getFd());
    }

    if (checkForm()) {
        //switch nick Status
        user.setNick(true);
        user.setNickName(getParameters()[0]);
        std::cout << "set nickName compeleted";
    }
}

void Nick::checkDuplicateNickName() {
    std::map<int, UserInfo>::iterator it;
    for (it = allUserInfo.begin(); it != allUserInfo.end(); it++) {
        if (it->second.getNickName() == user.getNickName() && it->second.getActive()) {
            std::string errMsg = ":" + user.getNickName() + " is Duplicate...";
            Communicate::sendToClient(user.getFd(), errMsg);
            close(user.getFd());
        }
    }

}
