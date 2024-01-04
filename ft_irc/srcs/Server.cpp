#include "Server.hpp"
#include "Channel.hpp"

/**
 * 포트 번호, 비밀번호로 실행 시작
*/
Server::Server(int ac, char **av) : serverName("BlackCowServer...") {
    if (ac != 3) {
        throw std::invalid_argument("Error invalid ac count");
    }

    // 포트 번호가 숫자인지, 0 ~ 65535 사이인지 검수
    portNumber = convertPortNumber(av[1]);
    // 비밀번호 저장
    std::string password(av[2]);
    this->password = password;

    openServer();
    pushServerPollfd();
}

int Server::convertPortNumber(char *portNumber) {
    for (size_t i = 0; i < strlen(portNumber); i++) {
        if (!isdigit(portNumber[i])) {
            throw std::invalid_argument("Error: 포트 번호는 숫자만 입력해주세요.");
        }
    }
    int port = atoi(portNumber);

    if (port < 0 || 65535  < port) {
        throw std::invalid_argument("Error: 포트 번호는 0 ~ 65535 사이로 입력해주세요.");
    }

    return port;
}

void Server::openServer() {
    // 소켓 열고, 서버에 fd값 넣어주기
    /**
     * 
     * socket(IP버전, type, protocol)
     * IPv4로 지정, TCP, TCP type으로 socket 생성 -> 통신의 주체가 되는 fd 생성
     * TCP protocl 특징
     * 1. 연결 지향성 (3핸쉑)
     * 2. 순서 보장
     * 3. 전달 보장 (너 클라한테까지 도달 해씀?)
    */
    int socketFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1) {
        throw std::runtime_error("Error: socket error");
    }
    // 만든 소켓 fd값 저장
    setSocketFd(socketFd);


    try {
        struct sockaddr_in serverAddr;
        setServerAddress(serverAddr, getPortNumber());

        /**
         * bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
         * 소켓에 인터넷 주소를 할당한다.
         * sockfd : 인터넷 주소(IP, PORT)를 할당할 소켓의 fd
         * myaddr : 할당하려는 인터넷 주소 구조체
         * bind 성공 시 sockfd에 해당하는 소켓에 정보가 할당된다. 실패 시 -1
        */
        if (bind(getSocketFd(), reinterpret_cast<const struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
            throw std::runtime_error("Error: bind error");
        }

        /**
         * listen(int socketfd, int backlog)
         * 연결 요청 대기상태에 두고자하는 소켓의 fd
         * backlog : 연결요청 대기 큐의 크기 정보. SOMAXCONN은 최대길이. 128
         * 성공 시 이제 이 server의 sockfd값은 일반 소켓이 아니라 서버 소켓. 이벤트가 발생하길 기다리고 있다.
        */
        if (listen(getSocketFd(), SOMAXCONN) == -1) {
            throw std::runtime_error("Error: listen error");
        }
    } catch (const std::exception &e) {
        close(getSocketFd());

        throw std::runtime_error(e.what());
    }
}

    /**
     * setter 부분에서 초기화 진행
     * 
     * sockaddr_in 구조체
     * sin_family : 주소 체계
     * sin_port : 16비트 TCP PORT 번호 -> 16비트 port 번호를 네트워크 바이트 순서대로 저장해야한다.
     * sin_addr : 32비트 IP 주소 -> 32비트 IP 정보 저장
    */
void Server::setServerAddress(struct sockaddr_in &serverAddr, int port) {
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 서버의 IP 주소를 자동으로 할당(INADDR_ANY)
    serverAddr.sin_port = htons(port);
}

void Server::pushServerPollfd() {
    pollfd pollfd;
    pollfd.fd = getSocketFd();
    pollfd.events = POLLIN;
    pollfds.push_back(pollfd);
}

std::string Server::getPassword() const {
	return password;
}

int Server::getPortNumber() const {
	return portNumber;
}

int Server::getSocketFd() const {
	return socketFd;
}

std::string Server::getServerName() const {
	return serverName;
}

void Server::setPassword(std::string password) {
	this->password = password;
}

void Server::setPortNumber(int portNum) {
	this->portNumber = portNum;
}

void Server::setSocketFd(int fd) {
	this->socketFd = fd;
}

