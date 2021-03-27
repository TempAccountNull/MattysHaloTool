// ReSharper disable All
#include "pch.h"
#include "memory.h"
#include "../../../utils/memory_editing.h"

#include "memory-offsets.h"

//init vars
bool haloreach::memory::pancam_mode = false;
bool haloreach::memory::inf_grenades = false;
bool haloreach::memory::redirect_print = false;

void haloreach::memory::toggle_pancam()
{
	BYTE pancam_on[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x40, 0x38, 0xB9, 0x2A, 0x09, 00, 00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, };
	BYTE pancam_off[] = { 0x0F, 0x84, 0xA4, 0x01, 0x00, 0x00, 0x40, 0x38, 0xB9, 0x2A, 0x09, 0x00, 0x00, 0x0F, 0x84, 0x97, 0x01, 0x00, 0x00, };
	char* lp_base_address = reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + memory_offsets::pancam_offset;
	if (!pancam_mode) {
		utils::memory::Patch(lp_base_address, reinterpret_cast<char*>(&pancam_off), 19);
	}
	else
	{
		utils::memory::Patch(lp_base_address, reinterpret_cast<char*>(&pancam_on), 19);
	}
}



void haloreach::memory::toggle_inf_grenades()
{
	BYTE grenades_off[] = { 0x2A, 0xC3 };
	char* lp_base_address = reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + memory_offsets::infinite_grenades_offset;
	if (!inf_grenades) {
		utils::memory::Patch(lp_base_address, reinterpret_cast<char*>(&grenades_off), 2);
	}
	else
	{
		utils::memory::Nop(lp_base_address, 2);
	}
}

void haloreach::memory::toggle_redirect_print()
{
	BYTE print_on[] = { 0xE0, 0xF4, 0x64 };
	BYTE print_off[] = { 0x18, 0xB2, 0x65 };


	char* lp_base_address = reinterpret_cast<char*>(GetModuleHandle("haloreach.dll")) + memory_offsets::hs_print_opcode_offset;
	if (!redirect_print) {
		utils::memory::Patch(lp_base_address, reinterpret_cast<char*>(&print_off), 3);
	}
	else
	{
		utils::memory::Patch(lp_base_address, reinterpret_cast<char*>(&print_on), 3);
	}
}