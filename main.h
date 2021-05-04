#ifndef _MAIN_H_
#define _MAIN_H_

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <stdio.h>
#include <windows.h>
#include "pause.h"
#include "prochelper.h"

typedef enum _state { released, held, promt } STATE;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeybordProc(int nCode, WPARAM wParam, LPARAM lParam);
BOOL ParseCmd(LPWSTR cmd);

#endif