/***************SocketServer*******************/

/*  Add "-lwsock32" (without the quotes) to the linker box under Project->Project Options->linker options.
If you have not set up a project, you will have to do that first. Go to File->New->Project->Empty Project. Then add your files by using Project->Add to Project.
If you want to link with other windows libraries you can use the same syntax. For example, "-lgdi32 -ladvapi32" will link to gdi32.lib and advapi32.lib.
*/
#include "stdio.h"
#include "Windows.h"
#pragma comment (lib,"WS2_32.lib")


main(int argc,char *argv[])
{
	int USERPORT=8000 ;
	char * HOST_IP_ADDR;
	if (argc<2)
 	{
		printf("Please Input the ipaddress of the server(127.0.0.1)Default:202.127.205.185\n");
		scanf("%s",HOST_IP_ADDR);
		if (sizeof(HOST_IP_ADDR)<7) 
			HOST_IP_ADDR="202.127.205.185";
 	}else
	{
		HOST_IP_ADDR=argv[1];
	}
	char buf[128];
	SOCKET s,ns;
	struct sockaddr_in client;
	struct sockaddr_in server;
	int namelen,pklen;
	int status;
	WSADATA wsd;
	printf("The SocketServer is running now--%s\n",HOST_IP_ADDR);
	printf("Waiting for inputs from Client\n");
	
	if((status=WSAStartup(MAKEWORD(2,2),&wsd))!=0)
	{
		perror("wsastartup() failed:");
		exit(1);
	}
	if((s=socket(AF_INET,SOCK_STREAM,0))< 0)
	{
		perror("socket failed :");
		exit(1);
	}
	ZeroMemory(&server,sizeof(server));
	server.sin_family=AF_INET;
	server.sin_port=htons(USERPORT);
	server.sin_addr.s_addr=htons(INADDR_ANY);
	if(bind(s,(struct sockaddr *)&server,sizeof(server))<0)
	{
		perror("bind() failed:");
		exit(2);
	}
	if(listen(s,4)!=0)
	{
		perror("listen()failed :");
		exit(3);
	}
	while(1)
	{
		namelen=sizeof(client);
		if((ns=accept(s, (struct sockaddr*)&client, &namelen))==-1)
		{
			perror("accept()failed:");
			exit(4);
		}
		printf("accept() successful!\n");
		for(;;)
		{
			if((pklen=recv(ns,buf,128,0))<0)
			{
				perror("recv() failed:");
				exit(5);
			}
			else if(pklen==0)
			{
				printf("recv():return FAILED,connected is shut down");
				break;
			}
			else
				printf("recv():return success,packet length=%d context is %s\n",pklen,buf);
			
			Sleep(1);
		
		}
	}
	closesocket(ns);
	closesocket(s);
	printf("server ended successfully\n");
}
