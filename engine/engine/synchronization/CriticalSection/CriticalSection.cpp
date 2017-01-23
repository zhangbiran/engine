#include "CriticalSection.h"

#include <windows.h>
#include <WinBase.h>
#include <iostream>
using namespace std;


//ע��: �ڽ���ֻ����ͬһ�����̵��߳�ͬ��, Ҫ���ڲ�ͬ����ʵ��ͬ����ʹ�û�����mutex

/*
void WINAPI InitializeCriticalSection(
_Out_ LPCRITICAL_SECTION lpCriticalSection
);
*/


/*
BOOL WINAPI InitializeCriticalSectionAndSpinCount(
	_Out_ LPCRITICAL_SECTION lpCriticalSection,
	_In_  DWORD              dwSpinCount
	);
	*/

CRITICAL_SECTION g_critical_section = { 0 };

DWORD WINAPI criticalThreadProc(void *arg)
{
	//TryEnterCriticalSection(&g_critical_section);	//������
	EnterCriticalSection(&g_critical_section);
	static int num = 1;
	cout << "num: " << num++ << endl;
	Sleep(1000);
	LeaveCriticalSection(&g_critical_section);

	return 0;
}

void CriticalSection::test(int argc, char ** argv)
{
	//�����ڽ���
#if defined(_WIN32_WINNT) && (_WIN32_WINNT >= 0x0403)
	InitializeCriticalSectionAndSpinCount(&g_critical_section, 4000);
	SetCriticalSectionSpinCount(&g_critical_section, 2000);//����spinCount
#else
	InitializeCriticalSection(&g_critical_section);
#endif

	//�����߳�
	HANDLE thread_handle[4] = { NULL };
	DWORD threadId;
	for (int i = 0; i < 4; ++i)
	{
		thread_handle[i] = CreateThread(NULL,
										0,
										criticalThreadProc,
										NULL,
										0,
										&threadId);
		if (NULL == thread_handle[i])
		{
			cout << "CreateThread failed i: " << i << endl;
		}
	}

	//�ȴ����߳̽���
	WaitForMultipleObjects(4, thread_handle, TRUE, INFINITE);
	cout << "main thread exit" << endl;

	//�ͷ��ڽ���
	DeleteCriticalSection(&g_critical_section);
}
