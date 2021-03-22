#pragma once

namespace haloreach
{
	class hooks
	{
	public:
		static void init_hooks();
		static void deinit_hooks();
		static bool ai_go_crazy;
	};
}