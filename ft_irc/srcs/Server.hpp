#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <exception>
#include <cstdlib>
#include <poll.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <set>
#include <csignal>
#include "UserInfo.hpp"
#include "Login.hpp"
#include "../command/Command.hpp"
#include "../command/Pass.hpp"
#include "../command/Nick.hpp"
#include "../command/User.hpp"
#include "../command/PrivateMessage.hpp"
#include "../command/Join.hpp"
#include "../command/Part.hpp"
#include "../command/Invite.hpp"
#include "../command/Kick.hpp"
#include "../command/Topic.hpp"
#include "../command/Quit.hpp"
#include "../command/Mode.hpp"
#include "../command/Ping.hpp"
#include "Channel.hpp"

class Server {
private:
    int portNumber;
    std::string password;
    int socketFd;
    std::string serverName;

    int convertPortNumber(char *strPortNumber);
    void openServer();
    void setServerAddress(struct sockaddr_in &serveraddr, int portNumber);
    void pushServerPollfd();

public:
    std::map<int, UserInfo> users;
    std::map<std::string, Channel> channels;
    char clientBuffer[SOMAXCONN][BUFSIZ];
    char sendBuffer[SOMAXCONN][BUFSIZ];
    std::vector<pollfd> pollfds;

    Server(int ac, char **av);

    //getter
    int getPortNumber() const;
    std::string getPassword() const;
    int getSocketFd() const;
    std::string getServerName() const;
    //setter
    void setPortNumber(int portNumber);
    void setPassword(std::string password);
    void setSocketFd(int fd);

    void acceptClient();
    Command *createCommand(UserInfo &user, std::string recvStr);
    void executeCommand(Command *cmd, UserInfo &user);
    void quitServer(int i);

    UserInfo &getUserInfoByFd(int userFd);
};

#endif