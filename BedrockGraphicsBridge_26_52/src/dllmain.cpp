#include <windows.h>
#include "overlay.h"

static DWORD WINAPI MainThread(LPVOID module) {
    StartOverlay();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE h = CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
        if (h) CloseHandle(h);
    }
    return TRUE;
}
