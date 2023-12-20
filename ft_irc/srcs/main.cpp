#include "Server.hpp"

int main(int ac, char **av) {
    try {
        /**
         * 포트 열고, pollfd값 등록하기
        */
        Server server(ac, av);
        std::cout << server.getPortNumber() << ", " << server.getPassword() << std::endl;

        while (1) {
            int resultFd = poll(server.pollfds.data(), server.pollfds.size(), 36000);
            if (resultFd == -1) {
                throw std::runtime_error("Error: poll error");
            }

            if (resultFd > 0) {
                if (server.pollfds[0].revents && POLLIN) {
                    std::cout << "client -> server connect" << std::endl;
                    server.acceptClient();
                    continue;
                }

                for (int i = 1; i < server.pollfds.size(); i++) {
                    if (server.pollfds[i].revents == 0) {
                        continue; // 이벤트 없음
                    }
                    if (server.pollfds[i].revents && POLLHUP || server.pollfds[i].revents && POLLERR) {
                        quiteServer(server, i);
                        std::cout << "quit client\n";
                    } else if (server.pollfds[i].revents && POLLIN) {
                        int fd = server.pollfds[i].fd;
                        char buffer[512];

                        memset(buffer, 0, sizeof(buffer));
                        ssize_t recvByte = recv(fd, buffer, sizeof(buffer), 0);

                        if (recvByte < 0) {
                            throw std::runtime_error("Error: Fail read");
                        } else {
                            std::strcat(server.clientBuffer[fd], buffer);
                            std::string recvStr(server.clientBuffer[fd]);

                            if (recvStr.find("\r\n") == std::string::npos) {
                                continue;
                            }
                        }
                    }
                }
            }
        }
    }

}