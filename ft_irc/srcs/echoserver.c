#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 512

void error_handling(char* message) {
    fputs(message, stderr); // 표준 에러 스트림에 메세지 출력
    fputc('\n', stderr); // 개행문자 하나 출력
    exit(1);
}

int main(int argc, char **argv) {
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    // TCP 연결, IPv6 도메인을 위한 소켓 생성
    /**
     * socket
     * param -> domain, type, protocol
     * domain -> AF_UNIX(로컬에서 프로세스끼리), AF_INET(IPv4 주소체계), AF_INET6(IPv6 주소체계)
     * type -> SOCK_STREAM(TCP), SOCK_DGRAM(UDP), SOCK_RAW(네트워크 및 전송 계층 헤더를 임의 조작)
     * protocol -> IPPROTO_TCP(TCP), IPPROTO_UDP(UPD), 0(type 에서 지정한 값을 사용한다)
     * 
     * serv_sock의 의미는 IPv6 주소체계를 사용하고, TCP 소켓 형식을 사용하며, 프로토콜은 TCP(type)의 값을 사용한다.
     * */
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1) {
        error_handling("socket error!");
    }

    // 서버 주소 정보설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // 서버의 주소 패밀리 동일하게 IPv6
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); // 서버의 ip 주소 다 열어
    serv_addr.sin_port = htons(atoi(argv[1])); // 서버 프로그램의 포트

    // 소켓과 주소 정보를 결합한다.
    /**
     * int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
     * sockfd -> 소켓의 fd값
     * myaddr -> 서버의 주소 정보
     * addrlen -> 서버 주소 정보의 길이
     * 
    */
    // int option = 1;
    // setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind error!!");
    }

    if (listen(serv_sock, 5) == -1) {
        error_handling("listen error");
    }

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1) {
        error_handling("accept error");
    }

    while (1) {
        char* buf[BUF_SIZE];

        if (read(clnt_sock, buf, BUF_SIZE) <= 0) {
            close(clnt_sock);
            printf("client disconnected\n");
            break;
        } else {
            write(clnt_sock, buf, strlen((const char*)buf));
        }
        memset(buf, 0, BUF_SIZE);
    }
    close(serv_sock);

    return 0;
}