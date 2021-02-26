#include <iostream>
#include <windows.h>

#include "main.h"
#include "console.h"

#pragma warning(disable:4996)

Main::Main
{
	Console::Console Terminal;
	std::map<std::string, std::shared_ptr<MainModule>> Commands;

	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleOutputCP(437);
	unsigned int ConsoleWidth = 80;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleBuf;
	if (GetConsoleScreenBufferInfo(hStdout, &ConsoleBuf))
	{
		ConsoleWidth = ConsoleBuf.dwSize.X;
	}

	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

#ifdef _ELDEBUG
	std::string buildType = "Debug ";
#else
	std::string buildType = "";
#endif
	std::cout << "ElDorito" << "\xC3\xC4\xC2\xC4\xC4\xC4\xC4\xC4\xC4\xB4 " << buildType << "Build Date: " << __DATE__ << " @ " << __TIME__ << std::endl;
	Terminal.SetTextColor(Console::Input);

	std::srand(GetTickCount());


	Terminal.SetTextColor(Console::Green | Console::Bright);
	std::cout << "Enter \"";
	Terminal.SetTextColor(Console::Input);
	std::cout << "help";
	Terminal.SetTextColor(Console::Green | Console::Bright);
	std::cout << "\" or \"";
	Terminal.SetTextColor(Console::Input);
	std::cout << "help (command)";
	Terminal.SetTextColor(Console::Green | Console::Bright);

	std::cout << "\" to get started!" << std::endl;

	std::cout << "Please take the time to read our ";
	Terminal.SetTextColor(Console::Input);
	std::cout << "disclaimer";
	Terminal.SetTextColor(Console::Green | Console::Bright);
	std::cout << " as well." << std::endl;

	// Register Modules
	//PushModule<Test>("test");
	//PushModule<Ammo>("ammo");
	//PushModule<Camera>("camera");
	//PushModule<DebugLogging>("debug");
	//PushModule<Fov>("fov");
	//PushModule<Globals>("global");
	//PushModule<Godmode>("god");
	//PushModule<Hud>("hud");
	//PushModule<LoadLevel>("load");
	//PushModule<Spawn>("spawn");
	//PushModule<Information>("info");
	//PushModule<ShowGameUI>("show_ui");
	//PushModule<Disclaimer>("disclaimer");
	//PushModule<ThirdPerson>("third");
	Terminal.PrintLine();
}

DWORD WINAPI dwConsole(LPVOID)

{
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