#include "pch.h"
#include "hooks.h"
#include "offsets.h"

//TODO: Hook game functions here!

//Bools
bool haloreach::hooks::ai_go_crazy = false;
bool haloreach::hooks::infinite_ammo = false;

//player_index_from_unit_index
typedef int __fastcall player_index_from_unit_index(int a1);
player_index_from_unit_index* player_index_from_unit_index_og = (player_index_from_unit_index*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::player_index_from_unit_index_offset);

//unit_start_running_blindly
typedef __int64 __fastcall unit_start_running_blindly(unsigned __int16 unit);
unit_start_running_blindly* run_blindly = (unit_start_running_blindly*)((char*)GetModuleHandle("haloreach.dll") + haloreach::offsets::unit_start_running_blindly_offset);

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
	unit_update_hook();
	weapon_has_infinite_ammo_hook();
}

void haloreach::hooks::deinit_hooks()
{
	unit_update_dispose();
	weapon_has_infinite_ammo_dispose();
}