#include "win32_event.h"
#include <iostream>
using namespace std;

/*
HANDLE WINAPI CreateThread(
	_In_opt_  LPSECURITY_ATTRIBUTES  lpThreadAttributes,
	_In_      SIZE_T                 dwStackSize,
	_In_      LPTHREAD_START_ROUTINE lpStartAddress,
	_In_opt_  LPVOID                 lpParameter,
	_In_      DWORD                  dwCreationFlags,
	_Out_opt_ LPDWORD                lpThreadId
	);
	*/


/*
HANDLE WINAPI CreateEvent(
_In_opt_ LPSECURITY_ATTRIBUTES lpEventAttributes,
_In_     BOOL                  bManualReset,
_In_     BOOL                  bInitialState,
_In_opt_ LPCTSTR               lpName
);
*/

HANDLE g_event = NULL;

DWORD WINAPI threadProc(void * arg)
{
	WaitForSingleObject(g_event, INFINITE);	//�ͷ��߳�����,��eventΪ���ź�
	cout << "errrno: " << errno << endl;
	static int num = 1;
	cout << num++ << ": " << GetCurrentThreadId() << endl;
	SetEvent(g_event);	//���¼�
	
	return GetCurrentThreadId();
}

void Win32_Event::test(int argc, char ** argv)
{
	errno = 10;
	//�����¼�
	g_event = CreateEvent(NULL, FALSE, TRUE, NULL);	//�Զ������¼�����ʼΪ���¼�
	if (NULL == g_event)
	{
		cout << "CreateEvent failed: " << GetLastError() << endl;
		return;
	}
	ResetEvent(g_event);	//��Ϊ���¼�

	//�����߳�
	HANDLE thread_handle[4] = { NULL };
	for (int i = 0; i < 4; ++i)
	{
		DWORD threadId = 0;
		thread_handle[i] = CreateThread(NULL, 0, threadProc, NULL, 0, &threadId);
		if (NULL == thread_handle[i])
		{
			cout << "CreateThread failed" << "i :" << i << endl;
			return;
		}
	}

	cout << "signal event" << endl;
	SetEvent(g_event);		//��Ϊ���¼�

	//�ȴ������߳̽���
	WaitForMultipleObjects(4, thread_handle, TRUE, INFINITE);
	cout << "main thread exit " << GetCurrentThreadId() << endl;

	//��ȡ�߳��˳���
	for (int i = 0; i < 4; ++i)
	{
		DWORD exitCode = 0;
		if (GetExitCodeThread(thread_handle[i], &exitCode))
		{
			cout << "exitCode: " << exitCode << endl;
		}
	}

	cout << "main errno: " << errno << endl;
	//�رվ��
	CloseHandle(g_event);
}