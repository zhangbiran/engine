#include <iostream>
using namespace std;

#include <cstdio>
#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")      // Socket编程需用的动态链接库
#pragma comment(lib, "Kernel32.lib")    // IOCP需要用到的动态链接库

#define PORT 5150
#define DATA_BUFSIZE 8192

DWORD WINAPI ServerWorkerThread(LPVOID ComlpetionPortID);


typedef struct
{
	OVERLAPPED OVerlapped;
	WSABUF DATABuf;
	CHAR Buffer[DATA_BUFSIZE];
	DWORD BytesSend, BytesRecv;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;


typedef struct
{
	SOCKET Socket;
}PER_HANDLE_DATA, *LPPER_HANDLE_DATA;


DWORD WINAPI ServerWorkerThread(LPVOID ComlpetionPortID);


int main(int argc, char* argv[])
{
	SOCKADDR_IN InternetAddr;
	SOCKET  Listen, Accept;
	HANDLE  CompetionPort;
	SYSTEM_INFO SystenInfo;
	LPPER_HANDLE_DATA   PerHandleData;
	LPPER_IO_OPERATION_DATA PerIOData;
	int i;
	DWORD   RecvBytes;
	DWORD   Flags;
	DWORD   ThreadID;
	WSADATA     wsadata;
	DWORD   Ret;
	if (Ret = WSAStartup(0x2020, &wsadata) != 0)
	{
		printf("WSAStartup failed with error %d/n", Ret);
		return 0;
	}

	//打开一个空的完成端口
	if ((CompetionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort failed with error %d/n", GetLastError());
		return 0;
	}

	GetSystemInfo(&SystenInfo);
	// 开启cpu个数的2倍个的线程
	for (i = 0; i < SystenInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE  ThreadHandle;

		//创建服务器工作线程，并且向线程传送完成端口
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompetionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread failed with error %d/n", GetLastError());
			return 0;
		}
		CloseHandle(ThreadHandle);
	}


	//打开一个服务器socket
	if ((Listen = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
	{
		printf("WSASocket()failed with error %d/n", WSAGetLastError());
		return 0;
	}
	InternetAddr.sin_family = AF_INET;
	InternetAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	InternetAddr.sin_port = htons(PORT);

	if (bind(Listen, (LPSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
	{
		printf("bind failed with error %d/n", WSAGetLastError());
		return 0;
	}

	if (listen(Listen, 5) == SOCKET_ERROR)
	{
		printf("listen failed with error %d/n", WSAGetLastError());
		return 0;
	}

// 开始处理IO数据
	cout << "iocp2 本服务器已准备就绪，正在等待客户端的接入...\n";
	//接收连接并且分发给完成端口
	while (TRUE)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept failed with error %d/n", WSAGetLastError());
			return 0;
		}

		//创建与套接字相关的套接字信息结构
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
		{
			printf("GlobalAlloc failed with error %d/n", GetLastError());
			return 0;
		}

		// Associate the accepted socket with the original completion port.
		printf("Socket number %d connected/n", Accept);

		PerHandleData->Socket = Accept;//结构中存入接收的套接字
		//与我们的创建的那个完成端口关联起来,将关键项也与指定的一个完成端口关联
		if ((CreateIoCompletionPort((HANDLE)Accept, CompetionPort, (DWORD)PerHandleData, 0)) == NULL)
		{
			printf("CreateIoCompletionPort failed with error%d/n", GetLastError());
			return 0;
		}
		// 创建同下面的WSARecv调用相关的IO套接字信息结构体
		if ((PerIOData = (LPPER_IO_OPERATION_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_OPERATION_DATA))) == NULL)
		{
			printf("GlobalAloc failed with error %d/n", GetLastError());
			return 0;
		}

		ZeroMemory(&(PerIOData->OVerlapped), sizeof(OVERLAPPED));
		PerIOData->BytesRecv = 0;
		PerIOData->BytesSend = 0;
		PerIOData->DATABuf.len = DATA_BUFSIZE;
		PerIOData->DATABuf.buf = PerIOData->Buffer;
		Flags = 0;

		if (WSARecv(Accept, &(PerIOData->DATABuf), 1, &RecvBytes, &Flags, &(PerIOData->OVerlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != ERROR_IO_PENDING)
			{
				printf("WSARecv()failed with error %d/n", WSAGetLastError());
				return 0;
			}
		}
	}

	return 0;
}

//工作线程
DWORD WINAPI ServerWorkerThread(LPVOID ComlpetionPortID)
{
	HANDLE  ComplectionPort = (HANDLE)ComlpetionPortID;
	DWORD   BytesTransferred;
	LPOVERLAPPED Overlapped;
	LPPER_HANDLE_DATA   PerHandleData;
	LPPER_IO_OPERATION_DATA PerIOData;
	DWORD   SendBytes, RecvBytes;

	DWORD   Flags;

	while (TRUE)
	{
		if (GetQueuedCompletionStatus(ComplectionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIOData, INFINITE) == 0)
		{
			printf("GetQueuedCompletionStatus failed with error%d/n", GetLastError());
			return 0;
		}

		//首先检查套接字上是否发生错误，如果发生了则关闭套接字并且清除同套节字相关的SOCKET_INFORATION 结构体
		if (BytesTransferred == 0)
		{
			printf("Closing Socket %d/n", PerHandleData->Socket);
			if (closesocket(PerHandleData->Socket) == SOCKET_ERROR)
			{
				printf("closesocket failed with error %d/n", WSAGetLastError());
				return 0;
			}
			GlobalFree(PerHandleData);
			GlobalFree(PerIOData);
			continue;
		}

		//检查BytesRecv域是否等于0，如果是，说明WSARecv调用刚刚完成，可以用从己完成的WSARecv调用返回的BytesTransferred值更新BytesRecv域
		if (PerIOData->BytesRecv == 0)
		{
			PerIOData->BytesRecv = BytesTransferred;
			PerIOData->BytesSend = 0;
		}
		else
		{
			PerIOData->BytesRecv += BytesTransferred;
		}

		//
		if (PerIOData->BytesRecv > PerIOData->BytesSend)//收到数据比发送的多了，就回发出去
		{
			//发布另一个WSASend()请求，因为WSASendi 不能确保发送了请的所有字节，继续WSASend调用直至发送完所有收到的字节
			ZeroMemory(&(PerIOData->OVerlapped), sizeof(OVERLAPPED));
			PerIOData->DATABuf.buf = PerIOData->Buffer + PerIOData->BytesSend;
			PerIOData->DATABuf.len = PerIOData->BytesRecv - PerIOData->BytesSend;

			if (WSASend(PerHandleData->Socket, &(PerIOData->DATABuf), 1, &SendBytes, 0, &(PerIOData->OVerlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSASend()fialed with error %d/n", WSAGetLastError());
					return 0;
				}
			}
		}
		else
		{
			PerIOData->BytesRecv = 0;
			//Now that is no more bytes to send post another WSARecv()request
			//现在己经发送完成
			Flags = 0;
			ZeroMemory(&(PerIOData->OVerlapped), sizeof(OVERLAPPED));
			PerIOData->DATABuf.buf = PerIOData->Buffer;
			PerIOData->DATABuf.len = DATA_BUFSIZE;
			if (WSARecv(PerHandleData->Socket, &(PerIOData->DATABuf), 1, &RecvBytes, &Flags, &(PerIOData->OVerlapped), NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSARecv()failed with error %d/n", WSAGetLastError());
					return 0;
				}
			}
		}
	}
}