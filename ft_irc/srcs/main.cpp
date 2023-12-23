#include "Server.hpp"

std::vector<std::string> splitByCRLF(std::string &input);
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

                for (size_t i = 1; i < server.pollfds.size(); i++) {
                    if (server.pollfds[i].revents == 0) {
                        std::cout << "읽을 이벤트 없다" << std::endl;
                        continue;
                    }
                    // 이벤트가 존재하지만 끝났거나, 에러가 발생한 케이스라면 서버를 종료합니다.
                    if (server.pollfds[i].revents & POLLHUP || server.pollfds[i].revents & POLLERR) {
                        std::cout << "bye~" << std::endl;
                        server.quitServer(i);
                    } 
                    else if (server.pollfds[i].revents & POLLIN) {
                        std::cout << "이벤트가 존재하고, 데이터도 있네" << std::endl;
                        int fd = server.pollfds[i].fd; // 해당 이벤트를 읽어온다.
                        char buffer[512];

                        memset(buffer, 0, sizeof(buffer));
                        ssize_t recvByte = recv(fd, buffer, sizeof(buffer), 0);

                        if (recvByte < 0) {
                            throw std::runtime_error("Error: Fail read");
                        } 
                        else {
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
                            for (size_t i = 0; i < commands.size(); i++) {
                                std::cout << "보낸 사람의 fd : " << fd << std::endl;
                                std::cout << "입력한 메세지 : " << commands[i] << std::endl;
                                
                                try {
                                    UserInfo &info = server.getUserInfoByFd(fd);
                                    // 유저의 정보, 커맨드를 갖고 cmd를 따온다
                                    Command *cmd = server.createCommand(info, commands[i]);
                                    

                                    // cmd 수행하기
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

/**
 * 왜 하필 CRLF?
 * CR(Carriage Return) \r
 * LF(Line Feed) \n
 * 텍스트 파일, 네트워크 프로토콜(예: HTTP 및 SMTP) 및 텍스트 서식 지정 및 줄 바꿈
 * irssi 규칙을 따라서 개행 및 줄바꿈을 인지
*/
std::vector<std::string> splitByCRLF(std::string &input) {
    std::vector<std::string> res;
    size_t start = 0;
    size_t foundIdx = input.find("\r\n");

    while (foundIdx != std::string::npos) {
        res.push_back(input.substr(start, foundIdx - start));
        start = foundIdx + 2;
        foundIdx = input.find("\r\n", start);
    }
    return res;
}