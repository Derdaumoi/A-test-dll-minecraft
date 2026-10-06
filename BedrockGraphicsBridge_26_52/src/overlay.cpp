#include "overlay.h"
#include "state.h"
#include "material_bridge.h"
#include "render_hook.h"
#include <thread>
#include <atomic>
#include <string>

static std::atomic_bool g_run{false};
static HWND g_hwnd = nullptr;
static std::thread g_thread;

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
    case WM_KEYDOWN:
        if (w == VK_F8) {
            State().menu = !State().menu.load();
            ShowWindow(h, State().menu ? SW_SHOW : SW_HIDE);
        }
        if (w == VK_F9) {
            State().mblLoader = !State().mblLoader.load();
            if (State().mblLoader) {
                // Resetting is intentionally not done here; loader is load-once.
                SetStatus("MBL Loader enabled (load-once)");
            } else {
                SetStatus("MBL Loader disabled");
            }
        }
        if (w == VK_F10) {
            State().deferred = !State().deferred.load();
            ApplyDeferredRenderState();
            SetStatus(State().deferred ? "Deferred Graphics: ON" : "Deferred Graphics: OFF");
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(h, &ps);
        RECT r{};
        GetClientRect(h, &r);

        FillRect(dc, &r, (HBRUSH)(COLOR_WINDOW + 1));

        std::string s =
            "BedrockGraphicsBridge 26.52\n"
            "F8  Menu\n"
            "F9  MBL Loader\n"
            "F10 Deferred Graphics\n\n"
            "MBL: " + std::string(State().mblLoader ? "ON" : "OFF") +
            "\nDeferred: " + std::string(State().deferred ? "ON" : "OFF") +
            "\nMaterials: " + std::to_string(State().materialCount.load()) +
            "\nRender hook: " + std::string(State().renderHook ? "ON" : "OFF") +
            "\n\n" + State().lastStatus;

        DrawTextA(dc, s.c_str(), -1, &r, DT_LEFT | DT_TOP | DT_NOPREFIX);
        EndPaint(h, &ps);
        return 0;
    }

    case WM_CLOSE:
        ShowWindow(h, SW_HIDE);
        State().menu = false;
        return 0;
    }
    return DefWindowProcA(h, m, w, l);
}

static void ThreadMain() {
    WNDCLASSA wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleA(nullptr);
    wc.lpszClassName = "BGBOverlayWindow";
    RegisterClassA(&wc);

    g_hwnd = CreateWindowExA(
        WS_EX_TOOLWINDOW | WS_EX_TOPMOST,
        wc.lpszClassName,
        "BedrockGraphicsBridge",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        80, 80, 430, 300,
        nullptr, nullptr, wc.hInstance, nullptr);

    // Hidden by default; F8 shows it.
    ShowWindow(g_hwnd, SW_HIDE);

    InstallRenderHook();

    while (g_run) {
        if (GetAsyncKeyState(VK_F8) & 1) {
            State().menu = !State().menu.load();
            ShowWindow(g_hwnd, State().menu ? SW_SHOW : SW_HIDE);
            InvalidateRect(g_hwnd, nullptr, TRUE);
        }

        if (State().mblLoader && !State().loadedForSession) {
            MaterialBridge::Instance().LoadAllOnce();
        }

        if (g_hwnd) InvalidateRect(g_hwnd, nullptr, FALSE);

        MSG msg{};
        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        Sleep(50);
    }

    RemoveRenderHook();
    if (g_hwnd) DestroyWindow(g_hwnd);
    g_hwnd = nullptr;
}

bool StartOverlay() {
    if (g_run.exchange(true)) return true;
    g_thread = std::thread(ThreadMain);
    g_thread.detach();
    return true;
}

void StopOverlay() {
    g_run = false;
}
