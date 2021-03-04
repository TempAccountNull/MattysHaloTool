#include <windows.h>

typedef float* __fastcall hs_game_rate_evaluate(int a1, unsigned int a2, int a3, int a4);
hs_game_rate_evaluate* change_game_speed = (hs_game_rate_evaluate*)(GetModuleHandle(L"haloreach.dll") + 0x21FD20);

void init_hooks()
{
	game_update__original = main_get_typed_module_address<decltype(game_update__original)>(0x29F7C0);

	DetourAttach((PVOID*)&game_update__original, game_update);

}