#include "prochelper.h"

int NameToPid(const LPWSTR name)
{
    HANDLE ProcList;
    PROCESSENTRY32 procEntry = { };
    int match = -1;
    BOOL next = TRUE;

    ProcList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    procEntry.dwSize = sizeof (PROCESSENTRY32);
    if (!Process32First(ProcList, &procEntry))
    {
        return -1;
    }

    // Search process by name
    for ( ; match = wcscmp(procEntry.szExeFile, name); next = Process32Next(ProcList, &procEntry))
    {
        if (!next) break;
    }
    if (match == 0)
    {
        return (int)procEntry.th32ProcessID;
    }
    return -1;
}