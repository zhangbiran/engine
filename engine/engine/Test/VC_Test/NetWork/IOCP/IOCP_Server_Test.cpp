#include "IOCP_Server_Test.h"
#include <Winsock2.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#include <iostream>
using namespace std;

#define PRINT(S, function) \
{\
if (SOCKET_ERROR == S)\
{\
	cout << function << " error" << endl; \
}\
}


typedef struct
{
	WSABUF wsaBuf;
	char buf[100];
	OVERLAPPED overlapped;
}PER_IO_DATA, *LPPER_IO_DATA;

typedef struct
{
	SOCKET sock;
}HANDLE_DATA, *LPHANDLE_DATA;

DWORD WINAPI AccpetThread(
	_In_ LPVOID lpParameter
	);

DWORD WINAPI WorkThread(
	_In_ LPVOID lpParameter
	);

void IOCP_Server_Test::test(int argc, char **argv)
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);

	HANDLE comPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (comPort == NULL)
	{
		cout << "CreateIoCompletionPort error " << endl;
	}

	DWORD threadID;
	HANDLE threadHandle;

	for (int i = 0; i < 4; i++)
	{
		threadHandle = CreateThread(NULL, 0, WorkThread, comPort, 0, &threadID);
		if (NULL == threadHandle)
		{
			cout << " CreateThread Failed: " << i << endl;
			break;
		}
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == sock)
	{
		cout << " socket error" << endl;
	}
	HOSTENT * localhost = gethostbyname("");
	cout << localhost->h_name << endl;

	char *ip = inet_ntoa(*(struct in_addr*)localhost->h_addr_list[0]);
	cout << ip << endl;

	struct sockaddr_in server_addr = { 0 };
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(7777);
	server_addr.sin_addr.S_un.S_addr = inet_addr(ip);
	ip = inet_ntoa(server_addr.sin_addr);
	cout << ip << endl;
	int ret = bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (SOCKET_ERROR == ret)
	{
		cout << "bind error" << endl;
	}

	ret = listen(sock, SOMAXCONN);
	if (SOCKET_ERROR == ret)
	{
		cout << "listen error" << endl;
	}

	while (true)
	{
		struct sockaddr_in addr = { 0 };
		int len = sizeof(addr);
		SOCKET acceptSock = accept(sock, (struct sockaddr*)&addr, &len);
		if (SOCKET_ERROR == acceptSock)
		{
			cout << "accpet error" << endl;
			break;

		}
		char *ip = inet_ntoa(addr.sin_addr);
		cout << ip << endl;


		LPHANDLE_DATA phandle_Data = (LPHANDLE_DATA)GlobalAlloc(GPTR, sizeof(HANDLE_DATA));
		phandle_Data->sock = acceptSock;
		HANDLE comPort2 = CreateIoCompletionPort(HANDLE(acceptSock), comPort, (DWORD)phandle_Data, 0);
		if (NULL == comPort2)
		{
			cout << "CreateIoCompletionPort 2" << endl;
			break;
		}

		LPPER_IO_DATA pData = (LPPER_IO_DATA)GlobalAlloc(GPTR, sizeof(PER_IO_DATA));
		ZeroMemory(&pData->overlapped, sizeof(OVERLAPPED));
		pData->wsaBuf.len = 100;
		pData->wsaBuf.buf = pData->buf;
		DWORD flags = 0;
		DWORD recvSize;
		WSARecv(acceptSock, &(pData->wsaBuf), 1, &recvSize, &flags, &(pData->overlapped), NULL);
		cout << "&(pData->overlapped) : " << &(pData->overlapped) << endl;
	}
}

DWORD WINAPI WorkThread(
	_In_ LPVOID lpParameter
	)
{
	HANDLE comPort = (HANDLE)lpParameter;
	DWORD sizeTransfer;
	HANDLE_DATA * handle_data = NULL;
	PER_IO_DATA * pdata = NULL;
	LPOVERLAPPED overlapTemp;
	static int i = 0;
	cout << i++ << endl;
	while (true)
	{
		if (!GetQueuedCompletionStatus(comPort, &sizeTransfer, (PULONG_PTR)&handle_data, (LPOVERLAPPED*)&overlapTemp, INFINITE))
		{
			cout << "GetQueuedCompletionStatus" << endl;
			break;
		}
		pdata = (LPPER_IO_DATA)CONTAINING_RECORD(overlapTemp, PER_IO_DATA, overlapped);
		pdata = (LPPER_IO_DATA)((char *)overlapTemp - FIELD_OFFSET(PER_IO_DATA, overlapped));
		cout << "&(*overlapTemp)" << &(*overlapTemp) << endl;
		//	cout << overlapTemp->Pointer << endl;
		cout << pdata->wsaBuf.buf << endl;
		//	cout << pdata->overlapped.Pointer << endl;
		ZeroMemory(&pdata->overlapped, sizeof(OVERLAPPED));
		pdata->wsaBuf.len = 100;
		pdata->wsaBuf.buf = pdata->buf;
		DWORD recvSize;
		DWORD flags = 0;
		WSARecv(handle_data->sock, &(pdata->wsaBuf), 1, &recvSize, &flags, &(pdata->overlapped), NULL);
	}

	cout << "exit" << endl;
	getchar();
	return 0;
}

DWORD WINAPI AccpetThread(
	_In_ LPVOID lpParameter
	)
{
	return 0;
}

