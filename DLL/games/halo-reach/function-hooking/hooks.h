#pragma once

namespace haloreach
{
	class hooks
	{
	public:
		static void init_hooks();
		static void deinit_hooks();
		static void reinit_hooks();
		static bool ai_go_crazy;
		static bool infinite_ammo;
		static bool no_overheat;
		static bool player_proj_only;
		static bool ai_null_perception;

		#if defined _DEBUG
		static bool test;
		#endif
		
		static float game_speed;
	};
}