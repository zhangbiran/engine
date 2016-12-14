#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include <windows.h>
#include <nb30.h>

#pragma comment(lib, "netapi32.lib")

struct ARG
{
	int lana;
	int session;
};

DWORD WINAPI sendThreadProc(void *arg)
{
	cout << "child thread: " << GetCurrentThreadId() << endl;

	ARG * ar = (ARG*)(arg);
	char sendBuf[100] = { 0 };
	NCB sendNCB;
	ZeroMemory(&sendNCB, sizeof(NCB));
	sendNCB.ncb_command = NCBSEND;
	sendNCB.ncb_lsn = ar->session;
	sendNCB.ncb_lana_num = ar->lana;
	sendNCB.ncb_buffer = PUCHAR(sendBuf);
	cin.rdbuf()->pubsync();
	while (cin.getline(sendBuf, 100))
	{
		sendNCB.ncb_length = cin.gcount();
		if (NRC_GOODRET != Netbios(&sendNCB))
		{
			cout << "NCBSEND Failed" << endl;
			return -100;
		}
	}

	return 200;
}

int main(void)
{
	LANA_ENUM lan_enum = { 0 };
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = PUCHAR(&lan_enum);
	ncb.ncb_length = sizeof(LANA_ENUM);
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "NCBENUM Failed" << endl;
		return -1;
	}

	for (int i = 0; i < lan_enum.length; ++i)
	{
		printf("%d ", lan_enum.lana[i]);
	}
	printf("\n");

	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_callname[0] = 254;
	ncb.ncb_callname[2] = 254;
	ncb.ncb_callname[3] = UCHAR(false);

	for (int i = 0; i < lan_enum.length; ++i)
	{
		ncb.ncb_lana_num = lan_enum.lana[i];
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "NCBRESET Failed" << endl;
			return -1;
		}
	}

	for (int i = 0; i < lan_enum.length; ++i)
	{
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBADDNAME;
		ncb.ncb_lana_num = lan_enum.lana[i];
		memset(ncb.ncb_name, ' ', NCBNAMSZ);
		memcpy(ncb.ncb_name, "netbios_server", strlen("netbios_server"));
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "NCBADDNAME Failed" << endl;
			return -1;
		}
		
		printf("%d\n", ncb.ncb_num);
	}
	
	while (1)
	{
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBLISTEN;
		memset(ncb.ncb_name, ' ', NCBNAMSZ);
		memcpy(ncb.ncb_name, "netbios_server", strlen("netbios_server"));

		memset(ncb.ncb_callname, ' ', NCBNAMSZ);
		memcpy(ncb.ncb_callname, "client2", strlen("client2"));

		cout << "listening.." << endl;
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "NCBLISTEN Failed" << endl;
			return -1;
		}
		cout << "client connected" << endl;
		printf("lana number: %d\n", ncb.ncb_lana_num);
		cout << "client name: " << ncb.ncb_callname << endl;

		int session = ncb.ncb_lsn;
		int lana = ncb.ncb_lana_num;
		ARG arg;
		arg.lana = lana;
		arg.session = session;
		DWORD threadID;
		HANDLE thread_handle = CreateThread(NULL, 0, sendThreadProc, &arg, 0, &threadID);
		if (NULL == thread_handle)
		{
			cout << "CreateThread Failed" << endl;
			return -1;
		}

		char buf[100] = { 0 };
		int len = 100;
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBRECV;
		ncb.ncb_buffer = PUCHAR(buf);
		ncb.ncb_lsn = session;
		ncb.ncb_lana_num = lana;
		
		NCB sendNCB;
		ZeroMemory(&sendNCB, sizeof(NCB));
		sendNCB.ncb_command = NCBSEND;
		sendNCB.ncb_lsn = session;
		sendNCB.ncb_lana_num = lana;
		sendNCB.ncb_buffer = PUCHAR(buf);
		while (1)
		{
			Sleep(5000);
			ncb.ncb_length = sizeof(buf);	//这个每次接收会变化，变成接收到的字节数
			if (NRC_GOODRET != Netbios(&ncb))
			{
				cout << "NCBRECV Failed" << endl;
				TerminateThread(thread_handle, 300);
				WaitForSingleObject(thread_handle, INFINITE);
				DWORD exitCode = 0;
				if (GetExitCodeThread(thread_handle, &exitCode))
				{
					cout << "child thread exitCode: " << exitCode << endl;
				}
				CloseHandle(thread_handle);
				break;
			}

			printf("%d\n", buf[ncb.ncb_length - 1]);
			cout << buf << ":" << ncb.ncb_length << endl;

			sendNCB.ncb_length = ncb.ncb_length;
			if (NRC_GOODRET != Netbios(&sendNCB))
			{
				cout << "NCBSEND Failed" << endl;
				
				break;
			}
		}
	}
	
	
	

	return 0;
}