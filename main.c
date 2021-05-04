#include "main.h"

HHOOK keyHook;
BOOL isPaused;
DWORD pid;
DWORD keyCode = VK_NEXT;
STATE state = released;
HWND hWindow;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
    if (!ParseCmd(pCmdLine))
    {
        return 42;
    }

    const wchar_t *wcName = L"PauseU";
    WNDCLASSEX wc = { };

    wc.lpszClassName = wcName;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.cbSize = sizeof (WNDCLASSEX);

    RegisterClassEx(&wc);

    // Creating message loop olny (invisible) window for hook processing
    hWindow = CreateWindowEx(0, wcName, wcName, 0,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        HWND_MESSAGE, (HMENU)NULL, hInstance, (LPVOID)NULL);

    if (hWindow == NULL)
    {
        return 42;
    }

    MSG msg;

    while (GetMessage(&msg, (HWND)NULL, 0, 0) > 0)
    {
        DispatchMessage(&msg);
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE :
        keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeybordProc, NULL, 0);
        if (keyHook == NULL)
        {
            wprintf(L"Hook setting failed. Error code %d\n", GetLastError());
            PostQuitMessage(42);
        }
        wprintf(L"Keyboard hook created successfully\n");
        return 0;

    case WM_CLOSE :
        if (MessageBox(NULL, L"Саня, вырубай!", L"Exit promt", MB_OKCANCEL | MB_ICONQUESTION | MB_TOPMOST) == IDOK)
        {
            if (UnhookWindowsHookEx(keyHook))
            {
                wprintf(L"Hook removed. Closing...\n");
            }
            PostQuitMessage(0);
        }
        else
        {
            state = released;
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeybordProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    LPKBDLLHOOKSTRUCT event = (LPKBDLLHOOKSTRUCT)lParam;

    if (nCode >= 0 && event->vkCode == keyCode)
    {
        // Key has been released
        if (event->flags & LLKHF_UP)
        {
            state = state == promt ? promt : released;
        }
        // Key has been pressed
        else
        {
            if (state == held)
            {
                state = promt;
                PostMessage(hWindow, WM_CLOSE, 0, 0);
            }
            else if (state == released)
            {
                if (isPaused)
                {
                    isPaused = !Unpause(pid);
                }
                else
                {
                    isPaused = Pause(pid);
                }
                state = held;
            }
        }     
    }    
    return CallNextHookEx((HHOOK)NULL, nCode, wParam, lParam);  
}

BOOL ParseCmd(LPWSTR cmd)
{
    BOOL result = TRUE;
    LPWSTR *argv;
    int argc;
    int code;

    argv = CommandLineToArgvW(cmd, &argc);

    switch (argc)
    {
    case 2 :
        if (swscanf(argv[1], L"%x", &keyCode) != 1)
        {
            wprintf(L"%ls is not a valid hex value\n", argv[1]);
            result = FALSE;
            break;
        }
        // If the second argument is valid, continue parsing
    case 1 :
        if (swscanf(argv[0], L"%u", &pid) != 1)
        {
            pid = NameToPid(argv[0]);
            if (pid == -1)
            {
                wprintf(L"Process %ls not found\n", argv[0]);
                result = FALSE;
            }
        }
        break;

    default :
        wprintf(L"Syntax: pauseu pid|filename [virtual key code]");
        result = FALSE;
    }
    LocalFree((HLOCAL)argv);
    return result;
}