#include <iostream>
#include <string>
#include <windows.h>
#include "Commands/test.h"
#include "libraries/Detours-4.0.1/include/detours.h"

#pragma warning(disable:4996)


void init_hooks()
{
	DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

}

DWORD WINAPI dwConsole(LPVOID)

{

	// Init Hooks
    init_hooks();
	
    //Init variables
    std::string command;

    //Initialize Console
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    //Redirect output to console
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    SetConsoleTitle(TEXT("Matthew's Halo Tool"));
    std::cout << "Placeholder.";
	
    while (true)
    {
        // Get user input
        std::cout << "\n> ";

        std::getline(std::cin, command);
        std::cout << command;

    	// Really awful command handling!
    	if (!command.compare("test")) TestCMD::test::do_something();
        if (!command.compare("exit")) break;
    	
    	
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