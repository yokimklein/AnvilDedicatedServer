#pragma once
#include <cseries\cseries.h>
#include <simulation\game_interface\simulation_game_entities.h>

enum e_simulation_game_engine_globals_update_flag : long
{
	_simulation_game_engine_globals_update_initial_teams, // initial teams & team designators
	_simulation_game_engine_globals_update_team_lives_per_round,
	_simulation_game_engine_globals_update_state, // game_engine_globals->current_state
	_simulation_game_engine_globals_update_game_finished,
	_simulation_game_engine_globals_update_round_index,
	_simulation_game_engine_globals_update_round_timer,
	_simulation_game_engine_globals_update_round_condition,

	k_simulation_game_engine_globals_flag_count
};

enum e_simulation_ctf_engine_globals_update_flag : long
{
	_simulation_ctf_engine_globals_update_side_switch = k_simulation_game_engine_globals_flag_count,
	_simulation_ctf_engine_globals_update_unknown8,
	_simulation_ctf_engine_globals_update_unknown9,
	_simulation_ctf_engine_globals_update_unknown10,
	_simulation_ctf_engine_globals_update_unknown11,
	_simulation_ctf_engine_globals_update_unknown12,

	k_simulation_ctf_engine_globals_flag_count
};

enum e_simulation_king_engine_globals_update_flag : long
{
	_simulation_king_engine_globals_update_unknown7 = k_simulation_game_engine_globals_flag_count,
	_simulation_king_engine_globals_update_unknown8,

	k_simulation_king_engine_globals_flag_count
};

enum e_simulation_sandbox_engine_globals_update_flag : long
{
	_simulation_sandbox_engine_globals_update_object_team = k_simulation_game_engine_globals_flag_count,
	_simulation_sandbox_engine_globals_update_object_grab, 
	_simulation_sandbox_engine_globals_update_budget, 
	_simulation_sandbox_engine_globals_update_object_count,
	_simulation_sandbox_engine_globals_update_content_type,

	k_simulation_sandbox_engine_globals_flag_count
};

enum e_simulation_vip_engine_globals_update_flag : long
{
	_simulation_vip_engine_globals_update_player1 = k_simulation_game_engine_globals_flag_count,
	_simulation_vip_engine_globals_update_player2,
	_simulation_vip_engine_globals_update_player3,
	_simulation_vip_engine_globals_update_player4,
	_simulation_vip_engine_globals_update_player5,
	_simulation_vip_engine_globals_update_player6,
	_simulation_vip_engine_globals_update_player7,
	_simulation_vip_engine_globals_update_player8,
	_simulation_vip_engine_globals_update_unused1,
	_simulation_vip_engine_globals_update_destination_zone1,
	_simulation_vip_engine_globals_update_destination_zone2,
	_simulation_vip_engine_globals_update_destination_zone3,
	_simulation_vip_engine_globals_update_destination_zone4,
	_simulation_vip_engine_globals_update_destination_zone5,
	_simulation_vip_engine_globals_update_destination_zone6,
	_simulation_vip_engine_globals_update_destination_zone7,
	_simulation_vip_engine_globals_update_destination_zone8,
	_simulation_vip_engine_globals_update_destination_zone9,
	_simulation_vip_engine_globals_update_destination_zone10,
	_simulation_vip_engine_globals_update_destination_zone11,
	_simulation_vip_engine_globals_update_destination_zone12,
	_simulation_vip_engine_globals_update_unused2,
	_simulation_vip_engine_globals_update_unknown29, // updates __unknown20 in s_vip_globals

	k_simulation_vip_engine_globals_flag_count
};

enum e_simulation_juggernaut_engine_globals_update_flag : long
{
	_simulation_juggernaut_engine_globals_update_juggernaut_player = k_simulation_game_engine_globals_flag_count,
	_simulation_juggernaut_engine_globals_update_unknown8, // updates unknown_seconds
	_simulation_juggernaut_engine_globals_update_unknown9, // updates __unknown6

	k_simulation_juggernaut_engine_globals_flag_count
};

