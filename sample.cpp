// sample.cpp: 定义控制台应用程序的入口点。
// 参考资料
// [1] 安川技术文档：“DX200_MotoPlus_Programmers_E05.pdf”
// [2] CSDN：C++ Socket通信总结（附C++实现）https://blog.csdn.net/linghu_java/article/details/43488919

#include "stdafx.h"
#include "winsock2.h"
#include "WS2tcpip.h"

#pragma comment(lib, "ws2_32.lib") // 注意，如果不加这个，会编译错误，出现一堆“无法解析的外部符号”

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

	// 初始化 windows socket library
	WSAStartup(MAKEWORD(2,0), &data);

	// 创建客户端套节字
	sockHandle = socket(AF_INET, SOCK_STREAM, 0);
	// AF_INET 指明使用 TCP/IP 协议族
	// SOCK_STREAM，0 这两个参量具体指明使用的协议
	// 第三个参数 0 - IPPROTO_IP(详细定义参见 winsock.h)
	// 但是在CSDN[2] 中用的是 IPPROTO_TCP - 6
	// 具体不用了解太详细，默认就好
	
	// 错误处理
	if(sockHandle < 0)
	{
		printf("Error:socket() rc = %d\n", sockHandle);
		exit(1);
	}

	memset( &serverSockAddr, 0, sizeof(serverSockAddr) ); //将保存地址的server置为全0
	
	// 这里安川的例程是AF_INET，但是CSDN的例子给的是PF_INET 
	// 参考CSDN：https://blog.csdn.net/xiongmaojiayou/article/details/7584211
	// 二者在windows当中完全一样，所以无所谓
	serverSockAddr.sin_family = AF_INET;
	
	// 原来的例程用了 inet_addr，但是2013以后的VS不认这个函数，改用了 inet_pton 两种函数在调用方法上有所不同，同时需要程序新加入一个头文件“WS2tcpip.h”
	//serverSockAddr.sin_addr.s_addr = inet_addr("192.168.255.1"); // IP address of the DX200(server)
	inet_pton(AF_INET, "192.168.255.1", &(serverSockAddr.sin_addr.s_addr)); // IP address of the DX200(server)

	serverSockAddr.sin_port = htons(PORT); // 连接服务器的端口号，htons()用于converts values between the host and network byte order
	// 其实具体干什么的也不用太理解
	
	rc = connect(sockHandle, 
	(struct sockaddr*)&serverSockAddr,
	sizeof(serverSockAddr));

	if(rc<0)
	{
		printf("Error:connect() rc=%d\n",rc);
		goto closeSockHandle;
	}

	printf(">");
	cntRcvSnd = 0;
	while(1)
	{
		int bytesKeyin;
		int bytesSend;
		int bytesRecv;
		char buff[BUFF_MAX + 1];

		fgets(buff, BUFF_MAX, stdin);

		bytesKeyin = strlen(buff);
		bytesSend = send(sockHandle, buff, bytesKeyin, 0);

		if(bytesSend != bytesKeyin)
		{
			printf("Error:send() rc=%d\n", bytesSend);
			break;
		}

		cntRcvSnd++;
		printf("send(%d)%s", cntRcvSnd, buff);

		bytesRecv = recv(sockHandle, buff, bytesSend, 0);
		if (bytesRecv < 0)
		{
			printf("Error:recv() rc=%d\n", bytesRecv);
			break;
		}

		cntRcvSnd++;
		buff[bytesRecv] = '\0';
		printf("recv(%d) %s", cntRcvSnd, buff);

		if (strncmp(buff,"EXIT",4) == 0)
			break;

		printf(">");
	}

	closeSockHandle:
		closesocket(sockHandle);
		WSACleanup();
		exit(0);
}

