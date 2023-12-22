#include "User.hpp"

User::User(Message *msg, UserInfo &userInfo) : Command(msg), user(userInfo)
{
}

User::~User()
{
}

void User::execute()
{
    if (user.getActive() || !user.getPass()) {
        Communicate::sendToClient(user.getFd(), "이미 등록된 유저이거나, 패스워드를 입력하기 전에 NICK 명령어 사용");
        return ;
    }

    if (user.getActive()) {
        // 이미 등록된 user일 경우
        std::string warning = "462 USER :You may not reregister";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendToClient(user.getFd(), warning);
        return;
    }

    // 필수 정보를 모두 입력하지 않았거나, :trailing이 비어있을 경우.
    if (getParameters().size() < 3 || getTrailing().empty()) {
        std::string warning = "461 USER :You may not reregister";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendToClient(user.getFd(), warning);
        return;
    }

    // 지정된 타입은 USER username hostname servername :realname
    std::vector<std::string> parameters = getParameters();
    user.setUserName(parameters[0]);
    user.setHostName(parameters[1]);
    user.setServerName(parameters[2]);
    user.setRealName(getTrailing());

    user.checkUser();
    Communicate::sendToClient(user.getFd(), "user applied");
 }
