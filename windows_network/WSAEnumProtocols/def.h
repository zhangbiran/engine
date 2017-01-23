#ifndef __DEF_H
#define __DES_H

#include <iostream>
using namespace std;
#include <map>
#include <string>
#include <Winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
std::map<int, string> _protoMap;
std::map<int, string> _addressFamilyMap;


_protoMap[IPPROTO_HOPOPTS] = string("IPv6 Hop-by-Hop options");
#endif//(_WIN32_WINNT >= 0x0501)
IPPROTO_ICMP = 1,
IPPROTO_IGMP = 2,
IPPROTO_GGP = 3,
#if(_WIN32_WINNT >= 0x0501)
IPPROTO_IPV4 = 4,
#endif//(_WIN32_WINNT >= 0x0501)
#if(_WIN32_WINNT >= 0x0600)
IPPROTO_ST = 5,
#endif//(_WIN32_WINNT >= 0x0600)
IPPROTO_TCP = 6,
#if(_WIN32_WINNT >= 0x0600)
IPPROTO_CBT = 7,
IPPROTO_EGP = 8,
IPPROTO_IGP = 9,
#endif//(_WIN32_WINNT >= 0x0600)
IPPROTO_PUP = 12,
IPPROTO_UDP = 17,
IPPROTO_IDP = 22,
#if(_WIN32_WINNT >= 0x0600)
IPPROTO_RDP = 27,
#endif//(_WIN32_WINNT >= 0x0600)

#if(_WIN32_WINNT >= 0x0501)
IPPROTO_IPV6 = 41, // IPv6 header
IPPROTO_ROUTING = 43, // IPv6 Routing header
IPPROTO_FRAGMENT = 44, // IPv6 fragmentation header
IPPROTO_ESP = 50, // encapsulating security payload
IPPROTO_AH = 51, // authentication header
IPPROTO_ICMPV6 = 58, // ICMPv6
IPPROTO_NONE = 59, // IPv6 no next header
IPPROTO_DSTOPTS = 60, // IPv6 Destination options
#endif//(_WIN32_WINNT >= 0x0501)

IPPROTO_ND = 77,
#if(_WIN32_WINNT >= 0x0501)
IPPROTO_ICLFXBM = 78,
#endif//(_WIN32_WINNT >= 0x0501)
#if(_WIN32_WINNT >= 0x0600)
IPPROTO_PIM = 103,
IPPROTO_PGM = 113,
IPPROTO_L2TP = 115,
IPPROTO_SCTP = 132,
#endif//(_WIN32_WINNT >= 0x0600)
IPPROTO_RAW = 255,

IPPROTO_MAX = 256,
//
//  These are reserved for internal use by Windows.
//
IPPROTO_RESERVED_RAW = 257,
IPPROTO_RESERVED_IPSEC = 258,
IPPROTO_RESERVED_IPSECOFFLOAD = 259,
IPPROTO_RESERVED_WNV = 260,
IPPROTO_RESERVED_MAX = 261

_protoMap

#endif