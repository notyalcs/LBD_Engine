#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define BUFLEN 512
#define PORT 2121
char playerNums[] = { '1', '2', '3', '4' };
int playerNum = 0;
std::vector<sockaddr_in> addresses;

int main() {
    SOCKET s;
    struct sockaddr_in server, si_other;
    int slen, recv_len;
    char buf[BUFLEN];
    WSADATA wsa;

    slen = sizeof(si_other);

    printf("\nInitializing Winsock...");

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("Initialized.\n");

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server.sin_family = AF_INET;
    server.sin_addr.S_un.S_addr = INADDR_ANY;
    //inet_pton(AF_INET, SERVER, &(server.sin_addr));
    server.sin_port = htons(PORT);

    printf("%d\n", htons(PORT));

    if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    puts("Bind done");

    while (playerNum < 2) {
        fflush(stdout);

        memset(buf, '\0', BUFLEN);

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        fflush(stdout);

        char checker[] = "Init";

        if (strcmp(checker, buf) == 0) {

            addresses.push_back(si_other);

            memset(buf, '\0', BUFLEN);

            buf[0] = playerNums[playerNum];
            ++playerNum;
            printf("current player num: %c", buf[0]);

            if (sendto(s, buf, recv_len, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR) {
                printf("sendto() failed with error code : %d", WSAGetLastError());
                exit(EXIT_FAILURE);
            }
        }

    }

    while (1) {
        printf("Waiting for data...");
        fflush(stdout);

        memset(buf, '\0', BUFLEN);

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code : %d", WSAGetLastError());
            exit(EXIT_FAILURE);
        }

        //printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
        printf("Data: %s\n", buf);

        for (int i = 0; i < addresses.size(); ++i) {
            if (sendto(s, buf, recv_len, 0, (struct sockaddr*)&addresses.at(i), slen) == SOCKET_ERROR) {
                printf("sendto() failed with error code : %d", WSAGetLastError());
                exit(EXIT_FAILURE);
            }
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;
}