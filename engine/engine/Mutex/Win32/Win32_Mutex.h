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
	ReleaseMutex(g_mutex);	//释放互斥量

	return 0;
}

class Win32_Mutex : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		g_mutex = CreateMutex(NULL, TRUE, NULL);	//第二个参数为TRUE, 指明当前创建它的线程一开始就占用它
		if (NULL == g_mutex)
		{
			cout << "CreateMutex failed: " << GetLastError() << endl;
			return;
		}

		//创建线程
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
		ReleaseMutex(g_mutex);	//释放创建时的占用
		WaitForMultipleObjects(4, thread_handle, TRUE, INFINITE);
		cout << "main thread exit" << endl;
	}
};


#endif