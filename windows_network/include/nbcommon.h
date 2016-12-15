#ifndef __NBCOMMON_H
#define __NBCOMMON_H
#include <windows.h>
#include <nb30.h>
#include <iostream>
using namespace std;
#include <cstdio>
#include <cstdlib>

#pragma comment(lib, "Netapi32.lib")

int lanaEnum(LANA_ENUM& lan_enum);
void printLanaEnum(const LANA_ENUM& lan_enum);
int resetAll(const LANA_ENUM& lana_enum, int maxSession, int maxNameTableSize, bool isUseFirstName);
int addName(const LANA_ENUM& lana_enum, const char *server_name);
int delName(int lana, const char *name);
int listenSync(char *server_name, char *client_name, NCB& ncb);
int listenAsyn(char *server_name, char *client_name, int lana, void (CALLBACK *callBackFunc)(NCB *));
int sendSync(int session, int lana, char *buf, int len);
int sendAsyn(int session, int lana, char *buf, int len, void(CALLBACK * callBackFunc)(NCB *));
int rcvSync(int session, int lana, char *buf, int& len);
int rcvAsyn(int session, int lana, char *buf, int len, void (CALLBACK * callBackFunc)(NCB*));
int hangUp(int session, int lana);
int cancel(NCB * pncb);
void formatNetBiosName(char *nb_name, char *out_name);

#endif