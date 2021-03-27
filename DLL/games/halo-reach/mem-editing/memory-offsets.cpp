#include "memory-offsets.h"
//Put the addresses of game memory here!

// 1.2094 offsets

int haloreach::memory_offsets::pancam_offset = 0xAED08;

//{
//	// ORIGINAL CODE - INJECTION POINT: "haloreach.dll"+AED08
//
//	"haloreach.dll" + AECD4: 48 8D 4C 24 50 - lea rcx, [rsp + 50]
//		"haloreach.dll" + AECD9 : E8 A6 AF 07 00 - call haloreach.dll + 129C84
//		"haloreach.dll" + AECDE : E8 11 4C 01 00 - call haloreach.dll + C38F4
//		"haloreach.dll" + AECE3 : 84 C0 - test al, al
//		"haloreach.dll" + AECE5 : 0F 84 C7 01 00 00 - je haloreach.dll + AEEB2
//		"haloreach.dll" + AECEB : 65 48 8B 04 25 58 00 00 00 - mov rax, gs : [00000058]
//		"haloreach.dll" + AECF4 : BA E8 00 00 00 - mov edx, 000000E8
//		"haloreach.dll" + AECF9 : 48 8B 04 D8 - mov rax, [rax + rbx * 8]
//		"haloreach.dll" + AECFD : 48 8B 0C 02 - mov rcx, [rdx + rax]
//		"haloreach.dll" + AED01 : 40 38 B9 28 09 00 00 - cmp[rcx + 00000928], dil
//		// ---------- INJECTING HERE ----------
//		"haloreach.dll" + AED08: 0F 84 A4 01 00 00 - je haloreach.dll + AEEB2
//		// ---------- DONE INJECTING  ----------
//		"haloreach.dll" + AED0E : 40 38 B9 2A 09 00 00 - cmp[rcx + 0000092A], dil
//		"haloreach.dll" + AED15: 0F 84 97 01 00 00 - je haloreach.dll + AEEB2
//		"haloreach.dll" + AED1B : 83 C9 FF - or ecx, -01
//		"haloreach.dll" + AED1E : E8 01 AD FA FF - call haloreach.dll + 59A24
//		"haloreach.dll" + AED23 : 8B C8 - mov ecx, eax
//		"haloreach.dll" + AED25 : E8 6A 3C 00 00 - call haloreach.dll + B2994
//		"haloreach.dll" + AED2A : 48 8B D8 - mov rbx, rax
//		"haloreach.dll" + AED2D : 48 85 C0 - test rax, rax
//		"haloreach.dll" + AED30 : 0F 84 7C 01 00 00 - je haloreach.dll + AEEB2
//		"haloreach.dll" + AED36 : 48 8D 4C 24 70 - lea rcx, [rsp + 70]
//}

int haloreach::memory_offsets::infinite_grenades_offset = 0x4FEEF0;

int haloreach::memory_offsets::hs_print_opcode_offset = 0x8DB910;


//{
//	// ORIGINAL CODE - INJECTION POINT: "haloreach.dll"+8DE018 - hs_chud_post_message_hack
//
//	"haloreach.dll" + 8DDFFF: 00 58 F5 - add[rax - 0B], bl
//		"haloreach.dll" + 8DE002 : FD - std
//		"haloreach.dll" + 8DE003 : EF - out dx, eax
//		"haloreach.dll" + 8DE004 : FD - std
//		"haloreach.dll" + 8DE005 : 7F 00 - jg haloreach.dll + 8DE007
//		"haloreach.dll" + 8DE007 : 00 80 F5 FD EF FD - add[rax - 0210020B], al
//		"haloreach.dll" + 8DE00D : 7F 00 - jg haloreach.dll + 8DE00F
//		"haloreach.dll" + 8DE00F : 00 B8 F4 FD EF FD - add[rax - 0210020C], bh
//		"haloreach.dll" + 8DE015 : 7F 00 - jg haloreach.dll + 8DE017
//		"haloreach.dll" + 8DE017 : 00 - db 00  // SHORTENED TO HIT INJECTION FROM:  add al,ah
//		// ---------- INJECTING HERE ----------
//		"haloreach.dll" + 8DE018 : E0 F4 - loopne haloreach.dll + 8DE00E
//		"haloreach.dll" + 8DE01A : FD - std
//		"haloreach.dll" + 8DE01B : EF - out dx, eax
//		"haloreach.dll" + 8DE01C : FD - std
//		// ---------- DONE INJECTING  ----------
//		"haloreach.dll" + 8DE01D : 7F 00 - jg haloreach.dll + 8DE01F
//		"haloreach.dll" + 8DE01F : 00 08 - add[rax], cl
//		"haloreach.dll" + 8DE021: F5 - cmc
//		"haloreach.dll" + 8DE022 : FD - std
//		"haloreach.dll" + 8DE023 : EF - out dx, eax
//		"haloreach.dll" + 8DE024 : FD - std
//		"haloreach.dll" + 8DE025 : 7F 00 - jg haloreach.dll + 8DE027
//		"haloreach.dll" + 8DE027 : 00 40 F4 - add[rax - 0C], al
//		"haloreach.dll" + 8DE02A : FD - std
//		"haloreach.dll" + 8DE02B : EF - out dx, eax
//}

//{
//	// ORIGINAL CODE - INJECTION POINT: "haloreach.dll"+8DB910 - hs_print
//
//	"haloreach.dll" + 8DB901: B3 FE - mov bl, -02
//		"haloreach.dll" + 8DB903 : EF - out dx, eax
//		"haloreach.dll" + 8DB904 : FD - std
//		"haloreach.dll" + 8DB905 : 7F 00 - jg haloreach.dll + 8DB907
//		"haloreach.dll" + 8DB907 : 00 30 - add[rax], dh
//		"haloreach.dll" + 8DB909 : B3 FE - mov bl, -02
//		"haloreach.dll" + 8DB90B : EF - out dx, eax
//		"haloreach.dll" + 8DB90C : FD - std
//		"haloreach.dll" + 8DB90D : 7F 00 - jg haloreach.dll + 8DB90F
//		"haloreach.dll" + 8DB90F : 00 - db 00  // SHORTENED TO HIT INJECTION FROM:  add [rax],bl
//		// ---------- INJECTING HERE ----------
//		"haloreach.dll" + 8DB910 : 18 B2 FE EF FD 7F - sbb[rdx + 7FFDEFFE], dh
//		// ---------- DONE INJECTING  ----------
//		"haloreach.dll" + 8DB916 : 00 00 - add[rax], al
//		"haloreach.dll" + 8DB918 : 40 B2 FE - mov dl, -02
//		"haloreach.dll" + 8DB91B : EF - out dx, eax
//		"haloreach.dll" + 8DB91C : FD - std
//		"haloreach.dll" + 8DB91D : 7F 00 - jg haloreach.dll + 8DB91F
//		"haloreach.dll" + 8DB91F : 00 68 B2 - add[rax - 4E], ch
//		"haloreach.dll" + 8DB922 : FE - db - 02
//		"haloreach.dll" + 8DB923 : EF - out dx, eax
//		"haloreach.dll" + 8DB924 : FD - std
//		"haloreach.dll" + 8DB925 : 7F 00 - jg haloreach.dll + 8DB927
//}