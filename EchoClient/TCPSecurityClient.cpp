
// �ʿ��� ��� ���ϵ�
#include <stdio.h>          // ǥ�� �Է�/��� ���
#include <stdlib.h>         // ǥ�� ���̺귯�� ���
#include <string.h>         // ���ڿ� ó�� ���̺귯��
#include <WinSock2.h>       // ������ ���� 2 ���
#include <WS2tcpip.h>       // WinSock2�� �߰� ����� �����ϴ� ���
#pragma comment(lib, "ws2_32.lib")  // WinSock2�� ���� ��Ŀ ���ù�


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
		fputs("������ �޽����� �Է��ϼ��� (q to quit) : ", stdout);
		fgets(message, BUFSIZE, stdin);	// ������ ������ �ַܼκ��� �Է�

		if (!strcmp(message, "q\n"))
			break;

		strLen = send(hSocket, message, strlen(message), 0);	// �޽��� ����
		
		for (recvLen = 0; recvLen < strLen;)
		{
			recvNum = recv(hSocket, &message[recvLen], strLen - recvLen, 0);

			if (recvNum == -1)
				ErrorHandling("recv() error");

			recvLen += recvNum;
		}

		message[strLen] = 0;
		printf_s("�����κ��� ���۵� �޽��� : %s \n", message);
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