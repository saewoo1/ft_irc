#include "Server.hpp"

/**
 * 포트 번호, 비밀번호로 실행 시작
*/
Server::Server(int ac, char **av) : serverName("saewoo") {
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
    for (int i = 0; i < strlen(portNumber); i++) {
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

    int clientFd = accept(getSocketFd(), reinterpret_cast<sockaddr *>(&client), &clientlen);

    users.insert(std::make_pair(clientFd, UserInfo()));
    users[clientFd].setFd(clientFd);

    pollfd newPollfd;
    newPollfd.fd = clientFd;
    newPollfd.events = POLLIN;
    pollfds.push_back(newPollfd);

    std::cout << "new client fd: " << clientFd << std::endl;
}

void Server::executeCommand(Command *cmd, UserInfo &info) {
    if (cmd) {
        cmd->execute();

    }
}

UserInfo &Server::getUserInfoByFd(int clientFd) {
    std::map<int, UserInfo>::iterator it = users.find(clientFd);

    if (it != users.end()) {
        return it->second;
    } else {
        throw std::runtime_error("Error: Not find any user");
    }
    return it->second;
}