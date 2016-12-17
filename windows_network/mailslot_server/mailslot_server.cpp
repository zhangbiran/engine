#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;


int main(void)
{
	HANDLE mailslot_handle = CreateMailslot(_TEXT("\\\\.\\Mailslot\\Myslot"), 0, MAILSLOT_WAIT_FOREVER, NULL);
	if (INVALID_HANDLE_VALUE == mailslot_handle)
	{
		cout << "CreateMailslot Failed" << endl;
		return -1;
	}

	char buf[100] = { 0 };
	DWORD rcvSize = 0;
	while (ReadFile(mailslot_handle, buf, 100, &rcvSize, NULL))
	{
		buf[rcvSize] = 0;
		cout << buf << ":" << rcvSize << endl;
	}

	return 0;
}