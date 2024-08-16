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
	_simulation_player_update_waypoint_action,
	_simulation_player_update_blocking_teleporter,
	_simulation_player_update_netdebug,
	_simulation_player_update_lives_remaining,
	_simulation_player_update_grief_player_index,
	_simulation_player_update_vehicle_entrance_ban,
	_simulation_player_update_active_in_game, // used in player quit? syncs _player_active_in_game_bit
	_simulation_player_update_sitting_out,
	_simulation_player_update_spectating_player,
	_simulation_player_update_control_aiming,
	_simulation_player_update_loadout_index,
	_simulation_player_update_consumable_supression,
	_simulation_player_update_last_killer,

	k_simulation_player_update_flag_count
};

struct s_player_respawn_target_request_event_data
{
	short player_index;
	short absolute_player_index;
};
static_assert(sizeof(s_player_respawn_target_request_event_data) == 0x4);

struct s_game_engine_request_boot_player_event_data
{
	datum_index booted_player_index;
	datum_index booting_player_index;
};
static_assert(sizeof(s_game_engine_request_boot_player_event_data) == 0x8);

void simulation_action_game_engine_player_create(short player_absolute_index);
void simulation_action_game_engine_player_delete(short player_absolute_index);
void simulation_action_game_engine_player_update(short player_index, c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_engine_player_update(datum_index player_index, long update_flag);
void simulation_action_player_taunt_request(short player_index);
bool __stdcall c_simulation_player_respawn_request_event_definition__apply_game_event(long reference_gamestate_count, const datum_index* gamestate_indicies, long payload_size, const s_player_respawn_target_request_event_data* payload);