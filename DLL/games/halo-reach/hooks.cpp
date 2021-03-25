#include "pch.h"
#include "hooks.h"
#include "offsets.h"

//NOTE: If you get a stack overflow error, you might be using the wrong offset for a function.

//Bools
bool haloreach::hooks::ai_go_crazy = false;
bool haloreach::hooks::infinite_ammo = false;
bool haloreach::hooks::no_overheat = false;
bool haloreach::hooks::player_proj_only = false;
bool haloreach::hooks::ai_null_perception = false;

//Floats
float haloreach::hooks::game_speed = 1.0;

// Function Calls

//game_time_get_speed
typedef float __fastcall game_time_get_speed();
game_time_get_speed* game_time_get_speed_og = reinterpret_cast<game_time_get_speed*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) +
	haloreach::offsets::game_time_get_speed_offset);

//game_time_set_rate_scale_direct
typedef __int64 __fastcall game_time_set_rate_scale_direct(float a1);
game_time_set_rate_scale_direct* game_time_set_rate_scale_direct_og = reinterpret_cast<game_time_set_rate_scale_direct*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) +
	haloreach::offsets::game_time_set_rate_scale_direct_offset);

//weapon_get_owner_unit_index
typedef int __fastcall weapon_get_owner_unit_index(int a1);
weapon_get_owner_unit_index* weapon_get_owner_unit_index_og = reinterpret_cast<weapon_get_owner_unit_index*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) +
	haloreach::offsets::weapon_get_owner_unit_index_offset);

//player_index_from_unit_index
typedef int __fastcall player_index_from_unit_index(int a1);
player_index_from_unit_index* player_index_from_unit_index_og = reinterpret_cast<player_index_from_unit_index*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) +
	haloreach::offsets::player_index_from_unit_index_offset);

//unit_start_running_blindly
typedef int __fastcall unit_start_running_blindly(int unit);
unit_start_running_blindly* run_blindly = reinterpret_cast<unit_start_running_blindly*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) +
	haloreach::offsets::unit_start_running_blindly_offset);

/// Function Hooks

//actor_perception_set_target
static void __fastcall actor_perception_set_target(int a1, int a2);
static decltype(actor_perception_set_target)* actor_perception_set_target_original = nullptr;

void actor_perception_set_target_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::actor_perception_set_target_offset);
	actor_perception_set_target_original = reinterpret_cast<decltype(actor_perception_set_target)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&actor_perception_set_target_original), actor_perception_set_target);
}

void actor_perception_set_target_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&actor_perception_set_target_original), actor_perception_set_target);
}

void __fastcall actor_perception_set_target(const int a1, const int a2)
{
	if (!haloreach::hooks::ai_null_perception)
	{
		actor_perception_set_target_original(a1, a2);
	}
}

//weapon_barrel_create_projectiles
static void __fastcall weapon_barrel_create_projectiles(int a1, __int16 a2, const struct s_predicted_weapon_fire_data near* a3, bool a4, bool a5);
static decltype(weapon_barrel_create_projectiles)* weapon_barrel_create_projectiles_original = nullptr;

void weapon_barrel_create_projectiles_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::weapon_barrel_create_projectiles_offset);
	weapon_barrel_create_projectiles_original = reinterpret_cast<decltype(weapon_barrel_create_projectiles)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&weapon_barrel_create_projectiles_original), weapon_barrel_create_projectiles);
}

void weapon_barrel_create_projectiles_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&weapon_barrel_create_projectiles_original), weapon_barrel_create_projectiles);
}

void __fastcall weapon_barrel_create_projectiles(const int a1, const __int16 a2, const struct s_predicted_weapon_fire_data near* a3, const bool a4, const bool a5)
{
	if (haloreach::hooks::player_proj_only)
	{
		//Check if unit firing weapon is player.
		const int unit = weapon_get_owner_unit_index_og(a1);
		const int unit_player_index = player_index_from_unit_index_og(unit);
		if (unit_player_index != -1)
		{
			weapon_barrel_create_projectiles_original(a1, a2, a3, a4, a5);
		}
	}
	else
	{
		weapon_barrel_create_projectiles_original(a1, a2, a3, a4, a5);
	}
}

