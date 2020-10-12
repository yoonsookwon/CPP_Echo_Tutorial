#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
    cout << "[오류 발생]: " << message << endl;
    system("pause");
    exit(1);
}

int main()
{

    std::cout << "[TCP Echo Server Ver0.2]\n";
    WSADATA wsaData;

    SOCKET serverSocket, clientSocket;
    SOCKADDR_IN serverAddress, clientAddress;

    int serverPort = 9876;
    char received[256];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을 초기화
        ShowErrorMessage("socket()");

    serverSocket = socket(PF_INET, SOCK_STREAM, 0); // PF_INET -> IPV4, SOCK_STREAM -> TCP형태로 형성하겠다.

    if (serverSocket == INVALID_SOCKET)
        ShowErrorMessage("socket()");

    memset(&serverAddress, 0, sizeof(serverAddress)); // serverAddress 초기화

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); //4바이트 정수를 네트워크 바이트 형식으로 (long)
    serverAddress.sin_port = htons(serverPort); // 2바이트 정수 네트워크 바이트 형식으로 (short)

    if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
        ShowErrorMessage("bind()");
    cout << "[현재 상태] bind()\n";

    if (listen(serverSocket, 5) == SOCKET_ERROR)
        ShowErrorMessage("listen()");
    cout << "[현재 상태] listen()\n";

    int sizeClientAddress = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
    cout << "[현재 상태] accept()\n";

    if (clientSocket == INVALID_SOCKET)
        ShowErrorMessage("accept()");

    while (1)
    {
        int length = recv(clientSocket, received, sizeof(received), 0);
        received[length] = NULL;
        cout << "[클라이언트 메시지]: " << received << endl;
        cout << "[메시지 전송]: " << received << endl;

        if (strcmp(received, "[exit]") == 0)
        {
            send(clientSocket, received, sizeof(received) - 1, 0);
            cout << "[서버 종료]\n";
            break;
        }
        send(clientSocket, received, sizeof(received) - 1, 0);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();


    system("pause");
    return 0;
}