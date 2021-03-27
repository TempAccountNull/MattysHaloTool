#pragma once
namespace ui
{
	class hooking
	{
	public:
		static bool g_present_hooked;
		static bool g_b_initialized;
		static void retrieve_values();
		static void detour_directx_present();
		static void detour_directx_draw_indexed();
		static void get_present();
		static void unhook_ui();
	};
}