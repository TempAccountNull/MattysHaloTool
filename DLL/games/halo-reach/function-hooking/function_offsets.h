#pragma once
namespace haloreach
{
	class function_offsets
	{
	public:
		static int unit_start_running_blindly_offset;
		static int unit_update_offset;
		static int game_update_offset;
		static int actor_perception_set_target_offset;
		static int weapon_has_infinite_ammo_offset;
		static int weapon_barrel_fire_weapon_heat_offset;
		static int player_index_from_unit_index_offset;
		static int weapon_get_owner_unit_index_offset;
		static int player_control_machinima_available_offset;
		static int player_mapping_next_active_output_user_offset;
		static int weapon_barrel_create_projectiles_offset;
		static int game_time_set_rate_scale_direct_offset;
		static int game_time_get_speed_offset;
		static int player_mapping_get_unit_by_output_user_offset;
	};
}