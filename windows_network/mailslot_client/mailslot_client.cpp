#include <windows.h>
#include <tchar.h>
#include <iostream>
using namespace std;

int main()
{
	HANDLE mailslot_handle = CreateFile(_TEXT("\\\\.\\Mailslot\\Myslot"), GENERIC_WRITE,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == mailslot_handle)
	{
		cout << "CreateFile Failed" << endl;
		return -1;
	}

	char buf[100] = { 0 };
	DWORD writeSize = 0;
	while (cin.getline(buf, 100))
	{
		WriteFile(mailslot_handle, buf, cin.gcount(), &writeSize, NULL);
		if (writeSize != cin.gcount())
		{
			cout << "WriteFile Failed" << endl;
			return -1;
		}
	}

	CloseHandle(mailslot_handle);
	return 0;
}