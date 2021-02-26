#include <iostream>
#include <string>
#include <windows.h>

#pragma warning(disable:4996)



DWORD WINAPI dwConsole(LPVOID)

{
    //Init variables
    std::string command;

    //Initialize Console
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    //Redirect output to console
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    SetConsoleTitle(TEXT("Matthew's Halo Tool"));

    while (true)
    {
        // Get user input
        std::cout << "\n> ";

        std::getline(std::cin, command);
        std::cout << command;
    }

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