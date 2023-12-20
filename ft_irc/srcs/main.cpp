#include "Server.hpp"

int main(int ac, char **av) {
    try {
        /**
         * 포트 열고, pollfd값 등록하기
         * 기본적으로 portnumber, password를 받아서 server를 생성합니다.
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

                            // crlf("\r\n")이 아닐 경우, 문자열만 저장해준다
                            if (recvStr.find("\r\n") == std::string::npos) {
                                continue;
                            }

                            std::vector<std::string> commands = splitByCRLF(recvStr);
                            std::string strBuffer(server.clientBuffer[fd]);
                            std::size_t lastCRLFPos = strBuffer.rfind("\r\n");

                            if (lastCRLFPos != std::string::npos) {
                                strBuffer.erase(0, lastCRLFPos + 2);
                            }
                            for (int i = 0; i < commands.size(); i++) {
                                std::cout << "보낸 사람의 fd : " << fd << std::endl;
                                std::cout << "입력한 메세지 : " << commands[i] << std::endl;
                                
                                try {
                                    UserInfo &info = server.getUserInfoByFd(fd);
                                    Command *cmd = server.createCommand(info, commands[i]);

                                    server.executeCommand(cmd, info);
                                } catch (const std::exception &e) {
                                    std::cerr << e.what() << std::endl;
                                    continue;
                                }
                            }
                            std::strcpy(server.clientBuffer[fd], strBuffer.c_str());
                        }
                    }
                }
            }
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

std::vector<std::string> splitByCRLF(std::string &input) {
    std::vector<std::string> res;
    int start = 0;
    int foundIdx = input.find("\r\n");

    while (foundIdx != std::string::npos) {
        res.push_back(input.substr(start, foundIdx - start));
        start = foundIdx + 2;
        foundIdx = input.find("\r\n", start);
    }
}

void quiteServer(Server &server, int i) {
    UserInfo &info = server.getUserInfoByFd(server.pollfds[i].fd);
    std::map<std::string, bool>::iterator it = info.channels.begin();

    for (; it != info.channels.end(); it++) {

    }
}