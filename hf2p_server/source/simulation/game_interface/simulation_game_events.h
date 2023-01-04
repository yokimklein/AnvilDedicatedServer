#pragma once

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