#include "function_offsets.h"

//Put the addresses of game functions here!
//TODO: Find a good automatic way to find updated offsets when game updates. (Pattern scanning is a possibility)

// 1.2241 offsets

int haloreach::function_offsets::unit_start_running_blindly_offset = 0x488294;

int haloreach::function_offsets::unit_update_offset = 0x4766A0;

int haloreach::function_offsets::game_update_offset = 0x4AAA4;

int haloreach::function_offsets::actor_perception_set_target_offset = 0x5E5134;

int haloreach::function_offsets::weapon_has_infinite_ammo_offset = 0x4A1F08;

int haloreach::function_offsets::weapon_barrel_fire_weapon_heat_offset = 0x49D3F8;

int haloreach::function_offsets::weapon_barrel_create_projectiles_offset = 0x49E714;

int haloreach::function_offsets::player_index_from_unit_index_offset = 0x53078;

int haloreach::function_offsets::game_time_set_rate_scale_direct_offset = 0x63D48;

int haloreach::function_offsets::game_time_get_speed_offset = 0x635E4;

int haloreach::function_offsets::weapon_get_owner_unit_index_offset = 0x49A0F0;

int haloreach::function_offsets::player_control_machinima_available_offset = 0xC4D10;

int haloreach::function_offsets::player_mapping_next_active_output_user_offset = 0x61278;

int haloreach::function_offsets::player_mapping_get_unit_by_output_user_offset = 0x611A4;