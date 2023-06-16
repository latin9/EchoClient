
/*
* connect 함수를 호출을 통한 성능 향상에 대한 에코 클라이언트 코드
*/

// 필요한 헤더 파일들
#include <stdio.h>          // 표준 입력/출력 헤더
#include <stdlib.h>         // 표준 라이브러리 헤더
#include <string.h>         // 문자열 처리 라이브러리
#include <WinSock2.h>       // 윈도우 소켓 2 헤더
#include <WS2tcpip.h>       // WinSock2에 추가 기능을 제공하는 헤더
#pragma comment(lib, "ws2_32.lib")  // WinSock2를 위한 링커 지시문

#define BUFSIZE 30
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET	hSocket;
	char message[30];
	int strLen;

	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf_s("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_DGRAM, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr);


	// connect 함수를 호출해서 UDP 소켓에 클라이언트의 IP와 Port를 할당해 주고 있다.
	// 따라서 데이터를 송 수신하기 위해서 커널이 소켓에 연결되는 과정이 필요하지 않으므로
	// 성능 향상을 기대할 수 있고, 또한 TCP 소켓의 입 출력 함수인 send와 recv를 호출할 수 있다.
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	while (1)
	{
		fputs("전송할 메시지를 입력 하세요 (q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);

		if (!strcmp(message, "q\n"))
			break;

		send(hSocket, message, strlen(message), 0);

		strLen = recv(hSocket, message, sizeof(message) - 1, 0);
		message[strLen] = 0;
		printf_s("서버로부터 전송된 메시지 : %s", message);
	}

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}