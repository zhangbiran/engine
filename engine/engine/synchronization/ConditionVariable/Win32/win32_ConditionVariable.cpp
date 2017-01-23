#include "win32_ConditionVariable.h"

#include <windows.h>
#include <synchapi.h>
#include <iostream>
#include <list>
using namespace std;

/*

VOID WINAPI InitializeConditionVariable(
_Out_ PCONDITION_VARIABLE ConditionVariable
);

*/

const int consumerThreadCount = 3;

CRITICAL_SECTION g_critical_section2 = { 0 };
CONDITION_VARIABLE buf_not_full;
CONDITION_VARIABLE buf_not_empty;

std::list<int> g_list;
int g_exit_num = 0;


DWORD WINAPI producerThreadProc(void *arg)
{
	while (true)
	{
		EnterCriticalSection(&g_critical_section2);
		while (g_list.size() >= 10)
		{
			SleepConditionVariableCS(&buf_not_full, &g_critical_section2, INFINITE);
		}

		static int i = 0;
		g_list.push_back(i++);
		LeaveCriticalSection(&g_critical_section2);
		WakeConditionVariable(&buf_not_empty);
		//Sleep(500);
		if (++g_exit_num >= 20 && g_exit_num %consumerThreadCount == 0)
		{
			return 0;
		}
	}

	return 0;
}

DWORD WINAPI consumerThreadProc(void *arg)
{
	while (TRUE)
	{
		EnterCriticalSection(&g_critical_section2);
		while (g_list.empty())
		{
			SleepConditionVariableCS(&buf_not_empty, &g_critical_section2, INFINITE);
		}
		
		int num = g_list.front();
		g_list.pop_front();
		cout << "num: "<< num << " size: " << g_list.size() << endl;
		LeaveCriticalSection(&g_critical_section2);
		WakeConditionVariable(&buf_not_full);
		Sleep(500);
		if (g_exit_num >= 20 && g_list.empty())
		{
			return 0;
		}
	}

	return 0;
}

void Win32ConditionVariable::test(int argc, char **argv)
{
	InitializeCriticalSection(&g_critical_section2);
	EnterCriticalSection(&g_critical_section2);

	InitializeConditionVariable(&buf_not_full);
	InitializeConditionVariable(&buf_not_empty);

	DWORD thread_Id = 0;
	HANDLE producer_thread_handle = CreateThread(NULL, 0, producerThreadProc, NULL, 0, &thread_Id);

	HANDLE consumer_thread_handles[consumerThreadCount] = { NULL };
	for (int i = 0; i < consumerThreadCount; ++i)
	{
		consumer_thread_handles[i] = CreateThread(NULL, 0, consumerThreadProc, NULL, 0, &thread_Id);
	}

	cout << "release critical section" << endl;
	LeaveCriticalSection(&g_critical_section2);

	WaitForSingleObject(producer_thread_handle, INFINITE);
	WaitForMultipleObjects(consumerThreadCount, consumer_thread_handles, TRUE, INFINITE);

	cout << "main thread exit" << endl;

	DeleteCriticalSection(&g_critical_section2);
}