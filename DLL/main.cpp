#include "pch.h"


#pragma warning(disable:4996)


void main_init_hooks()
{
	DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    HaloReach::Hooks::init_hooks();
    DetourTransactionCommit();
}

void main_deinit_hooks()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    HaloReach::Hooks::deinit_hooks();
    DetourTransactionCommit();
}

void main_reinit_hooks()
{
    main_deinit_hooks();
    main_init_hooks();
    std::cout << "\n Hooks reinitialized.";
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
        if (!command.compare("reinithooks")) main_reinit_hooks();
        if (!command.compare("deinithooks")) main_deinit_hooks();
        if (!command.compare("breakpoint")) break;
    	
    	
    }
	

    return 0;
}

bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        main_init_hooks();
        CreateThread(0, 0, dwConsole, 0, 0, 0);
        return TRUE;
    }
    return FALSE;
}