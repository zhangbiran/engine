#include <iostream>
using namespace std;

#include <cstdio>
#include <WINSOCK2.H>
#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")      // Socket������õĶ�̬���ӿ�
#pragma comment(lib, "Kernel32.lib")    // IOCP��Ҫ�õ��Ķ�̬���ӿ�

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

	//��һ���յ���ɶ˿�
	if ((CompetionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) == NULL)
	{
		printf("CreateIoCompletionPort failed with error %d/n", GetLastError());
		return 0;
	}

	GetSystemInfo(&SystenInfo);
	// ����cpu������2�������߳�
	for (i = 0; i < SystenInfo.dwNumberOfProcessors * 2; i++)
	{
		HANDLE  ThreadHandle;

		//���������������̣߳��������̴߳�����ɶ˿�
		if ((ThreadHandle = CreateThread(NULL, 0, ServerWorkerThread, CompetionPort, 0, &ThreadID)) == NULL)
		{
			printf("CreateThread failed with error %d/n", GetLastError());
			return 0;
		}
		CloseHandle(ThreadHandle);
	}


	//��һ��������socket
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

// ��ʼ����IO����
	cout << "iocp2 ����������׼�����������ڵȴ��ͻ��˵Ľ���...\n";
	//�������Ӳ��ҷַ�����ɶ˿�
	while (TRUE)
	{
		if ((Accept = WSAAccept(Listen, NULL, NULL, NULL, 0)) == SOCKET_ERROR)
		{
			printf("WSAAccept failed with error %d/n", WSAGetLastError());
			return 0;
		}

		//�������׽�����ص��׽�����Ϣ�ṹ
		if ((PerHandleData = (LPPER_HANDLE_DATA)GlobalAlloc(GPTR, sizeof(PER_HANDLE_DATA))) == NULL)
		{
			printf("GlobalAlloc failed with error %d/n", GetLastError());
			return 0;
		}

		// Associate the accepted socket with the original completion port.
		printf("Socket number %d connected/n", Accept);

		PerHandleData->Socket = Accept;//�ṹ�д�����յ��׽���
		//�����ǵĴ������Ǹ���ɶ˿ڹ�������,���ؼ���Ҳ��ָ����һ����ɶ˿ڹ���
		if ((CreateIoCompletionPort((HANDLE)Accept, CompetionPort, (DWORD)PerHandleData, 0)) == NULL)
		{
			printf("CreateIoCompletionPort failed with error%d/n", GetLastError());
			return 0;
		}
		// ����ͬ�����WSARecv������ص�IO�׽�����Ϣ�ṹ��
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

//�����߳�
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

		//���ȼ���׽������Ƿ������������������ر��׽��ֲ������ͬ�׽�����ص�SOCKET_INFORATION �ṹ��
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

		//���BytesRecv���Ƿ����0������ǣ�˵��WSARecv���øո���ɣ������ôӼ���ɵ�WSARecv���÷��ص�BytesTransferredֵ����BytesRecv��
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
		if (PerIOData->BytesRecv > PerIOData->BytesSend)//�յ����ݱȷ��͵Ķ��ˣ��ͻط���ȥ
		{
			//������һ��WSASend()������ΪWSASendi ����ȷ����������������ֽڣ�����WSASend����ֱ�������������յ����ֽ�
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
			//���ڼ����������
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