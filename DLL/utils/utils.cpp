#include "pch.h"
#include "utils.h"

#include "../games/halo-reach/function-hooking/hooks.h"
#include "../ui/ui.h"

// Misc Imports
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::string utils::locations::get_current_working_dir()
{
	char result[MAX_PATH];
	const std::string fullpathofdll = std::string(result, GetModuleFileName(reinterpret_cast<HINSTANCE>(&__ImageBase), result, MAX_PATH));
	std::string strippedpath = fullpathofdll.substr(0, fullpathofdll.find_last_of("\\/"));
	return strippedpath;
}

void utils::dll_management::kill_dll()
{
	// Put unhooking shit here
	ui::hooking::unhook_ui();
	haloreach::hooks::deinit_hooks();

	//Sleep for a lil bit
	Sleep(1000);

	//Detach DLL
	FreeLibraryAndExitThread(reinterpret_cast<HINSTANCE>(&__ImageBase), NULL);
}