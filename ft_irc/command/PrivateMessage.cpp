#include "PriavetMessage.hpp"


PrivateMessage::PrivateMessage(Message *msg, UserInfo &user, std::map<int, UserInfo> allUserInfo) : Command(msg), user(user), allUser(allUserInfo) {
    if (!findUserNickName()) {
        // 에러코드 모르겠어여 귓속말 보낼 유저가 존재하지 않을 때
        std::string result = "수신자가 존재하지 않는 닉네임이거나, 등록되지 않은 유저입니다.";
        Communicate::sendToClient(user.getFd(), result);
        return;
    }
    if (!validateFormat()) {
        std::string result = "수신 메세지 형식이 올바르지 않습니다.";
        Communicate::sendToClient(user.getFd(), result);
        return;
    }
}

void PrivateMessage::execute() {
    /**
     * 1. 모든 유저 중, 메세지에서 nick이 일치하는 유저를 검수한다.
     *  - [ ] nick이 일치하는 유저를 반환하는 기능
     *  PRIVMSG mick :testme!\r\n 상대방 nickname, 보낼 텍스트
     *  message에는 커맨드, 파라미터, trailing 순서로 들어오게 된다.
     *
     *  :saewoo!hihi@10.19.203.89 PRIVMSG hihihihi :testme!
     *  받은 사람 양식 -> 보낸사람닉네임!보내사람유저이름@ip PRIVMSG 받는사람닉네임 :보낸 메세지
     */

     // 모든 유저중에 닉네임 값을 가지고 순회 -> 일치하는 UserInfo를 찾아야 한다.
     std::cout << getParameters().at(0) << std::endl;
     if (getReceiverFd() == -1) {
         Communicate::sendToClient(user.getFd(), "없는 닉네임한테 귓속말 금지");
         return;
     }
     Communicate::sendToClient(getReceiverFd(), generateSendFormat());

}

int PrivateMessage::getReceiverFd() {
    std::string receiverName = getParameters().at(0);

    std::map<int, UserInfo>::iterator allUserIter = allUser.begin();
    for (allUserIter; allUserIter != allUser.end(); allUserIter++) {
        if (allUserIter->second.getNickName() == receiverName) {
            return allUserIter->second.getFd();
        }
    }
    return -1;
}
std::string PrivateMessage::generateSendFormat() {
    std::string result = ":" + getParameters().at(0) + "!"
            + user.getUserName() + "@" + "아이피어케넣죠"
            + getCmd() + ":" + getTrailing();
    return result;
}

// 발신자의 포멧 점검
// 1. 닉네임, trailing이 비어있으면 안된다.
bool PrivateMessage::validateFormat() {
    if (getParameters().size() != 1 || getTrailing().empty()) {
        return false;
    }
    return true;
}

bool PrivateMessage::findUserNickName() {
    std::map<int, UserInfo>::iterator it;
    for (it = allUser.begin(); it != allUser.end(); it++) {
        if (it->second.getNickName() == getParameters().at(0) && it->second.getActive()) {
            return true;
        }
    }
    return false;
}