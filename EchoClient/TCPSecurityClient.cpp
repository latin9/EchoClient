
// 필요한 헤더 파일들
#include <stdio.h>          // 표준 입력/출력 헤더
#include <stdlib.h>         // 표준 라이브러리 헤더
#include <string.h>         // 문자열 처리 라이브러리
#include <WinSock2.h>       // 윈도우 소켓 2 헤더
#include <WS2tcpip.h>       // WinSock2에 추가 기능을 제공하는 헤더
#pragma comment(lib, "ws2_32.lib")  // WinSock2를 위한 링커 지시문


#define BUFSIZE 1024
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	char message[BUFSIZE];
	int strLen, recvLen, recvNum;

	SOCKADDR_IN servAddr;

	if (argc != 3)
	{
		printf_s("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(atoi(argv[2]));
	inet_pton(AF_INET, argv[1], &servAddr.sin_addr);

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");

	while (1)
	{
		fputs("전송할 메시지를 입력하세요 (q to quit) : ", stdout);
		fgets(message, BUFSIZE, stdin);	// 전송할 데이터 콘솔로부터 입력

		if (!strcmp(message, "q\n"))
			break;

		strLen = send(hSocket, message, strlen(message), 0);	// 메시지 전송
		
		for (recvLen = 0; recvLen < strLen;)
		{
			recvNum = recv(hSocket, &message[recvLen], strLen - recvLen, 0);

			if (recvNum == -1)
				ErrorHandling("recv() error");

			recvLen += recvNum;
		}

		message[strLen] = 0;
		printf_s("서버로부터 전송된 메시지 : %s \n", message);
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