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