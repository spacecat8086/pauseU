#ifndef _PROCHELPER_H_
#define _PROCHELPER_H_

#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

int NameToPid(const LPWSTR name);

#endif