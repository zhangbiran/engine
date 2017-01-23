#include <iostream>
using namespace std;
#include <windows.h>
#include <tchar.h>


int main(void)
{
	if (!WaitNamedPipe(_T("\\\\.\\Pipe\\mypipe"), 1000))
	{
		cout << "WaitNamedPipe Failed: " << GetLastError() << endl;
		return -1;
	}

	HANDLE namedpipe_handle = CreateFile(_T("\\\\.\\Pipe\\mypipe"), GENERIC_READ | GENERIC_WRITE, 
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (NULL == namedpipe_handle)
	{
		cout << "CreateFile Failed: " << GetLastError() << endl;
		return -1;
	}

	char buf[100] = { 0 };
	char rcvBuf[100] = { 0 };
	DWORD rcvSize = 0;
	DWORD writeSize = 0;
	while (cin.getline(buf, 100))
	{
		if (!WriteFile(namedpipe_handle, buf, cin.gcount(), &writeSize, NULL))
		{
			cout << "WriteFile Failed: " << GetLastError() << endl;
			break;
		}

		if (!ReadFile(namedpipe_handle, rcvBuf, 100, &rcvSize, NULL))
		{
			cout << "ReadFile Failed: " << GetLastError() << endl;
			break;
		}

		cout << rcvBuf << ":" << rcvSize << endl;
	}

	CloseHandle(namedpipe_handle);
	return 0;
}