#include "Server.hpp"

Server::Server(int ac, char **av) : serverName("saewoo") {
    if (ac != 3) {
        throw std::invalid_argument("Error invalid ac count");
    }

    portNumber = convertPortNumber(av[1]);
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
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        throw std::runtime_error("Error: socket error");
    }
    setSocketFd(socketFd);

    try {
        struct sockaddr_in serverAddr;
        setServerAddress(serverAddr, getPortNumber());

        if (bind(getSocketFd(), reinterpret_cast<const struct sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1) {
            throw std::runtime_error("Error: bind error");
        }

        if (listen(getSocketFd(), SOMAXCONN) == -1) {
            throw std::runtime_error("Error: listen error");
        }
    } catch (const std::exception &e) {
        close(getSocketFd());

        throw std::runtime_error(e.what());
    }
}

void Server::setServerAddress(struct sockaddr_in &serverAddr, int port) {
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
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