#include "pch.h"

bool HaloReach::Time::SlowMotion = false;

//object_set_scale
typedef __int64 __fastcall unit_start_running_blindly(unsigned __int16 a1);
unit_start_running_blindly* do_weird_things = (unit_start_running_blindly*)((char*)GetModuleHandle(L"haloreach.dll") + 0x484ED8);

//unit_update_damage
static char __fastcall unit_update_damage(unsigned int a1);
static decltype(unit_update_damage)* unit_update_damage__original = nullptr;

void unit_update_damage_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle(L"haloreach.dll") + 0x472B30);
	unit_update_damage__original = reinterpret_cast<decltype(unit_update_damage)*>(pointer);
	DetourAttach((PVOID*)&unit_update_damage__original, unit_update_damage);
}

void unit_update_damage_dispose()
{
	DetourDetach((PVOID*)&unit_update_damage__original, unit_update_damage);
}

char __fastcall unit_update_damage(unsigned int a1)
{

	auto result = unit_update_damage__original(a1);
	do_weird_things(a1);
	return result;
}

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
	if (HaloReach::Time::SlowMotion)
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

void HaloReach::Hooks::init_hooks()
{
	game_update_hook();
	unit_update_damage_hook();
}



void HaloReach::Hooks::deinit_hooks()
{
	game_update_dispose();
	unit_update_damage_dispose();
}

