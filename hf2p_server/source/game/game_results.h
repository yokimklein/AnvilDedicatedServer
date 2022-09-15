#pragma once
#include "..\cseries\cseries.h"
#include "..\simulation\simulation.h"
#include "players.h"
#include "player_configuration.h"

enum e_player_statistics
{
	_games_played,
	_games_completed,
	_games_won,
	_games_tied,
	_rounds_completed,
	_rounds_won,
	_in_round_score,
	_in_game_total_score,
	_kills,
	_assists,
	_deaths,
	_betrayals,
	_suicides,
	_most_kills_in_a_row,
	_seconds_alive,
	_ctf_flag_scores,
	_ctf_flag_grabs,
	_ctf_flag_carrier_kills,
	_ctf_flag_returns,
	_assault_bomb_arms,
	_assault_bomb_grabs,
	_assault_bomb_disarms,
	_assault_bomb_detonations,
	_oddball_time_with_ball,
	_oddball_unused,
	_oddball_kills_as_carrier,
	_oddball_ball_carrier_kills,
	_king_time_on_hill,
	_king_total_control_time,
	_king_unused1, // both are just 'king-unused' in the original array
	_king_unused2, //
	_unused1, // all 3 are just 'unused' in original array
	_unused2, // 
	_unused3, // 
	_vip_takedowns,
	_vip_kills_as_vip,
	_vip_guard_time,
	_vip_time_as_vip,
	_vip_lives_as_vip,
	_juggernaut_kills,
	_juggernaut_kills_as_juggernaut,
	_juggernaut_total_control_time,
	_total_wp,
	_juggernaut_unused,
	_territories_owned,
	_territories_captures,
	_territories_ousts,
	_territories_time_in_territory,
	_infection_zombie_kills,
	_infection_infections,
	_infection_time_as_human,

	k_player_statistics_count
};

enum e_medals
{
	_medal_extermination,
	_medal_perfection,
	_medal_multiple_kill_2,
	_medal_multiple_kill_3,
	_medal_multiple_kill_4,
	_medal_multiple_kill_5,
	_medal_multiple_kill_6,
	_medal_multiple_kill_7,
	_medal_multiple_kill_8,
	_medal_multiple_kill_9,
	_medal_multiple_kill_10,
	_medal_5_kills_in_a_row,
	_medal_10_kills_in_a_row,
	_medal_15_kills_in_a_row,
	_medal_20_kills_in_a_row,
	_medal_25_kills_in_a_row,
	_medal_30_kills_in_a_row,
	_medal_sniper_kill_5,
	_medal_sniper_kill_10,
	_medal_shotgun_kill_5,
	_medal_shotgun_kill_10,
	_medal_collision_kill_5,
	_medal_collision_kill_10,
	_medal_sword_kill_5,
	_medal_sword_kill_10,
	_medal_juggernaut_kill_5,
	_medal_juggernaut_kill_10,
	_medal_zombie_kill_5,
	_medal_zombie_kill_10,
	_medal_human_kill_5,
	_medal_human_kill_10,
	_medal_human_kill_15,
	_medal_koth_kill_5,
	_medal_shotgun_kill_sword,
	_medal_vehicle_impact_kill,
	_medal_vehicle_hijack,
	_medal_aircraft_hijack,
	_medal_deadplayer_kill,
	_medal_player_kill_spreeplayer,
	_medal_spartanlaser_kill,
	_medal_stickygrenade_kill,
	_medal_sniper_kill,
	_medal_bashbehind_kill,
	_medal_bash_kill,
	_medal_flame_kill,
	_medal_driver_assist_gunner,
	_medal_assault_bomb_planted,
	_medal_assault_player_kill_carrier,
	_medal_vip_player_kill_vip,
	_medal_juggernaut_player_kill_juggernaut,
	_medal_oddball_carrier_kill_player,
	_medal_ctf_flag_captured,
	_medal_ctf_flag_player_kill_carrier,
	_medal_ctf_flag_carrier_kill_player,
	_medal_infection_survive,
	_medal_unused1, // all 3 are just 'unused1/2/3' in the original array
	_medal_unused2, //
	_medal_unused3, //

	k_medals_count
};

enum e_achievement
{
	_achievement_beat_sc100,
	_achievement_beat_sc110,
	_achievement_beat_sc120,
	_achievement_beat_sc130,
	_achievement_beat_sc140,
	_achievement_beat_sc150,
	_achievement_beat_l200,
	_achievement_beat_l300,
	_achievement_beat_campaign_normal,
	_achievement_beat_campaign_heroic,
	_achievement_beat_campaign_legendary,
	_achievement_wraith_killer,
	_achievement_naughty_naughty,
	_achievement_good_samaritan,
	_achievement_dome_inspector,
	_achievement_laser_blaster,
	_achievement_both_tubes,
	_achievement_i_like_fire,
	_achievement_my_clothes,
	_achievement_pink_and_deadly,
	_achievement_dark_times,
	_achievement_trading_down,
	_achievement_headcase,
	_achievement_boom_headshot,
	_achievement_ewww_sticky,
	_achievement_junior_detective,
	_achievement_gumshoe,
	_achievement_super_sleuth,
	_achievement_metagame_points_in_sc100,
	_achievement_metagame_points_in_sc110,
	_achievement_metagame_points_in_sc120,
	_achievement_metagame_points_in_sc130a,
	_achievement_metagame_points_in_sc130b,
	_achievement_metagame_points_in_sc140,
	_achievement_metagame_points_in_l200,
	_achievement_metagame_points_in_l300,
	_achievement_be_like_marty,
	_achievement_find_all_audio_logs,
	_achievement_find_01_audio_logs,
	_achievement_find_03_audio_logs,
	_achievement_find_15_audio_logs,
	_achievement_vidmaster_challenge_deja_vu,
	_achievement_vidmaster_challenge_endure,
	_achievement_vidmaster_challenge_classic,
	_achievement_heal_up,
	_achievement_stunning,
	_achievement_tourist,

