#ifndef __SYNCHRONIZATION_BARRIER_H
#define __SYNCHRONIZATION_BARRIER_H

#include "test.h"
#include <windows.h>
#include <iostream>
using namespace std;

//注意，最小要windows 8支持

/*
BOOL WINAPI InitializeSynchronizationBarrier(
	_Out_ LPSYNCHRONIZATION_BARRIER lpBarrier,
	_In_  LONG                      lTotalThreads,
	_In_  LONG                      lSpinCount
	);
	*/

/*
BOOL  WINAPI EnterSynchronizationBarrier(
_Inout_ LPSYNCHRONIZATION_BARRIER lpBarrier,
_In_    DWORD                     dwFlags
);

*/

/*
BOOL  WINAPI DeleteSynchronizationBarrier(
_Inout_ LPSYNCHRONIZATION_BARRIER lpBarrier
);
*/


/*
SYNCHRONIZATION_BARRIER g_syn_barrier = { 0 };

DWORD WINAPI synBarrierThreadProc(void *arg)
{
	cout << "thread arrived" << endl;
	EnterSynchronizationBarrier(&g_syn_barrier, SYNCHRONIZATION_BARRIER_FLAGS_SPIN_ONLY);
	cout << "thread Id: " << GetCurrentThreadId() << " enter" << endl;
	return 0;
}
*/

class SynchronizationBarrier : public CTest
{
public:
	void test(int argc, char **argv)
	{
	/*
		if (!InitializeSynchronizationBarrier(&g_syn_barrier, 5, -1))
		{
			cout << "InitializeSynchronizationBarrier failed" << endl;
			return;
		}

		HANDLE thread_handle[10] = { NULL };
		DWORD threadId = 0;
		for (int i = 0; i < 10; ++i)
		{
			thread_handle[i] = CreateThread(NULL, 0, synBarrierThreadProc, NULL, 0, &threadId);
			Sleep(1);
		}

		WaitForMultipleObjects(10, thread_handle, TRUE, INFINITE);
		cout << "main thread exit" << endl;

		DeleteSynchronizationBarrier(&g_syn_barrier);
		*/
	}
	
};


#endif