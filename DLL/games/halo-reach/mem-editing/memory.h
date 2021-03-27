#pragma once
namespace haloreach
{
	class memory
	{
	public:
		static bool pancam_mode;
		static bool inf_grenades;
		static bool redirect_print;
		static bool ai_spawning_mp_enabled;
		static void toggle_pancam();
		static void toggle_inf_grenades();
		static void toggle_redirect_print();
		static void toggle_ai_spawning_mp();
	};
}