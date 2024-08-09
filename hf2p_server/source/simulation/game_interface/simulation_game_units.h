#pragma once
#include <simulation\game_interface\simulation_game_objects.h>
#include <units\units.h>

enum e_simulation_unit_update_flag
{
	_simulation_unit_update_control = k_simulation_object_update_flag_count,
	_simulation_unit_update_parent_vehicle,
	_simulation_unit_update_desired_aiming_vector,
	_simulation_unit_update_desired_weapon_set,
	_simulation_unit_update_weapon1_type,
	_simulation_unit_update_weapon2_type,
	_simulation_unit_update_weapon3_type,
	_simulation_unit_update_weapon4_type,
	_simulation_unit_update_weapon1_state,
	_simulation_unit_update_weapon2_state,
	_simulation_unit_update_weapon3_state,
	_simulation_unit_update_weapon4_state,
	_simulation_unit_update_grenade_counts,
	_simulation_unit_update_active_camo,
	_simulation_unit_update_equipment,
	_simulation_unit_update_equipment_charges,
	_simulation_unit_update_consumable_energy,
	_simulation_unit_update_control_context,
	_simulation_unit_update_assassination_data,
	// removed from HO
	//_simulation_unit_update_hud_info,
	//_simulation_unit_update_map_editor_flags,

	k_simulation_unit_update_flag_count
};

struct s_simulation_unit_state_data
{
	s_simulation_object_state_data object;

	short controlling_player_absolute_index;
	short controlling_simulation_actor_index;
	short controlling_actor_squad_index;
	short controlling_actor_fire_team_index;
	short controlling_actor_starting_location_index;
	long replication_entity_index;
	short parent_seat_index;
	real_vector3d desired_aiming_vector;
	s_unit_weapon_set desired_weapon_set;

	struct
	{
		long weapon_definition_index;
		short multiplayer_weapon_identifier;
		short simulation_weapon_identifier;
		short rounds_loaded;
		short rounds_inventory;
		real age;
	} weapon_types[4];
	byte grenade_counts[4];
	bool active_camo_active;
	real active_camo_maximum;
	real active_camo;
	real active_camo_regrowth;
	long parent_gamestate_index;
	long equipment_definition_indices[4]; // is this this?

	byte __unknown12C[4];
	long __unknown130;
	long __unknown134;
	byte __data138[0x1];
	byte __unknown139;
	byte __data13A[0x2];

	c_simulation_object_index_state_data assassination_data;
	string_id assassination_sync_action_name;
	real_point3d assassination_authoritative_pos;
	real_vector3d assassination_authoritative_fwd;
	bool assassination_is_victim;
};
static_assert(sizeof(s_simulation_unit_state_data) == sizeof(s_simulation_object_state_data) + 0xC4);