enum e_simulation_territories_engine_globals_update_flag : long
{
	_simulation_territories_engine_globals_update_territory1 = k_simulation_game_engine_globals_flag_count,
	_simulation_territories_engine_globals_update_territory2,
	_simulation_territories_engine_globals_update_territory3,
	_simulation_territories_engine_globals_update_territory4,
	_simulation_territories_engine_globals_update_territory5,
	_simulation_territories_engine_globals_update_territory6,
	_simulation_territories_engine_globals_update_territory7,
	_simulation_territories_engine_globals_update_territory8,
	_simulation_territories_engine_globals_update_unknown15, // updates __unknown430
	_simulation_territories_engine_globals_update_unknown16, // updates __unknown434
	_simulation_territories_engine_globals_update_unknown17, // updates __unknown438

	k_simulation_territories_engine_globals_flag_count
};

enum e_simulation_assault_engine_globals_update_flag : long
{
	_simulation_assault_engine_globals_update_unknown13 = k_simulation_ctf_engine_globals_flag_count, // updates __unknown164C
	_simulation_assault_engine_globals_update_unknown14, // updates 0x1670
	_simulation_assault_engine_globals_update_unknown15, // updates 0x1682
	_simulation_assault_engine_globals_update_unknown16, // something player index related

	k_simulation_assault_engine_globals_flag_count
};

enum e_simulation_infection_engine_globals_update_flag : long
{
	_simulation_infection_engine_globals_update_unknown8 = k_simulation_game_engine_globals_flag_count, // updates __unknown0
	_simulation_infection_engine_globals_update_unknown9, // updates __unknown2
	_simulation_infection_engine_globals_update_unknown10, // updates __unknown10
	_simulation_infection_engine_globals_update_unknown11, // updates __unknown4
	_simulation_infection_engine_globals_update_unknown12, // updates __unknown8

	k_simulation_infection_engine_globals_flag_count
};

enum e_simulation_statborg_update_flag : long
{
	_simulation_statborg_update_player0,
	_simulation_statborg_update_player1,
	_simulation_statborg_update_player2,
	_simulation_statborg_update_player3,
	_simulation_statborg_update_player4,
	_simulation_statborg_update_player5,
	_simulation_statborg_update_player6,
	_simulation_statborg_update_player7,
	_simulation_statborg_update_player8,
	_simulation_statborg_update_player9,
	_simulation_statborg_update_player10,
	_simulation_statborg_update_player11,
	_simulation_statborg_update_player12,
	_simulation_statborg_update_player13,
	_simulation_statborg_update_player14,
	_simulation_statborg_update_player15,
	_simulation_statborg_update_team0,
	_simulation_statborg_update_team1,
	_simulation_statborg_update_team2,
	_simulation_statborg_update_team3,
	_simulation_statborg_update_team4,
	_simulation_statborg_update_team5,
	_simulation_statborg_update_team6,
	_simulation_statborg_update_team7,
	_simulation_statborg_update_finalize_for_game_end,

	k_simulation_statborg_update_flag_count
};

enum e_simulation_player_update_flag : long
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

void simulation_action_game_engine_globals_create();
void simulation_action_game_engine_globals_delete();
void simulation_action_game_statborg_create();
void simulation_action_game_statborg_delete();
void simulation_action_game_ai_create();
void simulation_action_game_ai_delete();
void simulation_action_game_map_variant_create_all();
void simulation_action_game_map_variant_create_for_chunk(long chunk_index);
void simulation_action_game_map_variant_delete();
void simulation_action_game_engine_player_create(short player_absolute_index);
void simulation_action_game_engine_player_delete(short player_absolute_index);
void simulation_action_breakable_surfaces_create();
void simulation_action_game_statborg_update(c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_statborg_update(long update_flag);
void simulation_action_game_engine_player_update(short player_index, c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_engine_player_update(datum_index player_index, long update_flag);
void simulation_action_game_engine_globals_update(c_flags<long, ulong64, 64>* update_flags);
void simulation_action_game_engine_globals_update(long update_flag);
void __cdecl simulation_action_weapon_state_update(datum_index weapon_index);