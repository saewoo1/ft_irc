#include "Server.hpp"

Server::Server(int ac, char **av) : serverName("saewoo") {
    if (ac != 3) {
        throw std::invalid_argument("Error: 포트 번호와 비밀번호만 입력해주세요.");
    }

     // port 번호, 비밀 번호를 입력 받는다.
    portNumber = convertPort(av[1]);
    std::string password(av[2]);
    this->password = password;

    openServer();
    pushServerPollfd();
}

// str -> int 포트 번호 파싱
int Server::convertPort(char *strPortNumber) {
    for (int i = 0; i < strlen(strPortNumber); i++ ){
        if (!std::isdigit(strPortNumber[i])) {
            throw std::invalid_argument("Error: 포트 번호는 숫자만 입력해주세요.");
        }
    }

    int port = atoi(strPortNumber);
    if (port < 0 || 65535 < port) {
        throw std::invalid_argument("Error: 포트 번호는 0 ~ 65535 사이로 입력해주세요.");
    }
    return port;
}

void Server::openServer() {
    createSocket();

    try {

    }
}


// TCP 연결, IPv6 도메인을 위한 소켓 생성
/**
 * socket
 * param -> domain, type, protocol
 * domain -> AF_UNIX(로컬에서 프로세스끼리), AF_INET(IPv4 주소체계), AF_INET6(IPv6 주소체계)
 * type -> SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW(네트워크 및 전송 계층 헤더를 임의 조작)
 * protocol -> IPPROTO_TCP(TCP), IPPROTO_UDP(UPD), 0(type 에서 지정한 값을 사용한다)
 * 
 * socketFd 의미는 IPv4 주소체계를 사용하고, TCP 소켓 형식을 사용하며, 프로토콜은 TCP(type)의 값을 사용한다.
 * */
void Server::createSocket() {
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);

    if (socketFd == -1) {
        throw std::runtime_error("createSocket fail...");
    }
    setSocketFd(socketFd);
}

void Server::setServerAddress(struct sockaddr_in &serveraddr, int portNumber) {
    std::memset(&serveraddr, 0, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소 long int -> byte order
    serveraddr.sin_port = portNumber;
}

void Server::pushServerPollfd() {
    pollfd pollfd;

    pollfd.fd = getSocketFd();
    pollfd.events = POLLIN;
    pollfds.push_back(pollfd);
}

std::string Server::getPassword() const
{
	return password;
}

int Server::getPortNumber() const
{
	return portNumber;
}

int Server::getSocketFd() const
{
	return socketFd;
}
std::string Server::getServerName() const
{
	return serverName;
}

void Server::setPassword(std::string password)
{
	this->password = password;
}

void Server::setPortNumber(int portNum)
{
	this->portNumber = portNum;
}

void Server::setSocketFd(int fd)
{
	this->socketFd = fd;
}

void Server::acceptClient() {
    sockaddr_in client;
    socklen_t clientLen = sizeof(client);

    int clientFd = accept(getSocketFd(), reinterpret_cast<sockaddr *>(&client), &clientLen);

    users.insert(std::make_pair(clientFd, UserInfo()));
    users[clientFd].setFd(clientFd);
}