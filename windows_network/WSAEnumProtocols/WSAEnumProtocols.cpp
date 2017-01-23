#include <iostream>
using namespace std;

#include <Winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

/*
int WSAEnumProtocols(
_In_    LPINT              lpiProtocols,
_Out_   LPWSAPROTOCOL_INFO lpProtocolBuffer,
_Inout_ LPDWORD            lpdwBufferLength
);


*/
IP_ADDR_ANY
ADDR_ANY
void printProtocols(WSAPROTOCOL_INFO *info)
{
	printf("%s\n", info->szProtocol);
}

int main(void)
{
	WSAData data = { 0 };
	WSAStartup(MAKEWORD(2, 2), &data);
	WSAPROTOCOL_INFO *info = NULL;
	DWORD protoNum = 0;
	int ret = WSAEnumProtocols(NULL, info, &protoNum);
	if (WSAENOBUFS != WSAGetLastError())
	{
		cout << "error" << endl;
		return -1;
	}

	cout << "protoNum: " << protoNum << endl;
	cout << "ret: " << ret << endl;
	cout << protoNum / sizeof(WSAPROTOCOL_INFO) << endl;
	cout << protoNum % sizeof(WSAPROTOCOL_INFO) << endl;
	cout << sizeof(WSAPROTOCOL_INFO) << endl;

	info = (WSAPROTOCOL_INFO*)GlobalAlloc(GPTR, protoNum);
	cout << info[0].iAddressFamily << endl;

	ret = WSAEnumProtocols(NULL, info, &protoNum);
	if (SOCKET_ERROR == ret)
	{
		cout << "WSAEnumProtocols Failed" << endl;
		return -1;
	}
	cout << "protoNum: " << protoNum << endl;
	cout << "ret: " << ret << endl;

	for (int i = 0; i < ret; ++i)
	{
		printProtocols(&info[i]);
	}
	
	WSACleanup();
	return 0;
}