#include "PrivateMessage.hpp"


/**
 * 진행 순서
 * 1. 채널, 개인 사용자인지 파싱
 * 2. 채널, 개인 사용자별 포맷팅 검증
 * 3. 채널 -> 채널 내의 모든 사용자에게 순회하며 fd값 받고, 메시지 전달
 * 4. 개인 -> 한명의 fd 값에 메세지 전달
*/
PrivateMessage::PrivateMessage(Message *msg, UserInfo &user, std::map<int, UserInfo> allUserInfo, std::map<std::string, Channel> allChannels) : Command(msg), user(user), allUser(allUserInfo), allChannels(allChannels) {
    // 메세지 포매팅만 검증한다.
    if (!validateFormat()) {
        std::string result = "수신 메세지 형식이 올바르지 않습니다.";
        Communicate::sendToClient(user.getFd(), result);
        return;
    }
}

bool PrivateMessage::isChannelMsg() {
    if (getParameters().at(0)[0] == '#') {
        return true;
    }
    return false;
}

// 진짜 존재하는 채널인지, 채널 내부에 1명 이상의 인원이 있는지

bool PrivateMessage::validateChannelMsg() {
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = allChannels.find(channelName);

    // 채널이 존재하고, 해당 채널 내부에 유저 수가 1명 이상이라면 true를 반환한다.
    return (it != allChannels.end()) && (it->second.getUserCount() != 0);
}

void PrivateMessage::sendChannelMsg() {
    std::string channelName = getParameters().at(0);
    std::map<std::string, Channel>::iterator it = allChannels.find(channelName);

    Channel channel = it->second; // 해당 채널을 찾아온다.
    std::map<std::string, UserInfo> usersInChannel = channel.users; // 채널 내의 모든 유저들을 찾아온다.
    std::map<std::string, UserInfo>::iterator userIt = usersInChannel.begin();
    for (; userIt != usersInChannel.end(); userIt++) {
        Communicate::sendToClient(userIt->second.getFd(), generateSendFormat()); // 모든 유저들에게 메세지를 전송한다.
    }
}

void PrivateMessage::execute() {
    if (isChannelMsg() && validateChannelMsg()) {
        std::cout << "채널 내의 모든 유저에게 메세지 전송을 시작합니다" << std::endl;
        sendChannelMsg(); // 채널 내의 모든 사용자에게 메세지를 전달한다
        return ;
    }
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
     if (getReceiverFd() == -1) {
         Communicate::sendToClient(user.getFd(), "없는 닉네임한테 귓속말 금지");
         return;
     }
    Communicate::sendToClient(getReceiverFd(), generateSendFormat());

}

int PrivateMessage::getReceiverFd() {
    std::string receiverName = getParameters().at(0);

    std::map<int, UserInfo>::iterator allUserIter = allUser.begin();
    for (; allUserIter != allUser.end(); allUserIter++) {
        if (allUserIter->second.getNickName() == receiverName) {
            return allUserIter->second.getFd();
        }
    }
    return -1;
}
std::string PrivateMessage::generateSendFormat() {
    std::string result = ":" + getParameters().at(0) + "!"
            + user.getNickName() + "@" + user.getHostName()
            + getCmd() + ":" + getTrailing();
    return result;
}

// 발신자의 포멧 점검
// 1. 닉네임, trailing이 비어있으면 안된다.
// 닉네임 -> PRVMSG receiverName :message
// 채널 -> 1. PRIVMSG #channelName :message
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