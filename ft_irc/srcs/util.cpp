#include "UserInfo.hpp"

UserInfo getByNickName(int clientFd, std::map<int, UserInfo> allUsers) {
    std::map<int, UserInfo>::iterator it = allUsers.find(clientFd);

    if (it !=allUsers.end()) {
        return it -> second;
    }
}