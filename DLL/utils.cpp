#include "pch.h"
#include "utils.h"
#include "ui/ui.h"

// Misc Imports
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::string utils::locations::GetCurrentWorkingDir()
{
	char result[MAX_PATH];
	std::string fullpathofdll = std::string(result, GetModuleFileName((HINSTANCE)&__ImageBase, result, MAX_PATH));
	std::string strippedpath = fullpathofdll.substr(0, fullpathofdll.find_last_of("\\/"));
	return strippedpath;
}

void utils::DLL_Management::Kill_DLL()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	// Put unhooking shit here
	ui::hooking::UnhookUI();
	DetourTransactionCommit();

	//Sleep for a lil bit
	Sleep(1000);

	//Detach DLL
	FreeLibraryAndExitThread((HINSTANCE)&__ImageBase, NULL);
}