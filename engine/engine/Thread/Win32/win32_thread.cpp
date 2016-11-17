#include "win32_thread.h"
#include <windows.h>
#include <process.h>
#include <iostream>
using namespace std;

DWORD g_TlsIndex = 0;
/*
uintptr_t _beginthreadex( // NATIVE CODE  
	void *security,
	unsigned stack_size,
	unsigned(__stdcall *start_address)(void *),
	void *arglist,
	unsigned initflag,
	unsigned *thrdaddr
	);
	*/
	
unsigned threadProc(void * arg)
{
	int * p = (int*)arg;
	cout << "arg: " << *p << endl;

	Sleep(500);									//INFINITE һֱ��time out
	cout << "pid: " << _getpid() << endl;		//���̺�
	cout << "child errno: " << errno << endl;	// 0

	int thread_Id = GetCurrentThreadId();
	cout << "thread_id: " << thread_Id << endl;
	
	char *buf = "child thread local storage";
	TlsSetValue(g_TlsIndex, buf);
	char *pBuf = (char*)TlsGetValue(g_TlsIndex);
	if (pBuf != NULL)
	{
		cout << pBuf << endl;
		//TlsFree(g_TlsIndex);		//����������ͷţ���ô���߳�ʹ��TlsValue��ʱ��Ͳ���ɹ�
	}

	int exitCode = 100;				
	_endthreadex(exitCode);				//�˳��룬�ᱻ�ȴ����������̲߳�ץ��
	
	return 0;
}

void Win32_Therad::test(int argc, char ** argv)
{
	errno = 200;	//�ӽ��̺�������ӵ�ж����ڴ����洢errnoȫ�ֱ���

	DWORD tlsIndex = TlsAlloc();
	char *buf = "main thread local storage";
	TlsSetValue(tlsIndex, buf);
	g_TlsIndex = tlsIndex;

	int arg = 10;
	uintptr_t thread_handle = _beginthreadex(
		NULL,
		0,
		(unsigned(__stdcall *)(void*))threadProc,
		&arg,
		0,
		NULL);

	DWORD status = 0;
	if (::WaitForSingleObject((HANDLE)thread_handle, INFINITE) == WAIT_OBJECT_0
		&& ::GetExitCodeThread((HANDLE)thread_handle, &status) != FALSE)
	{
		::CloseHandle((HANDLE)thread_handle);
	}

	char *pBuf = (char*)TlsGetValue(g_TlsIndex);
	if (NULL != pBuf)
	{
		cout << pBuf << endl;
		TlsFree(g_TlsIndex);
	}

	cout << "child exit status: " << status << endl;	//100
	cout << "main thread errno: " << errno << endl;		//200
	cout << "main thread exit" << endl;
}