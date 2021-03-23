// dllmain.cpp : Defines the entry point for the DLL application.
// TODO: Unlock cursor when using menu.

#include "pch.h"

#include "utils.h"
#include "games/halo-reach/hooks.h"
#include "ui/ui.h"

void Hook_UI()
{
	ui::hooking::retrieveValues();

	// After this call, Present should be hooked and controlled by me.
	ui::hooking::detourDirectXPresent();

	while (!ui::hooking::g_bInitialised) {
		Sleep(1000);
	}

	ui::hooking::detourDirectXDrawIndexed();
}

void Hook_Games()
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
	Hook_Games();
	Hook_UI();
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
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		utils::DLL_Management::Kill_DLL();
	}

	return TRUE;
}