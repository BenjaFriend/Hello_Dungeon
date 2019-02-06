
#ifndef UNICODE
#define UNICODE
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#define DEF_SERVER_PORT     50000
#define DEF_CLIENT_PORT     50001