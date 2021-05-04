#include "pause.h"

BOOL Pause(DWORD pid)
{
    int result = DebugActiveProcess(pid);

    if (!result)
    {
        wprintf(L"Can't suspend process %u. Error %x\n", pid, GetLastError());
        return FALSE;
    }
    wprintf(L"Process %u successfully suspended\n", pid);
    DebugSetProcessKillOnExit(FALSE);
    return TRUE;    
}

BOOL Unpause(DWORD pid)
{
    int result = DebugActiveProcessStop(pid);
        
    if (!result)
    {
        wprintf(L"Can't resume process %u. Error %x\n", pid, GetLastError());
        return FALSE;
    }
    wprintf(L"Process %u successfully resumed\n", pid);
    return TRUE; 
}