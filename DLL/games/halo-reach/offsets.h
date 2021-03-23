#pragma once
namespace haloreach
{
	class offsets
	{
	public:
		static int unit_start_running_blindly_offset;

		static int unit_update_offset;

		static int weapon_has_infinite_ammo_offset;
		static int weapon_barrel_fire_weapon_heat_offset;

		static int player_index_from_unit_index_offset;
		static int weapon_get_owner_unit_index_offset;
		static int weapon_barrel_create_projectiles_offset;
	};
}