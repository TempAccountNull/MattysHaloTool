#include "pch.h"

//TODO: this code is a real mess, should tidy it up sometime.
//NOTE: std::cout << "\n function(" << a1 << "," << a2 << "," << a3 << ")";
// hs_objects_can_see_object__original(object,objecttosee,degrees);

// Get player
unsigned int player = 0;

//object_set_in_limbo
typedef char __fastcall object_set_hidden(int a1, bool a2);
object_set_hidden* actor_activity_reset_og = (object_set_hidden*)((char*)GetModuleHandle(L"haloreach.dll") + 0x4633B4);

//object_kill
typedef __int64 __fastcall object_kill(unsigned int a1, char a2, char a3, char a4, char a5);
object_kill* object_kill_og = (object_kill*)((char*)GetModuleHandle(L"haloreach.dll") + 0x46E9C0);

//hs_objects_can_see_object
typedef char __fastcall hs_objects_can_see_object(int a1, unsigned int a2, float a3);
hs_objects_can_see_object* hs_objects_can_see_object_og = (hs_objects_can_see_object*)((char*)GetModuleHandle(L"haloreach.dll") + 0x1F2320);

//player_index_from_unit_index
typedef int __fastcall player_index_from_unit_index(int a1);
player_index_from_unit_index* player_index_from_unit_index_og = (player_index_from_unit_index*)((char*)GetModuleHandle(L"haloreach.dll") + 0x5CB6C);

//unit_update
static char __fastcall unit_update(unsigned int a1, __int64 a2, __int64 a3, float a4);
static decltype(unit_update)* unit_update__original = nullptr;

void unit_update_hook()
{
	long long* pointer = reinterpret_cast<long long*>((char*)GetModuleHandle(L"haloreach.dll") + 0x4732E4);
	unit_update__original = reinterpret_cast<decltype(unit_update)*>(pointer);
	DetourAttach((PVOID*)&unit_update__original, unit_update);
}

void unit_update_dispose()
{
	DetourDetach((PVOID*)&unit_update__original, unit_update);
}

char __fastcall unit_update(unsigned int a1, __int64 a2, __int64 a3, float a4)
{

	
	// If unit does not have a player index, do shit.

	int unit_is_player = player_index_from_unit_index_og(a1);

	if (unit_is_player != -1)
	{
		player = a1;
	}
	else
	{
		// Can player see unit?
		if (hs_objects_can_see_object_og(player, a1, 90.0))
		{
			actor_activity_reset_og(a1,true);
		}
		else
		{
			actor_activity_reset_og(a1, false);
		}
	}
	
	char result = unit_update__original(a1,a2,a3,a4);

	return result;
}

// End of functions

void HaloReach::Hooks::init_hooks()
{
	unit_update_hook();
}

void HaloReach::Hooks::deinit_hooks()
{
	unit_update_dispose();
}