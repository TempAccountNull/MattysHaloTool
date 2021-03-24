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


//game_time_get_speed
typedef float __fastcall game_time_get_speed();
game_time_get_speed* game_time_get_speed_og = (game_time_get_speed*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::game_time_get_speed_offset);

//game_time_set_rate_scale_direct
typedef __int64 __fastcall game_time_set_rate_scale_direct(float a1);
game_time_set_rate_scale_direct* game_time_set_rate_scale_direct_og = (game_time_set_rate_scale_direct*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::game_time_set_rate_scale_direct_offset);

//weapon_get_owner_unit_index
typedef int __fastcall weapon_get_owner_unit_index(int a1);
weapon_get_owner_unit_index* weapon_get_owner_unit_index_og = (weapon_get_owner_unit_index*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::weapon_get_owner_unit_index_offset);

//player_index_from_unit_index
typedef int __fastcall player_index_from_unit_index(int a1);
player_index_from_unit_index* player_index_from_unit_index_og = (player_index_from_unit_index*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::player_index_from_unit_index_offset);

//unit_start_running_blindly
typedef int __fastcall unit_start_running_blindly(int unit);
unit_start_running_blindly* run_blindly = (unit_start_running_blindly*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::unit_start_running_blindly_offset);

//actor_perception_set_target
static void __fastcall actor_perception_set_target(int a1, int a2);
static decltype(actor_perception_set_target)* actor_perception_set_target__original = nullptr;

void actor_perception_set_target_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::actor_perception_set_target_offset);
	actor_perception_set_target__original = reinterpret_cast<decltype(actor_perception_set_target)*>(pointer);
	DetourAttach((PVOID*)&actor_perception_set_target__original, actor_perception_set_target);
}

void actor_perception_set_target_dispose()
{
	DetourDetach((PVOID*)&actor_perception_set_target__original, actor_perception_set_target);
}

void __fastcall actor_perception_set_target(int a1, int a2)
{
	if (!haloreach::hooks::ai_null_perception)
	{
		actor_perception_set_target__original(a1, a2);
	}
}

//weapon_barrel_create_projectiles
static void __fastcall weapon_barrel_create_projectiles(int a1, __int16 a2, const struct s_predicted_weapon_fire_data near* a3, bool a4, bool a5);
static decltype(weapon_barrel_create_projectiles)* weapon_barrel_create_projectiles__original = nullptr;

void weapon_barrel_create_projectiles_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::weapon_barrel_create_projectiles_offset);
	weapon_barrel_create_projectiles__original = reinterpret_cast<decltype(weapon_barrel_create_projectiles)*>(pointer);
	DetourAttach((PVOID*)&weapon_barrel_create_projectiles__original, weapon_barrel_create_projectiles);
}

void weapon_barrel_create_projectiles_dispose()
{
	DetourDetach((PVOID*)&weapon_barrel_create_projectiles__original, weapon_barrel_create_projectiles);
}

void __fastcall weapon_barrel_create_projectiles(int a1, __int16 a2, const struct s_predicted_weapon_fire_data near* a3, bool a4, bool a5)
{
	if (haloreach::hooks::player_proj_only)
	{
		//Check if unit firing weapon is player.
		int unit = weapon_get_owner_unit_index_og(a1);
		int unit_player_index = player_index_from_unit_index_og(unit);
		if (unit_player_index != -1)
		{
			weapon_barrel_create_projectiles__original(a1, a2, a3, a4, a5);
		}
	}
	else
	{
		weapon_barrel_create_projectiles__original(a1, a2, a3, a4, a5);
	}
}

//weapon_barrel_fire_weapon_heat
static void __fastcall weapon_barrel_fire_weapon_heat(int a1, int a2);
static decltype(weapon_barrel_fire_weapon_heat)* weapon_barrel_fire_weapon_heat__original = nullptr;

void weapon_barrel_fire_weapon_heat_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::weapon_barrel_fire_weapon_heat_offset);
	weapon_barrel_fire_weapon_heat__original = reinterpret_cast<decltype(weapon_barrel_fire_weapon_heat)*>(pointer);
	DetourAttach((PVOID*)&weapon_barrel_fire_weapon_heat__original, weapon_barrel_fire_weapon_heat);
}

void weapon_barrel_fire_weapon_heat_dispose()
{
	DetourDetach((PVOID*)&weapon_barrel_fire_weapon_heat__original, weapon_barrel_fire_weapon_heat);
}

void __fastcall weapon_barrel_fire_weapon_heat(int a1, int a2)
{
	if (!haloreach::hooks::no_overheat)
	{
		weapon_barrel_fire_weapon_heat__original(a1, a2);
	}
}

//weapon_has_infinite_ammo
static bool __fastcall weapon_has_infinite_ammo(int a1);
static decltype(weapon_has_infinite_ammo)* weapon_has_infinite_ammo__original = nullptr;

void weapon_has_infinite_ammo_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::weapon_has_infinite_ammo_offset);
	weapon_has_infinite_ammo__original = reinterpret_cast<decltype(weapon_has_infinite_ammo)*>(pointer);
	DetourAttach((PVOID*)&weapon_has_infinite_ammo__original, weapon_has_infinite_ammo);
}

void weapon_has_infinite_ammo_dispose()
{
	DetourDetach((PVOID*)&weapon_has_infinite_ammo__original, weapon_has_infinite_ammo);
}

bool __fastcall weapon_has_infinite_ammo(int a1)
{
	if (!haloreach::hooks::infinite_ammo)
	{
		bool result = weapon_has_infinite_ammo__original(a1);
		return result;
	}

	return true;
}

//game_update
static void __fastcall game_update(int a1, float near* a2);
static decltype(game_update)* game_update__original = nullptr;

void game_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::game_update_offset);
	game_update__original = reinterpret_cast<decltype(game_update)*>(pointer);
	DetourAttach((PVOID*)&game_update__original, game_update);
}

void game_update_dispose()
{
	DetourDetach((PVOID*)&game_update__original, game_update);
}

void __fastcall game_update(int a1, float near* a2)
{

	//TODO: Possibly not put these things in a constant loop?
	game_time_set_rate_scale_direct_og(haloreach::hooks::game_speed);
	
	game_update__original(a1,a2);
}

//unit_update - great for making units do things
static bool __fastcall unit_update(int a1);
static decltype(unit_update)* unit_update__original = nullptr;

void unit_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::unit_update_offset);
	unit_update__original = reinterpret_cast<decltype(unit_update)*>(pointer);
	DetourAttach((PVOID*)&unit_update__original, unit_update);
}

void unit_update_dispose()
{
	DetourDetach((PVOID*)&unit_update__original, unit_update);
}

bool __fastcall unit_update(int a1)
{
	char result = unit_update__original(a1);

	if (haloreach::hooks::ai_go_crazy)
	{
		int unit_player_index = player_index_from_unit_index_og(a1);
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
	game_update_hook();
	unit_update_hook();
	weapon_has_infinite_ammo_hook();
	weapon_barrel_fire_weapon_heat_hook();
	weapon_barrel_create_projectiles_hook();
	actor_perception_set_target_hook();
}

void haloreach::hooks::deinit_hooks()
{
	game_update_dispose();
	unit_update_dispose();
	weapon_has_infinite_ammo_dispose();
	weapon_barrel_fire_weapon_heat_dispose();
	weapon_barrel_create_projectiles_dispose();
	actor_perception_set_target_dispose();
}