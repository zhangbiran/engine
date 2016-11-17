#ifndef __WIN32_MUTEX_H
#define __WIN32_MUTEX_H
#include "test.h"
#include <windows.h>
#include <iostream>
using namespace std;


/*

HANDLE WINAPI CreateMutex(
_In_opt_ LPSECURITY_ATTRIBUTES lpMutexAttributes,
_In_     BOOL                  bInitialOwner,
_In_opt_ LPCTSTR               lpName
);

*/

HANDLE g_mutex = NULL;

DWORD WINAPI mutexThreadProc(void *arg)
{
	WaitForSingleObject(g_mutex, INFINITE);
	static int num = 1;
	cout << "num: " << num++ << endl;
	Sleep(1000);
	ReleaseMutex(g_mutex);	//�ͷŻ�����

	return 0;
}

class Win32_Mutex : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		g_mutex = CreateMutex(NULL, TRUE, NULL);	//�ڶ�������ΪTRUE, ָ����ǰ���������߳�һ��ʼ��ռ����
		if (NULL == g_mutex)
		{
			cout << "CreateMutex failed: " << GetLastError() << endl;
			return;
		}

		//�����߳�
		HANDLE thread_handle[4] = { NULL };
		DWORD threadId = 0;
		for (int i = 0; i < 4; ++i)
		{
			thread_handle[i] = CreateThread(NULL, 0, mutexThreadProc, NULL, 0, &threadId);
			if (NULL == thread_handle[i])
			{
				cout << "CreateThread failed i:" << i << endl;
				return;
			}
		}

		cout << "release mutex" << endl;
		ReleaseMutex(g_mutex);	//�ͷŴ���ʱ��ռ��
		WaitForMultipleObjects(4, thread_handle, TRUE, INFINITE);
		cout << "main thread exit" << endl;
	}
};


#endif