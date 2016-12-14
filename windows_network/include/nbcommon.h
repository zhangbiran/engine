#ifndef __NBCOMMON_H
#define __NBCOMMON_H
#include <windows.h>
#include <nb30.h>
#include <iostream>
using namespace std;

#pragma comment(lib, "Netapi32.lib")

#include <windows.h>
int LanaEnum(LANA_ENUM *lenum);


#endif