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

	Sleep(500);									//INFINITE 一直不time out
	cout << "pid: " << _getpid() << endl;		//进程号
	cout << "child errno: " << errno << endl;	// 0

	int thread_Id = GetCurrentThreadId();
	cout << "thread_id: " << thread_Id << endl;
	
	char *buf = "child thread local storage";
	TlsSetValue(g_TlsIndex, buf);
	char *pBuf = (char*)TlsGetValue(g_TlsIndex);
	if (pBuf != NULL)
	{
		cout << pBuf << endl;
		//TlsFree(g_TlsIndex);		//如果这里先释放，那么主线程使用TlsValue的时候就不会成功
	}

	int exitCode = 100;				
	_endthreadex(exitCode);				//退出码，会被等待它结束的线程捕抓到
	
	return 0;
}

void Win32_Therad::test(int argc, char ** argv)
{
	errno = 200;	//子讲程和主进程拥有独立内存区存储errno全局变量

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