#include "pch.h"

//TODO: this code is a real mess, should tidy it up sometime.
//NOTE: std::cout << "\n function(" << a1 << "," << a2 << "," << a3 << ")";
bool HaloReach::Time::SlowMotion = false;

//unit_start_running_blindly
typedef int __fastcall player_index_from_unit_index(int a1);
player_index_from_unit_index* player_index_from_unit_index_og = (player_index_from_unit_index*)((char*)GetModuleHandle(L"haloreach.dll") + 0x5CB6C);

//unit_start_running_blindly
typedef __int64 __fastcall unit_start_running_blindly(unsigned __int16 unit);
unit_start_running_blindly* run_blindly = (unit_start_running_blindly*)((char*)GetModuleHandle(L"haloreach.dll") + 0x484ED8);

//unit_update_damage - great for making units do things
static char __fastcall unit_update_damage(unsigned int unit);
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

char __fastcall unit_update_damage(unsigned int unit)
{
	char result = unit_update_damage__original(unit);

	int unit_player_index = player_index_from_unit_index_og(unit);
	// If unit does not have a player index, do shit.
	if (unit_player_index == -1)
	{
		run_blindly(unit);
	}
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
	//game_update_hook();
	unit_update_damage_hook();
}

void HaloReach::Hooks::deinit_hooks()
{
	//game_update_dispose();
	unit_update_damage_dispose();
}

