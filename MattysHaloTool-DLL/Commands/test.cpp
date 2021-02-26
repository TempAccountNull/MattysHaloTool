#include "test.h"

#include <libloaderapi.h>
typedef float* __fastcall hs_game_rate_evaluate(int a1, unsigned int a2, int a3, int a4);
hs_game_rate_evaluate* change_game_speed = (hs_game_rate_evaluate*)(GetModuleHandle(L"haloreach.dll") + 0x21FD20);


void TestCMD::test::do_something()
{
	change_game_speed(2, 2, 2, 2);
}
