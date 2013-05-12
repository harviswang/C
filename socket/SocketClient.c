/************SocketClient************************/
/*************Add "-lwsock32" (without the quotes) to the linker box under Project->Project Options-linker options.*********************/

#include "stdio.h"
#include "Windows.h"
//#include //this can be replaced by WS2_32.lib
#pragma comment (lib,"WS2_32.lib")
#define USERPORT 8000
main(int argc , char *argv[])
{
	char buf[128];
	SOCKET sclient;
	struct sockaddr_in server;
	int namelen, pklen;
	int status, ret;
	char szmessage[128];
	WSADATA wsd;
	char ch;
	char * ipaddr;
	printf("Please ensure that the SocketServer.exe is running in the server! \n ");
	if (argc < 2)
	{
		printf("Please  input the ip address(127.0.0.1).Default:202.127.205.185\n");
		scanf("%s" , ipaddr);
		if (sizeof(ipaddr) < 7)
			ipaddr = "202.127.205.185";  //this may be replaced by argv[1]
		printf("\n");
	}
	else
		if (argc = 2)
		{
			ipaddr = argv[1];
		}
		else
		{
			perror("params too much!");
			exit(1);
		}
	if ((status = WSAStartup(MAKEWORD(2 , 2) , &wsd)) != 0)
	{
		perror("wsastartup() failed:");
		exit(1);
	}
	if ((sclient = socket(AF_INET , SOCK_STREAM , 0)) < 0)
	{
		perror("socket failed :");
		exit(1);
	}
	ZeroMemory(&server , sizeof(server));
	ZeroMemory(szmessage , sizeof(szmessage));
	server.sin_family = AF_INET;
	server.sin_port = htons(USERPORT);

	server.sin_addr.s_addr = inet_addr(ipaddr);
	if (connect(sclient , (struct sockaddr *) &server , sizeof(server)) < 0)
	{
		printf("%s" , ipaddr);
		perror("connect  failed :");
		printf("%d" , WSAGetLastError( ));

		exit(3);
	}
	else
	{
		printf("%s%s" , ipaddr , " connect OK!\n");
	}
	namelen = sizeof(sclient);

	while (1)
	{

		ZeroMemory(szmessage , sizeof(szmessage));
		printf("Press q to exit or input string(size<128) sent to server: " , szmessage);

		scanf("%s" , szmessage);
		printf("\n");
		if (szmessage == "q")
			break;
		// send(sclient,szmessage,sizeof(szmessage),0);

		if (send(sclient , szmessage , sizeof(szmessage) , 0) < 0)
		{
			perror("send() failed:");
			break;
		}

		printf("send return success!\n context is %s \n" , szmessage);
		Sleep(10);

	}

	closesocket(sclient);
	WSACleanup( );
	printf("server ended successfully!Input any key to exit\n");
	scanf("%c" , &ch);
	return 0;
}
