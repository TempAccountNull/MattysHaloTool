// dll_main.cpp : Defines the entry point for the DLL application.
// TODO: Unlock cursor when using menu.

#include "pch.h"

#include "utils/utils.h"
#include "games/halo-reach/function-hooking/hooks.h"
#include "ui/ui.h"

void hook_ui()
{
	ui::hooking::get_present();

	// If GetPresent failed we have this backup method to get Present Address
	if (!ui::hooking::g_present_hooked) {
		ui::hooking::retrieve_values();
	}

	// After this call, Present should be hooked and controlled by me.
	ui::hooking::detour_directx_present();

	while (!ui::hooking::g_b_initialized) {
		Sleep(1000);
	}

	ui::hooking::detour_directx_draw_indexed();
}

void hook_games()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//Hook game functions
	haloreach::hooks::init_hooks();

	DetourTransactionCommit();
}

int WINAPI main()
{
	//Initialize Console
	//AllocConsole();
	//AttachConsole(GetCurrentProcessId());

	////Redirect output to console
	//freopen("CONIN$", "r", stdin);
	//freopen("CONOUT$", "w", stdout);
	hook_games();
	hook_ui();
}

//BOOL APIENTRY DllMain(HMODULE hModule,
//	DWORD  ul_reason_for_call,
//	LPVOID lpReserved)
//{
//	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
//	switch (ul_reason_for_call)  // NOLINT(hicpp-multiway-paths-covered)
//	{
//	case DLL_PROCESS_ATTACH:
//		// Hook shit and render UI
//		DisableThreadLibraryCalls(hModule);
//		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
//		break;
//	case DLL_THREAD_ATTACH:
//	case DLL_THREAD_DETACH:
//	case DLL_PROCESS_DETACH:
//		utils::DLL_Management::Kill_DLL();
//		break;
//	}
//	return TRUE;
//}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(main), nullptr, NULL, nullptr);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		utils::dll_management::kill_dll();
	}

	return TRUE;
}