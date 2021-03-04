#include "pch.h"
bool HaloReach::time::slowmotion = false;

//game_rate
typedef __int64 __fastcall game_rate(float a1);
game_rate* change_game_speed = (game_rate*)((char*)GetModuleHandle(L"haloreach.dll") + 0x5251C);

// Game_Update
static __int64 __fastcall game_update(int tick_count, float* game_seconds_elapsed);
static decltype(game_update)* game_update__original = nullptr;

void game_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle(L"haloreach.dll") + 0x49F7C);
	game_update__original = reinterpret_cast<decltype(game_update)*>(pointer);
	DetourAttach((PVOID*)&game_update__original, game_update);
}

void game_update_dispose()
{
	DetourDetach((PVOID*)&game_update__original, game_update);
}

__int64 __fastcall game_update(int tick_count, float* game_seconds_elapsed)
{
	// TODO: any pre-update logic here

	//std::cout << tick_count << *game_seconds_elapsed;

	auto result = game_update__original(tick_count, game_seconds_elapsed);

	// TODO: any post-update logic here
	if (HaloReach::time::slowmotion)
	{
		change_game_speed(0.1);
	}
	else
	{
		change_game_speed(1.0);
	}

	return result;
}

// End of functions

void HaloReach::hooks::init_hooks()
{
	game_update_hook();
}



void HaloReach::hooks::deinit_hooks()
{
	game_update_dispose();
}

