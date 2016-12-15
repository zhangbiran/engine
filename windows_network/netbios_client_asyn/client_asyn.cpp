#include "nbcommon.h"

char server_name[] = "server_asyn";
char client_name[] = "client_asyn";

#define DEBUG_ERROR {\
	cout << "error: " << __FILE__ << " " << __LINE__ << endl; \
	return -1; \
}\

void CALLBACK rcvCallBack(struct _NCB * pncb)
{
	NCB *rcvNCB = pncb;
	cout << rcvNCB->ncb_buffer << ":" << rcvNCB->ncb_length << endl;

	rcvNCB->ncb_length = 100;
	if (NRC_GOODRET != Netbios(rcvNCB))
	{
		printf("%0x\n", rcvNCB->ncb_retcode);
		return;
	}
}

int sync_communication(NCB& ncb)
{
	int session = ncb.ncb_lsn;
	int lana = ncb.ncb_lana_num;

	char sendBuf[100] = { 0 };
	NCB sendNCB;
	ZeroMemory(&sendNCB, sizeof(NCB));
	sendNCB.ncb_command = NCBSEND;
	sendNCB.ncb_lsn = session;
	sendNCB.ncb_lana_num = lana;
	sendNCB.ncb_buffer = (PUCHAR)(sendBuf);

	while (cin.getline(sendBuf, 100))
	{
		sendNCB.ncb_length = cin.gcount();
		if (NRC_GOODRET != Netbios(&sendNCB))
		{
			printf("%0x\n", sendNCB.ncb_retcode);
			DEBUG_ERROR;
		}

		NCB rcvNCB;
		ZeroMemory(&rcvNCB, sizeof(NCB));
		char *rcvBuf = new char[100];
		ZeroMemory(rcvBuf, 100);
		rcvNCB.ncb_command = NCBRECV;
		rcvNCB.ncb_lsn = session;
		rcvNCB.ncb_lana_num = lana;
		rcvNCB.ncb_buffer = PUCHAR(rcvBuf);
		rcvNCB.ncb_length = 100;

		if (NRC_GOODRET != Netbios(&rcvNCB))
		{
			printf("%0x\n", rcvNCB.ncb_retcode);
			DEBUG_ERROR;
		}
		cout << rcvBuf << ":" << rcvNCB.ncb_length << endl;
		delete[]rcvBuf;
	}

	return 0;
}

int main(void)
{
	LANA_ENUM lana_enum = { 0 };
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (PUCHAR)(&lana_enum);
	ncb.ncb_length = sizeof(LANA_ENUM);
	if (NRC_GOODRET != Netbios(&ncb))
	{
		DEBUG_ERROR;
	}

	for (int i = 0; i < lana_enum.length; ++i)
	{
		cout << (int)(lana_enum.lana[i]) << " ";
	}
	cout << endl;


	for (int i = 0; i < lana_enum.length; ++i)
	{
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBRESET;
		ncb.ncb_callname[0] = 254;
		ncb.ncb_callname[2] = 254;
		ncb.ncb_callname[3] = UCHAR(false);
		ncb.ncb_lana_num = lana_enum.lana[i];
		if (NRC_GOODRET != Netbios(&ncb))
		{
			DEBUG_ERROR;
		}
	}



	for (int i = 0; i < lana_enum.length; ++i)
	{
		ZeroMemory(&ncb, sizeof(NCB));
		ncb.ncb_command = NCBADDNAME;
		memset(ncb.ncb_name, ' ', NCBNAMSZ);
		memcpy(ncb.ncb_name, client_name, strlen(client_name));
		ncb.ncb_lana_num = lana_enum.lana[i];
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout.flags(std::ios_base::hex);
			cout << "error code: " << ncb.ncb_retcode << endl;
			DEBUG_ERROR;
		}
	}

	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBCALL;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_name, client_name, strlen(client_name));
	memset(ncb.ncb_callname, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_callname, server_name, strlen(server_name));

	cout << "connecting .." << endl;
	if (NRC_GOODRET != Netbios(&ncb))
	{
		printf("%0x\n", ncb.ncb_retcode);
		DEBUG_ERROR;
	}
	cout << "connected" << endl;
	cout << ncb.ncb_callname << endl;

	sync_communication(ncb);

	return 0;
}