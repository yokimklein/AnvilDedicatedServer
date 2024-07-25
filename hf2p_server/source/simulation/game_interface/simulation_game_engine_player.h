#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

enum e_simulation_player_update_flag
{
	_simulation_player_update_spawn_timer,
	_simulation_player_update_early_respawn,
	_simulation_player_update_health_traits,
	_simulation_player_update_weapon_traits,
	_simulation_player_update_movement_traits,
	_simulation_player_update_appearance_traits,
	_simulation_player_update_sensor_traits,
	_simulation_player_update_player_waypoint_unknown, // TODO: likely a gametype option, research this further
	_simulation_player_update_unknown_player_flags1, // blocking teleporter?
	_simulation_player_update_unknown_player_data,
	_simulation_player_update_lives,
	_simulation_player_update_grief, // is this to do with player booting?
	_simulation_player_update_unknown_player_flags2,
	_simulation_player_update_unknown_player_flags3, // player quit?
	_simulation_player_update_unknown_player_flags4, // update called in game_engine_update_player_sitting_out
	_simulation_player_update_spectating_player,
	_simulation_player_update_control,
	_simulation_player_update_character_type,
	_simulation_player_update_equipment_charges, // called at the same time as _simulation_unit_update_equipment_charges, sets some tick fields in the player datum
	_simulation_player_update_revenge,

	k_simulation_player_update_flag_count
};

void simulation_action_game_engine_player_create(short player_absolute_index);
void simulation_action_game_engine_player_delete(short player_absolute_index);
void simulation_action_game_engine_player_update(short player_index, c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_engine_player_update(datum_index player_index, long update_flag);