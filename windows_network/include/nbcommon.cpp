#include "nbcommon.h"

int lanaEnum(LANA_ENUM& lan_enum)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBENUM;
	ncb.ncb_buffer = (PUCHAR)(&lan_enum);
	ncb.ncb_length = sizeof(LANA_ENUM);
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "lanaEnum Failed" << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
}

void printLanaEnum(const LANA_ENUM& lan_enum)
{
	cout << "lana numbers: ";
	for (int i = 0; i < lan_enum.length; ++i)
	{
		cout << int(lan_enum.lana[i]) << " ";
	}
	cout << endl;
}

int resetAll(const LANA_ENUM& lana_enum, int maxSession, int maxNameTableSize, bool isUseFirstName)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBRESET;
	ncb.ncb_callname[0] = maxSession;
	ncb.ncb_callname[2] = maxNameTableSize;
	ncb.ncb_callname[3] = isUseFirstName;
	for (int i = 0; i < lana_enum.length; ++i)
	{
		ncb.ncb_lana_num = lana_enum.lana[i];
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "resetAll Failed" << endl;
			return ncb.ncb_retcode;
		}
	}

	return NRC_GOODRET;
}

int addName(const LANA_ENUM& lana_enum, const char *server_name)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBADDNAME;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_name, server_name, strlen(server_name));
	for (int i = 0; i < lana_enum.length; ++i)
	{
		ncb.ncb_lana_num = lana_enum.lana[i];
		if (NRC_GOODRET != Netbios(&ncb))
		{
			cout << "addName Failed" << endl;
			return ncb.ncb_retcode;
		}
	}

	return NRC_GOODRET;
}

int delName(int lana, const char *name)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBDELNAME;
	ncb.ncb_lana_num = lana;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_name, name, strlen(name));
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "delName" << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
}

int listenSync(char *server_name, char *client_name, NCB& ncb)
{
	cout << "listenning ..." << endl;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBLISTEN;
	memset(ncb.ncb_name, ' ', NCBNAMSZ);
	memset(ncb.ncb_callname, ' ', NCBNAMSZ);
	memcpy(ncb.ncb_name, server_name, strlen(server_name));
	memcpy(ncb.ncb_callname, client_name, strlen(client_name));
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "acceptSync Failed" << endl;
		return ncb.ncb_retcode;
	}

	cout << "listen end." << endl;
	return NRC_GOODRET;
}

int listenAsyn(char *server_name, char *client_name, int lana, void (CALLBACK *callBackFunc)(NCB *))
{
	NCB *listenNCB = new NCB();
	ZeroMemory(listenNCB, sizeof(NCB));
	listenNCB->ncb_command = NCBLISTEN | ASYNCH;
	memset(listenNCB->ncb_name, ' ', NCBNAMSZ);
	memcpy(listenNCB->ncb_name, server_name, strlen(server_name));
	memset(listenNCB->ncb_callname, ' ', NCBNAMSZ);
	memcpy(listenNCB->ncb_callname, client_name, strlen(client_name));
	listenNCB->ncb_post = callBackFunc;
	listenNCB->ncb_lana_num = lana;

	cout << "listening ..." << endl;
	if (NRC_GOODRET != Netbios(listenNCB))
	{
		cout << "listenAsyn Failed" << endl;
		delete listenNCB;
		return listenNCB->ncb_retcode;
	}

	cout << "listening end." << endl;
	return NRC_GOODRET;
}

int sendSync(int session, int lana, char *buf, int len)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBSEND;
	ncb.ncb_lsn = session;
	ncb.ncb_lana_num = lana;
	ncb.ncb_buffer = (PUCHAR)(buf);
	ncb.ncb_length = len;
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "sendSync Failed" << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
};

int sendAsyn(int session, int lana, char *buf, int len, void(CALLBACK * callBackFunc)(NCB *))
{
	NCB *pncb = new NCB;
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBSEND | ASYNCH;
	pncb->ncb_lsn = session;
	pncb->ncb_lana_num = lana;
	pncb->ncb_buffer = (PUCHAR)(buf);
	pncb->ncb_length = len;
	pncb->ncb_post = callBackFunc;

	if (NRC_GOODRET != Netbios(pncb))
	{
		cout << "sendAsyn Failed" << endl;
		return pncb->ncb_retcode;
		delete pncb;
	}

	return NRC_GOODRET;
}

int rcvSync(int session, int lana, char *buf, int& len)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBRECV;
	ncb.ncb_lsn = session;
	ncb.ncb_lana_num = lana;
	ncb.ncb_buffer = (PUCHAR)(buf);
	ncb.ncb_length = len;
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "rcvSync Failed" << endl;
		return ncb.ncb_retcode;
	}

	len = ncb.ncb_length;
	return NRC_GOODRET;
}

int rcvAsyn(int session, int lana, char *buf, int len, void (CALLBACK * callBackFunc)(NCB*))
{
	NCB *pncb = new NCB();
	ZeroMemory(pncb, sizeof(NCB));
	pncb->ncb_command = NCBRECV | ASYNCH;
	pncb->ncb_lsn = session;
	pncb->ncb_lana_num = lana;
	pncb->ncb_buffer = (PUCHAR)(buf);
	pncb->ncb_length = len;
	pncb->ncb_post = callBackFunc;

	if (NRC_GOODRET != Netbios(pncb))
	{
		cout << "rcvAsyn Failed" << endl;
		delete pncb;
		return pncb->ncb_retcode;
	}

	return NRC_GOODRET;
}

int hangUp(int session, int lana)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBHANGUP;
	ncb.ncb_lsn = session;
	ncb.ncb_lana_num = lana;
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "hangUp Failed" << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
}

int cancel(NCB * pncb)
{
	NCB ncb;
	ZeroMemory(&ncb, sizeof(NCB));
	ncb.ncb_command = NCBCANCEL;
	ncb.ncb_lana_num = pncb->ncb_lana_num;
	ncb.ncb_buffer = (PUCHAR)(pncb);
	if (NRC_GOODRET != Netbios(&ncb))
	{
		cout << "cancel Failed" << endl;
		return ncb.ncb_retcode;
	}

	return NRC_GOODRET;
}

void formatNetBiosName(char *nb_name, char *out_name)
{
	memcpy(out_name, nb_name, NCBNAMSZ);
	out_name[NCBNAMSZ] = 0;
}