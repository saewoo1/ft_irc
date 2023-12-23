#include "Nick.hpp"

bool Nick::checkForm()
{
    if (getParameters()[0].size() >= 10) {
        std::string warning = "432 " + getParameters().at(0) + " :Error nickname";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendToClient(user.getFd(), warning);
        return false;
    }
    if (isDuplicateNickName()) {
        Communicate::sendToClient(user.getFd(), "중복된 닉네임입니다.");
        return false;
    }
    return true;
}

Nick::Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser) : Command(msg), user(userInfo), allUserInfo(infoOfUser) {

}

Nick::~Nick()
{
}

/**
 * 닉네임 등록 / 변경 기능
 * 1. 로그인 이전 -> 새로 닉네임 등록
 * 2. 로그인 이후 -> 기존 닉네임에서 새로운 닉네임으로 변경.
*/
void Nick::execute()
{
    // PASSWORD 충족 못하면? 우선순위 어덕하지
    if (!user.getPass()) {
        Communicate::sendToClient(user.getFd(), "비밀번호 입력이 되어있지 않습니다!");
        return;
    }

    if (getParameters().size() < 1) {
        std::string warning = "431 :No nickname given";
        Communicate::sendToClient(user.getFd(), warning);
        return ;
    }
    // NICK J J J J -> J로 등록되는데 이거도 size 검수해서 param 1 아니면 에러 띄워여?

    if (user.getActive()) {
        if (!isDuplicateNickName()) {
            // 기존 유저의 닉네임 변경 시 setNick -> true 변환.
            user.setNick(true);
            user.setNickName(getParameters().at(0));
            Communicate::sendToClient(user.getFd(), "new NickName Set clear!");
            return ;
        }
        Communicate::sendToClient(user.getFd(), "중복된 닉네임입니다.");
        return ;
    }

    if (checkForm()) {
        // 닉네임 최초 등록
        user.setNickName(getParameters()[0]);
        std::cout << "set nickName compeleted";
    }
}

bool Nick::isDuplicateNickName() {
    std::map<int, UserInfo>::iterator it;
    for (it = allUserInfo.begin(); it != allUserInfo.end(); it++) {
        // 입력한 닉네임과, 기존에 로그인이 성공한 닉네임이 겹친다면..
        if ((it->second.getNickName() == getParameters().at(0)) && it->second.getActive()) {
            return true;
        }
    }
    return false;

}