void Server::acceptClient() {
    sockaddr_in client;
    socklen_t clientlen = sizeof(client);

    /**
     * getSockFd -> 서버 소켓의 fd값 받아오기.
     * 
     * accept(int socketFd, clientAddr, clientAddrSize)
     * 접속 요청 수락, 요청 마무리하기 전까지(요청이 들어오기 전까지) 계속 대기상태
     * 성공 시 연결된 소켓이 만들어진다.
     * clientAddr : accept 할 클라이언트측 주소정보 구조체 주소
    */
    int clientSocketFd = accept(getSocketFd(), reinterpret_cast<sockaddr *>(&client), &clientlen);

    // Map<Integer, UserInfo> users
    /**
     * 연결된 소켓, 유저 정보 key-value
     * 
    */
    users.insert(std::make_pair(clientSocketFd, UserInfo()));
    //UserInfo 객체 내부에 clientSocketFd 저장
    users[clientSocketFd].setFd(clientSocketFd);

    /**
     * 구조체 내부 멤버들로는
     * int fd, short events, short revents
     * newPollfd 선언, fd, events 값 세팅
     * 클라이언트 소켓을 모니터링하며, POLLIN(읽을 수 있는 이벤트들) 수행하도록 구조체를 초기화하고,
     * 이를 Server 클래스의 멤버인 pollfds 에 추가했슴
     * POLLIN?
     *  지정된 fd에서 데이터를 읽을 수 있는 이벤트를 나타내는 상수.
     *  pollfd 객체에서 clientfd를 모니터링하며, 읽을 수 있는 이벤트들을 받아들인다.
     *  poll 반환 시, revents 멤버를 확인하여 각 fd에서 어떤 이벤트가 발생했는지 확인할 수 있게 된다.
     *  revents에 POLLIN이 포함되어 있다면, 해당 소켓에서 읽을 준비가 된 데이터가 존재한다는 의미.
     *  if (pollfds.revents && POLLIN) 요런 상태.
     * 
     * 서버에서 동시에 여러가지의 소켓의 활동을 기다리고, 서버가 여러 클라이언트를 효율적으로 처리할 수 있도록
     * non-blocking 방식으로 수행
    */
    pollfd newPollfd;
    newPollfd.fd = clientSocketFd;
    newPollfd.events = POLLIN;
    pollfds.push_back(newPollfd);

    std::cout << "new client fd: " << clientSocketFd << std::endl;
}

Command *Server::createCommand(UserInfo &user, std::string recvStr) {
    Message msg(user.getFd(), recvStr);
    Command *cmd = 0;
    if (msg.getCmd() == "PASS")
        cmd = new Pass(&msg, user, password);
    else if (msg.getCmd() == "NICK")
        cmd = new Nick(&msg, user, users, channels);
    else if (msg.getCmd() == "USER")
        cmd = new User(&msg, user);
    else if (msg.getCmd() == "PRIVMSG")
        cmd = new PrivateMessage(&msg, user, users, channels);
    else if (msg.getCmd() == "JOIN")
        cmd = new Join(&msg, user, channels);
    else if (msg.getCmd() == "PART")
        cmd = new Part(&msg, user, users, channels);
    else if (msg.getCmd() == "INVITE")
        cmd = new Invite(&msg, user, channels, users);
    else if (msg.getCmd() == "KICK")
        cmd = new Kick(&msg, user, users, channels);
    else if (msg.getCmd() == "TOPIC")
        cmd = new Topic(&msg, user, channels);
    else if (msg.getCmd() == "QUIT")
        cmd = new Quit(&msg, user, channels, users, pollfds);
    else if (msg.getCmd() == "MODE")
        cmd = new Mode(&msg, user, users, channels);
    else if (msg.getCmd() == "PING")
        cmd = new Ping(&msg, user);
    // else
    //     Communicate::sendMessage(user, "421", msg.getCmd(), "Unknown command");
    return cmd;

}

void Server::executeCommand(Command *cmd, UserInfo &info) {
    if (cmd) {
        cmd->execute();

        if (!info.getActive() && (cmd->getCmd() == "NICK" || cmd->getCmd() == "USER")) {
            Login(info, users, getServerName());
        }

        delete(cmd);
    }
}

UserInfo &Server::getUserInfoByFd(int clientSocketFd) {
    std::map<int, UserInfo>::iterator it = users.find(clientSocketFd);

    if (it != users.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Error: Not find any user");
    }
    return it->second;
}

void Server::quitServer(int i) {
    UserInfo &info = getUserInfoByFd(pollfds[i].fd);
    std::map<std::string, bool>::iterator it = info.channels.begin(); // 모든 채널들

    for (; it != info.channels.end(); it++) {
        std::string channelName = it->first;
        // 서버 내에 존재하는 채널명 하나 가져오기
        std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);

        if (channelIt != channels.end()) {
            // 채널 객체 가져오기
            Channel &channel = channelIt->second;
            // 채널에서 사용자 제거
            channel.users.erase(info.getNickName());
            channel.operators.erase(info.getNickName());
            channel.invite.erase(info.getNickName());

            // 다른 사용자들에게 사용자가 나갔음을 알림
            std::map<std::string, UserInfo>::iterator userIt = channel.users.begin();
            for (; userIt != channel.users.end(); userIt++) {
                UserInfo &userInChatRoom = userIt->second;

                if (userInChatRoom.getFd() == info.getFd()) {
                    continue; // 나가는 사용자는 건너뜀
                }

                // 다른 사용자에게 사용자가 나갔음을 알림
                std::string bye = ":" + info.getNickName() + "!" + info.getUserName() + "@" + info.getServerName() + " QUIT :Quit: leaving";
                Communicate::sendToClient(userInChatRoom.getFd(), bye);
            }
        }
    }
    info.setActive(false);
    users.erase(info.getFd());
    close(info.getFd());
    pollfds.erase(pollfds.begin() + i);
}