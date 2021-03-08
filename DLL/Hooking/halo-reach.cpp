#include "pch.h"

//TODO: this code is a real mess, should tidy it up sometime.
//NOTE: std::cout << "\n function(" << a1 << "," << a2 << "," << a3 << ")";

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

	return result;
}

// End of functions

void HaloReach::Hooks::init_hooks()
{
	game_update_hook();
}

void HaloReach::Hooks::deinit_hooks()
{
	game_update_dispose();
}

