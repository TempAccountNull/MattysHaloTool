#include <iostream>
#include <windows.h>

#include "main.h"
#include "console.h"

#pragma warning(disable:4996)



DWORD WINAPI dwConsole(LPVOID)

{
	Console::AllocateConsole("Matthew's Halo Tool");
    return 0;
}

bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, dwConsole, 0, 0, 0);
        return TRUE;
    }
    return FALSE;
}