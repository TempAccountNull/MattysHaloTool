#include "offsets.h"

//Put the addresses of game functions here!
//TODO: Find a good automatic way to find updated offsets when game updates. (Pattern scanning is a possibility)

// 1.2094 offsets

int haloreach::offsets::unit_start_running_blindly_offset = 0x484ED8;

int haloreach::offsets::unit_update_offset = 0x4732E4;

int haloreach::offsets::weapon_has_infinite_ammo_offset = 0x49E81C;

int haloreach::offsets::weapon_barrel_fire_weapon_heat_offset = 0x49A008;

int haloreach::offsets::weapon_barrel_create_projectiles_offset = 0x49B324;

int haloreach::offsets::player_index_from_unit_index_offset = 0x5CB6C;

int haloreach::offsets::weapon_get_owner_unit_index_offset = 0x496D00;