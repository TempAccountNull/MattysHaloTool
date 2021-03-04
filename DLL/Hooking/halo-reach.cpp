#include "pch.h"


static __int64 __fastcall game_update(int tick_count, float* game_seconds_elapsed);
static decltype(game_update)* game_update__original = nullptr;

void HaloReach::hooks::init_hooks()
{
	long long* pointer = reinterpret_cast<long long*>(GetModuleHandle(L"haloreach.dll") + 0x49F7C);
	game_update__original = reinterpret_cast<decltype(game_update)*>(pointer);
	DetourAttach((PVOID*)&game_update__original, game_update);

}

void HaloReach::hooks::deinit_hooks()
{
	DetourDetach((PVOID*)&game_update__original, game_update);
}


__int64 __fastcall game_update(int tick_count, float* game_seconds_elapsed)
{
	// TODO: any pre-update logic here

	std::cout << tick_count << game_seconds_elapsed;
	auto result = game_update__original(tick_count, game_seconds_elapsed);

	// TODO: any post-update logic here

	return result;
}