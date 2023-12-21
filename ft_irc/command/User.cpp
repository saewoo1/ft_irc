#include "User.hpp"

User::User(UserInfo &userInfo) : user(userInfo)
{
}

User::~User()
{
}

void User::execute()
{
    if (user.getActive() || !user.getPass()) {
        return ;
    }

    if (user.getActive()) {
        std::string warning = "462 USER :You may not reregister";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        //sendWarnning(user.getFd(), warning); 
        return;
    }

    if (getParameters().size() < 3 || getTrailing().empty()) {
        std::string warning = "461 USER :You may not reregister";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        //sendWarnning(user.getFd(), warning); 
        return;
    }

    std::vector<std::string> parameters = getParameters();
    user.setUserName(parameters[0]);
    user.setHostName(parameters[1]);
    user.setServerName(parameters[2]);
    user.setRealName(getTrailing());

    user.checkUser();
 }