//weapon_barrel_fire_weapon_heat
static void __fastcall weapon_barrel_fire_weapon_heat(int a1, int a2);
static decltype(weapon_barrel_fire_weapon_heat)* weapon_barrel_fire_weapon_heat_original = nullptr;

void weapon_barrel_fire_weapon_heat_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::weapon_barrel_fire_weapon_heat_offset);
	weapon_barrel_fire_weapon_heat_original = reinterpret_cast<decltype(weapon_barrel_fire_weapon_heat)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&weapon_barrel_fire_weapon_heat_original), weapon_barrel_fire_weapon_heat);
}

void weapon_barrel_fire_weapon_heat_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&weapon_barrel_fire_weapon_heat_original), weapon_barrel_fire_weapon_heat);
}

void __fastcall weapon_barrel_fire_weapon_heat(const int a1, const int a2)
{
	if (!haloreach::hooks::no_overheat)
	{
		weapon_barrel_fire_weapon_heat_original(a1, a2);
	}
}

//weapon_has_infinite_ammo
static bool __fastcall weapon_has_infinite_ammo(int a1);
static decltype(weapon_has_infinite_ammo)* weapon_has_infinite_ammo_original = nullptr;

void weapon_has_infinite_ammo_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::weapon_has_infinite_ammo_offset);
	weapon_has_infinite_ammo_original = reinterpret_cast<decltype(weapon_has_infinite_ammo)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&weapon_has_infinite_ammo_original), weapon_has_infinite_ammo);
}

void weapon_has_infinite_ammo_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&weapon_has_infinite_ammo_original), weapon_has_infinite_ammo);
}

bool __fastcall weapon_has_infinite_ammo(const int a1)
{
	if (!haloreach::hooks::infinite_ammo)
	{
		const bool result = weapon_has_infinite_ammo_original(a1);
		return result;
	}

	return true;
}

//game_update
static void __fastcall game_update(int a1, float near* a2);
static decltype(game_update)* game_update_original = nullptr;

void game_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::game_update_offset);
	game_update_original = reinterpret_cast<decltype(game_update)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&game_update_original), game_update);
}

void game_update_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&game_update_original), game_update);
}

void __fastcall game_update(const int a1, float near* a2)
{
	//TODO: Possibly not put these things in a constant loop?
	game_time_set_rate_scale_direct_og(haloreach::hooks::game_speed);

	game_update_original(a1, a2);
}

//unit_update - great for making units do things
static bool __fastcall unit_update(int a1);
static decltype(unit_update)* unit_update_original = nullptr;

void unit_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>(reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + haloreach::offsets::unit_update_offset);
	unit_update_original = reinterpret_cast<decltype(unit_update)*>(pointer);
	DetourAttach(reinterpret_cast<PVOID*>(&unit_update_original), unit_update);
}

void unit_update_dispose()
{
	DetourDetach(reinterpret_cast<PVOID*>(&unit_update_original), unit_update);
}

bool __fastcall unit_update(const int a1)
{
	const char result = unit_update_original(a1);  // NOLINT(bugprone-narrowing-conversions, cppcoreguidelines-narrowing-conversions)

	if (haloreach::hooks::ai_go_crazy)
	{
		const int unit_player_index = player_index_from_unit_index_og(a1);
		// If unit does not have a player index, do shit.
		if (unit_player_index == -1)
		{
			run_blindly(a1);
		}
	}

	return result;
}

void haloreach::hooks::init_hooks()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	//
	game_update_hook();
	unit_update_hook();
	weapon_has_infinite_ammo_hook();
	weapon_barrel_fire_weapon_heat_hook();
	weapon_barrel_create_projectiles_hook();
	actor_perception_set_target_hook();
	//
	DetourTransactionCommit();
}

void haloreach::hooks::deinit_hooks()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	//
	game_update_dispose();
	unit_update_dispose();
	weapon_has_infinite_ammo_dispose();
	weapon_barrel_fire_weapon_heat_dispose();
	weapon_barrel_create_projectiles_dispose();
	actor_perception_set_target_dispose();
	//
	DetourTransactionCommit();
}

void haloreach::hooks::reinit_hooks()
{
	deinit_hooks();
	Sleep(1000);
	init_hooks();
}