// dllmain.cpp : Defines the entry point for the DLL application.
// TODO: Make a hook for the games graphics api, aka DX11 and integrate nuklear
// https://github.com/Immediate-Mode-UI/Nuklear

#include "pch.h"

void Hook_UI()
{
	ui::hooking::GetPresent();

	// If GetPresent failed we have this backup method to get Present Address
	if (!ui::hooking::g_PresentHooked) {
		ui::hooking::retrieveValues();
	}
    

	// After this call, Present should be hooked and controlled by me.
	ui::hooking::detourDirectXPresent();
	while (!ui::hooking::g_bInitialised) {
		Sleep(1000);
	}
}

int WINAPI main()
{
    Hook_UI();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
)
{
	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	switch (ul_reason_for_call)  // NOLINT(hicpp-multiway-paths-covered)
    {
    case DLL_PROCESS_ATTACH:
        // Hook shit and render UI
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)main, NULL, NULL, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		// Unhook shit and get rid of UI
        ui::hooking::UnhookUI();
        break;
    }
    return TRUE;
}

