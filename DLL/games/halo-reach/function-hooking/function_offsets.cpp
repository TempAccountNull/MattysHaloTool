#include "function_offsets.h"

//Put the addresses of game functions here!
//TODO: Find a good automatic way to find updated offsets when game updates. (Pattern scanning is a possibility)

// 1.2448 offsets

int haloreach::function_offsets::unit_start_running_blindly_offset = 0x48A8E8;

int haloreach::function_offsets::unit_update_offset = 0x478CF4;

int haloreach::function_offsets::game_update_offset = 0x5E95C;

int haloreach::function_offsets::actor_perception_set_target_offset = 0x5E7784;

int haloreach::function_offsets::weapon_has_infinite_ammo_offset = 0x4A455C;

int haloreach::function_offsets::weapon_barrel_fire_weapon_heat_offset = 0x49FA4C;

int haloreach::function_offsets::weapon_barrel_create_projectiles_offset = 0x4A0D68;

int haloreach::function_offsets::player_index_from_unit_index_offset = 0x4FC64;

int haloreach::function_offsets::game_time_set_rate_scale_direct_offset = 0x63ED0;

int haloreach::function_offsets::game_time_get_speed_offset = 0x6376C;

int haloreach::function_offsets::weapon_get_owner_unit_index_offset = 0x49C744;

int haloreach::function_offsets::player_control_machinima_available_offset = 0xC4EF0;

int haloreach::function_offsets::player_mapping_next_active_output_user_offset = 0x5DE64;

int haloreach::function_offsets::player_mapping_get_unit_by_output_user_offset = 0x5DD90;