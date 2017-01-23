#ifndef __WIN32_SEMAPTHORE_H
#define __WIN32_SEMAPTHORE_H

#include "test.h"
#include <windows.h>
#include <iostream>
using namespace std;

/*

HANDLE WINAPI CreateSemaphore(
_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
_In_     LONG                  lInitialCount,
_In_     LONG                  lMaximumCount,
_In_opt_ LPCTSTR               lpName
);

*/

HANDLE g_semaphore = NULL;

DWORD WINAPI semaphoreThreadProc(void * arg)
{
	//等待信量号大于0，信号量-1
	WaitForSingleObject(g_semaphore, INFINITE);
	static int num = 1;
	cout << "num: " << num++ << endl;
	Sleep(1000);
	//释放信量号,这里释放1,即+1
	ReleaseSemaphore(g_semaphore, 1, NULL);
	return 0;
}

class Win32_Semaphore : public CTest
{
public:
	void test(int argc, char ** argv)
	{
		//创建信号量,初始值为0，最大值为10
		g_semaphore = CreateSemaphore(NULL, 0, 10, NULL);
		if (NULL == g_semaphore)
		{
			cout << "CreateSemaphore failed : " << GetLastError() << endl;
			return;
		}

		//创建线程
		HANDLE thread_handle[10] = { NULL };
		DWORD threadId = 0;
		for (int i = 0; i < 10; ++i)
		{
			thread_handle[i] = CreateThread(NULL, 0, semaphoreThreadProc, NULL, 0, &threadId);
			if (NULL == thread_handle[i])
			{
				cout << "CreateThread failed i: " << i << endl;
				return;
			}
		}

		//释放信号量,这里+1
		cout << "release semaphore" << endl;
		ReleaseSemaphore(g_semaphore, 1, NULL);

		WaitForMultipleObjects(10, thread_handle, TRUE, INFINITE);
		cout << "main thread exit" << endl;
	}
};

#endif