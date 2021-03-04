#include "pch.h"


#pragma warning(disable:4996)


void main_init_hooks()
{
	DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    HaloReach::hooks::init_hooks();
}

void main_deinit_hooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    HaloReach::hooks::deinit_hooks();
    DetourTransactionCommit();
}

DWORD WINAPI dwConsole(LPVOID)

{
    // Init Hooks
    //main_init_hooks();
	
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
    	if (!command.compare("inithooks")) main_init_hooks();
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