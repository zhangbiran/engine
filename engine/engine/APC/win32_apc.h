#ifndef __WIN32_APC_H
#define __WIN32_APC_H

#include "test.h"
#include <windows.h>
#include <iostream>
using namespace std;
/*
DWORD WINAPI QueueUserAPC(
_In_ PAPCFUNC  pfnAPC,
_In_ HANDLE    hThread,
_In_ ULONG_PTR dwData
);

*/

/*
VOID CALLBACK APCProc(
_In_ ULONG_PTR dwParam
);

*/

/*

DWORD WINAPI WaitForSingleObjectEx(
_In_ HANDLE hHandle,
_In_ DWORD  dwMilliseconds,
_In_ BOOL   bAlertable
);
*/

HANDLE g_event2 = NULL;
HANDLE g_thread_handle = NULL;

VOID CALLBACK APCProc(ULONG_PTR dwParam);

DWORD WINAPI apcThreadProc(void *arg)
{
	WaitForSingleObject(g_event2, INFINITE);
	cout << "child thread come" << GetCurrentThreadId() << endl;
	SetEvent(g_event2);

	QueueUserAPC(APCProc, g_thread_handle, NULL);
	for (;;)
	{
		WaitForSingleObjectEx(g_event2, INFINITE, TRUE);
		cout << "event ocur" << endl;
		QueueUserAPC(APCProc, g_thread_handle, NULL);
		Sleep(1000);
		//SetEvent(g_event2);
	}

	return 0;
}

VOID CALLBACK APCProc(ULONG_PTR dwParam)
{
	static int i = 1;
	cout << i++ << endl;
}

VOID CALLBACK APCProc2(ULONG_PTR dwParam)
{
	cout << "world " << endl;
}


class Win32_APC : public CTest
{
public:
	void test(int argc, char **argv)
	{
		g_event2 = CreateEvent(NULL, FALSE, FALSE, NULL);

		DWORD thread_id = 0;
		g_thread_handle = CreateThread(NULL, 0, apcThreadProc, NULL, 0, &thread_id);

		//QueueUserAPC(APCProc, g_thread_handle, NULL);
		//QueueUserAPC(APCProc2, g_thread_handle, NULL);


		cout << "SetEvent" << endl;
		SetEvent(g_event2);
		WaitForSingleObject(g_thread_handle, INFINITE);
		cout << "main thread exit" << endl;
		CloseHandle(g_event2);
	}
};

#endif