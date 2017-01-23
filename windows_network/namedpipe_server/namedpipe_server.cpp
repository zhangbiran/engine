#include <iostream>
using namespace std;
#include <windows.h>
#include <tchar.h>


int main(void)
{
	HANDLE named_pipe_handle = CreateNamedPipe(_T("\\\\.\\Pipe\\mypipe"),
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 5, 0, 0, 1000, NULL);

	if (INVALID_HANDLE_VALUE == named_pipe_handle)
	{
		cout << "CreateNamedPipe Failed: " << GetLastError() << endl;
		return -1;
	}

	while (1)
	{
		if (0 == ConnectNamedPipe(named_pipe_handle, NULL))
		{
			cout << "ConnectNamedPipe" << endl;
			return -1;
		}
		cout << "connected." << endl;

		char buf[100] = { 0 };
		DWORD rcvSize = 0;
		DWORD writeSize = 0;
		while (1)
		{
			if (!ReadFile(named_pipe_handle, buf, 100, &rcvSize, NULL))
			{
				cout << "ReadFile Failed: " << GetLastError() << endl;
				break;
			}

			buf[rcvSize] = 0;
			cout << buf << ":" << rcvSize << endl;

			if (!WriteFile(named_pipe_handle, buf, rcvSize, &writeSize, NULL))
			{
				cout << "WriteFile Failed: " << GetLastError() << endl;
				break;
			}
		}

		if (0 == DisconnectNamedPipe(named_pipe_handle))
		{
			cout << "DisconnectNamedPipe Failed: " << GetLastError() << endl;
		}
	}

	CloseHandle(named_pipe_handle);
	cout << "main end" << endl;
	return 0;
}