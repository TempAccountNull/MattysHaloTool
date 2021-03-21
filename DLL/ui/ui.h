#pragma once
namespace ui
{
	class hooking
	{
	public:
		static bool g_PresentHooked;
		static bool g_bInitialised;
		static void retrieveValues();
		static void detourDirectXPresent();
		static void UnhookUI();
	};
}