	k_achievement_count
};

enum e_damage
{
	_damage_kills,
	_damage_deaths,
	_damage_betrayals,
	_damage_suicides,
	_damage_headshots,

	k_damage_count
};

enum e_player_vs_player
{
	_player_vs_player_kills,
	_player_vs_player_deaths,

	k_player_vs_player_count
};

#pragma pack(push, 1)
struct s_game_results_player_data
{
	bool player_exists;
	byte machine_index;
	s_player_identifier player_identifier;
	byte pad2[6];
	s_player_configuration player_data;
	byte player_standing;
	byte pad3[11];
	short player_score;
	byte pad4[2];
};
static_assert(sizeof(s_game_results_player_data) == 0xB90);

struct s_game_results_player
{
	bool player_valid;
	bool player_data_valid;
	byte pad1[6];
	s_game_results_player_data player_data;
};
static_assert(sizeof(s_game_results_player) == 0xB98);

struct s_game_results_team_data
{
	bool team_exists;
	byte team_standing;
	short team_score;
};
static_assert(sizeof(s_game_results_team_data) == 0x4);

struct s_game_results_team
{
	bool team_valid;
	bool team_data_valid;
	s_game_results_team_data team_data;
};
static_assert(sizeof(s_game_results_team) == 0x6);

struct s_game_results_player_statistics_update
{
	bool player_statistics_valid;
	ushort player_statistics_update[k_player_statistics_count];
};
static_assert(sizeof(s_game_results_player_statistics_update) == 0x67);

struct s_game_results_player_medals_statistics_update
{
	bool player_medals_valid;
	byte pad[1];
	ushort medals_statistics_update[k_medals_count];
};
static_assert(sizeof(s_game_results_player_medals_statistics_update) == 0x76);

struct s_game_results_player_achievements_statistics_update
{
	bool player_achievements_valid;
	byte pad[1];
	ushort achievements_statistics_update[k_achievement_count];
};
static_assert(sizeof(s_game_results_player_achievements_statistics_update) == 0x60);

struct s_game_results_player_damage_statistics_update
{
	bool player_damage;
	byte pad[1];
	ushort damage_statistics_update[k_damage_count];
};
static_assert(sizeof(s_game_results_player_damage_statistics_update) == 0xC);

struct s_game_results_player_statistics
{
	bool player_valid;
	s_game_results_player_statistics_update player_statistics;
	s_game_results_player_medals_statistics_update medals_statistics;
	s_game_results_player_achievements_statistics_update achievements_statistics;
	s_game_results_player_damage_statistics_update damage_statistics[64]; // why 64? for each simulation event?
};
static_assert(sizeof(s_game_results_player_statistics) == 0x43E);

struct s_game_results_player_vs_player
{
	bool player_vs_player_valid;
	byte pad[1];
	ushort player_vs_player_statistics[k_player_vs_player_count];
};
static_assert(sizeof(s_game_results_player_vs_player) == 0x6);

struct s_game_results_player_team_valid
{
	bool team_valid;
	byte pad[1];
	ushort player_team_valid_statistics[k_player_statistics_count];
};
static_assert(sizeof(s_game_results_player_team_valid) == 0x68);

struct s_game_results_machine
{
	bool machine_valid;
	byte pad1[1];
	s_machine_identifier machine_identifier;
	bool machine_exists;
	bool machine_connected;
	bool machine_host;
	bool machine_initial_host;
	bool machine_voluntary_quit;
	byte pad2[1];
	byte machine_bandwidth_estimate[10]; // could be just a int64 with 2 bytes of padding at the end
};
static_assert(sizeof(s_game_results_machine) == 0x22);

struct s_game_results_incremental_update
{
	bool started;
	byte pad1[3];
	ulong start_time;
	bool finished;
	byte pad2[3];
	ulong finish_time;
	bool finalized;
	byte pad3[3];
	ulong finish_reason; // unused
	s_game_results_player players[k_network_maximum_players_per_session];
	s_game_results_team teams[k_network_maximum_players_per_session];
	s_game_results_player_statistics player_statistics[k_network_maximum_players_per_session];
	s_game_results_player_vs_player player_vs_player_statistics[k_network_maximum_players_per_session][k_network_maximum_players_per_session];
	s_game_results_player_team_valid player_valid_teams[k_network_maximum_players_per_session];
	s_game_results_machine machines[k_network_maximum_machines_per_session];
	byte pad4[6];
};
static_assert(sizeof(s_game_results_incremental_update) == 0x10CA0);
#pragma pack(pop)