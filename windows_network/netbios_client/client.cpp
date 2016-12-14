#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
#include <windows.h>
#include <nb30.h>

#pragma comment(lib, "netapi32.lib")

struct ARG
{
	int session;
	int lana;
};

DWORD WINAPI sendThreadProc(void *arg)
{
	ARG *ar = (ARG*)(arg);
	cout << "child thread: " << GetCurrentThreadId() << endl;

	char buf[100] = { 0 };
	NCB rcvNCB;
	ZeroMemory(&rcvNCB, sizeof(NCB));
	rcvNCB.ncb_command = NCBRECV;
	rcvNCB.ncb_buffer = PUCHAR(buf);
	rcvNCB.ncb_lsn = ar->session;
	rcvNCB.ncb_lana_num = ar->lana;
	while (1)
	{
		rcvNCB.ncb_length = 100;
		if (NRC_GOODRET != Netbios(&rcvNCB))
		{
			cout << "NCBRECV Failed" << endl;
			return -100;
		}
		buf[rcvNCB.ncb_length] = 0;
		cout << buf << endl;
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
		if (i == 0)
		{
			ZeroMemory(&ncb, sizeof(NCB));
			ncb.ncb_command = NCBADDNAME;
			ncb.ncb_lana_num = lan_enum.lana[i];
			memset(ncb.ncb_name, ' ', NCBNAMSZ);
			memcpy(ncb.ncb_name, "client1", strlen("client1"));
			if (NRC_GOODRET != Netbios(&ncb))
			{
				cout << "NCBADDNAME Failed" << endl;
				return -1;
			}

			printf("%d\n", ncb.ncb_num);
		}
		else
		{
			ZeroMemory(&ncb, sizeof(NCB));
			ncb.ncb_command = NCBADDNAME;
			ncb.ncb_lana_num = lan_enum.lana[i];
			memset(ncb.ncb_name, ' ', NCBNAMSZ);
			memcpy(ncb.ncb_name, "client2", strlen("client2"));
			if (NRC_GOODRET != Netbios(&ncb))
			{
				cout << "NCBADDNAME Failed" << endl;
				return -1;
			}
			printf("%d\n", ncb.ncb_num);
		}
	}

	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBCALL;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_name, "client2", strlen("client2"));

	memset(ncb.ncb_callname, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_callname, "netbios_server", strlen("netbios_server"));

	cout << "connecting.." << endl;
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "NCBCALL Failed" << endl;
		return -1;
	}
	cout << "client connected" << endl;
	printf("lana number: %d\n", ncb.ncb_lana_num);
	cout << "server name: " << ncb.ncb_callname << endl;

	int session = ncb.ncb_lsn;
	int lana = ncb.ncb_lana_num;
	DWORD threadID;
	

	ARG arg;
	arg.session = session;
	arg.lana = lana;
	HANDLE handle_thread = CreateThread(NULL, 0, sendThreadProc, &arg, 0, &threadID);
	if (NULL == handle_thread)
	{
		cout << "CreateThread Failed" << endl;
		return -1;
	}

	char buf[100] = { 0 };
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBSEND;
	ncb.ncb_lana_num = lana;
	ncb.ncb_lsn = session;
	ncb.ncb_buffer = PUCHAR(buf);
	while (cin.getline(buf, 100))
	{
		ncb.ncb_length = cin.gcount();
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "NCBSEND Failed" << endl;
			return -1;
		}
	}

	WaitForSingleObject(handle_thread, INFINITE);
	DWORD exitCode = 0;
	GetExitCodeThread(handle_thread, &exitCode);
	cout << "child thread exitCode: " << exitCode << endl;
	cout << "main end" << endl;
	return 0;
}