#include "nbcommon.h"

int LanaEnum(LANA_ENUM *lenum)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = PUCHAR(lenum);
	ncb.ncb_length = sizeof(LANA_ENUM);

	if (Netbios(&ncb) != NRC_GOODRET)
	{
		cout << "error: Netbios: NCBENUM: " << ncb.ncb_retcode << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
}
