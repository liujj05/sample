// sample.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#define PORT 11000 // Port number specified by the server side (Robot side)
#define BUFF_MAX 256

int main()
{
	SOCKET sockHandle;
	struct sockaddr_in serverSockAddr;
	int cntRcvSnd;
	WSADATA data;
	int rc;

	printf("Simple TCP client\n");
	printf("Please type the ASCII characters\n");
    return 0;
}

