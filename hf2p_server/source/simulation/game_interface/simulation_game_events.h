#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

enum e_simulation_event_type
{
	_simulation_event_type_damage_aftermath,
	_simulation_event_type_damage_section_response,
	_simulation_event_type_breakable_surface_damage,
	_simulation_event_type_projectile_attached,
	_simulation_event_type_projectile_detonate,
	_simulation_event_type_projectile_impact_effect,
	_simulation_event_type_projectile_object_impact_effect,
	_simulation_event_type_effect_on_pos,
	_simulation_event_type_game_engine,
	_simulation_event_type_unit_board_vehicle,
	_simulation_event_type_unit_pickup,
	_simulation_event_type_weapon_effect,
	_simulation_event_type_weapon_empty_click,
	_simulation_event_type_hs_script_wake, // new - distributed script waking
	_simulation_event_type_hit_marker,
	_simulation_event_type_authority_ignored_predicted_position, // new - async ragdolls?
	_simulation_event_type_unit_exit_vehicle,
	_simulation_event_type_unit_assassinate,
	_simulation_event_type_player_taunt_request, // new - end game podium stances
	_simulation_event_type_weapon_fire,
	_simulation_event_type_unit_equipment_use,
	_simulation_event_type_weapon_reload,
	_simulation_event_type_unit_throw_initiate,
	_simulation_event_type_unit_melee_initiate,
	_simulation_event_type_weapon_pickup,
	_simulation_event_type_weapon_put_away,
	_simulation_event_type_weapon_drop,
	_simulation_event_type_weapon_tether_request, // new - used for the latch-tether trigger behaviour (grenade launcher)
	_simulation_event_type_vehicle_flip,
	_simulation_event_type_vehicle_trick,
	_simulation_event_type_device_touch,
	_simulation_event_type_unit_throw_release,
	_simulation_event_type_unit_melee_damage,
	_simulation_event_type_unit_melee_clang,
	_simulation_event_type_unit_enter_vehicle,
	_simulation_event_type_game_engine_request_boot_player,
	_simulation_event_type_player_respawn_request,
	_simulation_event_type_player_force_base_respawn,
	_simulation_event_type_unit_equipment_pickup,
	_simulation_event_type_projectile_supercombine_request,
	_simulation_event_type_object_refresh,
	_simulation_event_type_player_editor_request,

	k_simulation_event_type_maximum_count = 64
};

enum e_damage_section_response_type
{
	_damage_section_receives_all_damage,
	_damage_section_receives_area_effect_damage,
	_damage_section_receives_local_damage,

	k_damage_section_response_type_count = 3
};

struct s_simulation_player_taunt_request_data
{
	long player_index;
};
static_assert(sizeof(s_simulation_player_taunt_request_data) == 0x4);

struct s_simulation_damage_section_response_data
{
	long damage_section_index;
	long response_index; // max of 16
	c_enum<e_damage_section_response_type, long, k_damage_section_response_type_count> damage_section_response_type;
};
static_assert(sizeof(s_simulation_damage_section_response_data) == 0xC);

bool __stdcall c_simulation_player_respawn_request_event_definition__apply_game_event(long reference_gamestate_count, const long* gamestate_indicies, long payload_size, const long* payload);
void simulation_event_generate_for_remote_peers(e_simulation_event_type event_type, long object_references_count, long* object_references, long ignore_player_index, long payload_size, void const* payload_data);
void simulation_event_generate_for_clients(e_simulation_event_type event_type, long object_references_count, long* object_references, long ignore_player_index, long payload_size, void const* payload_data);
void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type);