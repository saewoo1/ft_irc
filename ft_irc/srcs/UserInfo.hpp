#ifndef USERINFO_HPP
#define USERINFO_HPP

#include <iostream>
#include <string>
#include <map>

class UserInfo {
    private:
        int fd;
        std::string nickName;
        std::string userName;
        std::string hostName;
        std::string serverName;
        std::string realName;

        bool pass;
        bool nick;
        bool user;
        bool active;

        std::string sendBuffer;

    public:
        std::map<std::string, bool> channels;

        UserInfo();
        void setFd(int fd);
        void setNickName(std::string nickName);
        void setUserName(std::string userName);
        void setHostName(std::string hostName);
        void setRealName(std::string realName);
        void setServerName(std::string serverName);

        int getFd() const;
        std::string getNickName() const;
        std::string getUserName() const;
        std::string getHostName() const;
        std::string getServerName() const;
        std::string getRealName() const;

        bool getPass() const;
        bool getNick() const;
        bool getUser() const;
        bool getActive() const;

        void checkPass();
        void checkNick();
        void checkUser();
        void checkActive();
        
};

#endif