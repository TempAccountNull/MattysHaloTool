#include "function_offsets.h"

//Put the addresses of game functions here!
//TODO: Find a good automatic way to find updated offsets when game updates. (Pattern scanning is a possibility)

// 1.2094 offsets

int haloreach::function_offsets::unit_start_running_blindly_offset = 0x484ED8;

int haloreach::function_offsets::unit_update_offset = 0x4732E4;

int haloreach::function_offsets::game_update_offset = 0x49F7C;

int haloreach::function_offsets::actor_perception_set_target_offset = 0x5E1BAC;

int haloreach::function_offsets::weapon_has_infinite_ammo_offset = 0x49E81C;

int haloreach::function_offsets::weapon_barrel_fire_weapon_heat_offset = 0x49A008;

int haloreach::function_offsets::weapon_barrel_create_projectiles_offset = 0x49B324;

int haloreach::function_offsets::player_index_from_unit_index_offset = 0x5CB6C;

int haloreach::function_offsets::game_time_set_rate_scale_direct_offset = 0x5251C;

int haloreach::function_offsets::game_time_get_speed_offset = 0x51DB8;

int haloreach::function_offsets::weapon_get_owner_unit_index_offset = 0x496D00;

int haloreach::function_offsets::player_control_machinima_available_offset = 0xC38F4;