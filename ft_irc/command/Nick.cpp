#include "Nick.hpp"

bool Nick::checkForm()
{
    if (getParameters()[0].size() > 10) {
        std::string warning = "432 " + getParameters().at(0) + " :Error nickname";

        //에러 메세지 user의 fd값으로 보내는 함수 작성하기
        Communicate::sendToClient(user.getFd(), warning);
        return false;
    }
    if (isDuplicateNickName()) {
        return false;
    }
    return true;
}

Nick::Nick(Message *msg, UserInfo &userInfo, std::map<int, UserInfo> &infoOfUser, std::map<std::string, Channel> &channels) : Command(msg), user(userInfo), allUserInfo(infoOfUser), channels(channels) {

}

/**
 * 1. 모든 채널을 순회한다
 * 2. 채널 내에서 users를 순회한다.
 * 3. nickName과 일치한다면, 해당 Channel을 반환한다.
*/
bool Nick::isInChannel() {
    std::map<std::string, Channel>::iterator it = channels.begin();

    // 모든 채널 순회
    for (; it != channels.end(); it++) {
        // 채널 내부의 users 중, nickName과 일치하는게 있다면, 해당 NickName은 채널에 속한 유저이다.
        if (it->second.users.find(user.getNickName()) != it->second.users.end()) {
            return true;
        }
    }
    return false;
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
    if (!user.getPass()) {
        Communicate::sendToClient(user.getFd(), "비밀번호 입력이 되어있지 않습니다!");
        return;
    }

    if (getParameters().size() < 1) {
        Communicate::sendMessage(user, "431", getCmd(), "No nickname given");
        return ;
    }
    
    if (user.getActive()) {
        if (!isDuplicateNickName()) {
            // 기존 유저의 닉네임 변경 시 setNick -> true 변환.
            if (isInChannel()) {
                updateChannelUser(getParameters().at(0));
            }
            user.setNick(true);
            user.setNickName(getParameters().at(0));
            Communicate::sendToClient(user.getFd(), "new NickName Set clear!");
            return ;
        }
        Communicate::sendMessage(user, "433", getCmd(), "Nickname is already in use");
        return ;
    }

    if (checkForm()) {
        updateUserNickName();
    }
}

// channels의 channel의 users를 업데이트..;;
void Nick::updateChannelUser(std::string newNickName) {
    std::map<std::string, Channel>::iterator it = channels.begin();

    // 모든 채널 순회
    for (; it != channels.end(); it++) {
        Channel &channel = it->second;
        // 채널 내부의 users 중, nickName과 일치하는게 있다면, 해당 NickName은 채널에 속한 유저이다.
        std::string oldName = user.getNickName();
        std::map<std::string, UserInfo>::iterator userIt = channel.users.find(user.getNickName());
        if (userIt != channel.users.end()) {
            UserInfo user = userIt->second;
            user.setNickName(newNickName);
            channel.users.erase(oldName);

            // 기존의 채널 내의 user 정보를 삭제하고, 새로운 닉네임과 함께 정보를 업데이트합니다.
            channel.users.insert(std::make_pair(newNickName, user));
        }

        std::map<std::string, UserInfo>::iterator operatorIt = channel.operators.find(oldName);
        if (operatorIt != channel.operators.end()) {
            UserInfo user = userIt->second;
            user.setNickName(newNickName);
            channel.operators.erase(oldName);
            // 기존의 채널 내의 user 정보를 삭제하고, 새로운 닉네임과 함께 정보를 업데이트합니다.
            channel.operators.insert(std::make_pair(newNickName, user));
        }

        std::map<std::string, UserInfo>::iterator inviteIt = channel.invite.find(oldName);
        if (inviteIt != channel.invite.end()) {
            UserInfo user = userIt->second;
            user.setNickName(newNickName);
            channel.invite.erase(oldName);
            // 기존의 채널 내의 user 정보를 삭제하고, 새로운 닉네임과 함께 정보를 업데이트합니다.
            channel.invite.insert(std::make_pair(newNickName, user));
        }
    }
}

void Nick::updateUserNickName() {
    std::string newNickName = getParameters()[0];
    user.setNickName(newNickName);

    std::map<int, UserInfo>::iterator it = allUserInfo.find(user.getFd());
    if (it != allUserInfo.end()) {
        it->second.setNickName(newNickName);
    }
}

bool Nick::isDuplicateNickName() {
    std::map<int, UserInfo>::iterator it;
    for (it = allUserInfo.begin(); it != allUserInfo.end(); it++) {
        // 입력한 닉네임과, 기존에 로그인이 성공한 닉네임이 겹친다면..
        if ((it->second.getNickName() == getParameters().at(0)) && it->second.getActive()) {
            std::string result = ":" + user.getServerName() + " 436 " + user.getUserName() + ":" + getCmd() + ":Nickname collision";
            Communicate::sendToClient(user.getFd(), result);
            return true;
        }
    }
    return false;

}
