#include "nbcommon.h"


char server_name[] = "server_asyn";
char client_name[] = "*";

void CALLBACK sendCallBack(NCB *pncb);
void CALLBACK rcvCallBack(NCB *pncb);


void CALLBACK listenCallBack(NCB * pncb)
{
	cout << pncb->ncb_callname << endl;

	
	if (NRC_GOODRET != listenAsyn(server_name, client_name, pncb->ncb_lana_num, listenCallBack))
	{
		return;
	}

	char *buf = new char[100];
	ZeroMemory(buf, 100);
	if (NRC_GOODRET != rcvAsyn(pncb->ncb_lsn, pncb->ncb_lana_num, buf, 100, rcvCallBack))
	{
		return;
	}


	cout << "listenCallBack end." << endl;
}

void CALLBACK rcvCallBack(NCB *pncb)
{
	cout << pncb->ncb_buffer << ":" << pncb->ncb_length << endl;

	if (NRC_GOODRET != sendAsyn(pncb->ncb_lsn, pncb->ncb_lana_num, (char*)pncb->ncb_buffer, pncb->ncb_length, sendCallBack))
	{
		return;
	}

	char *buf = new char[100];
	ZeroMemory(buf, 100);
	if (NRC_GOODRET != rcvAsyn(pncb->ncb_lsn, pncb->ncb_lana_num, buf, 100, rcvCallBack))
	{
		return;
	}

	delete pncb;
}

void CALLBACK sendCallBack(NCB *pncb)
{
	delete[]pncb->ncb_buffer;
	delete pncb;

	cout << "sendCallBack" << endl;
}

int main(void)
{
	LANA_ENUM lana_enum = { 0 };
	if (NRC_GOODRET != lanaEnum(lana_enum))
	{
		return -1;
	}

	printLanaEnum(lana_enum);

	if (NRC_GOODRET != resetAll(lana_enum, 254, 254, false))
	{
		return -1;
	}

	if (NRC_GOODRET != addName(lana_enum, server_name))
	{
		return -1;
	}

	
	for (int i = 0; i < lana_enum.length; ++i)
	{
		if (NRC_GOODRET != listenAsyn(server_name, client_name, lana_enum.lana[i], listenCallBack))
		{
			return -1;
		}
	}
	

	while (1)
	{
		Sleep(5000);
	}
	return 0;
}