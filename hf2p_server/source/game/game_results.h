#pragma once
#include <cseries\cseries.h>
#include <simulation\simulation.h>
#include <game\players.h>
#include <game\player_configuration.h>
#include <game\game_options.h>
#include <models\damage_info_definitions.h>
#include <text\unicode.h>

enum e_game_results_statistic
{
	_game_results_statistic_games_played = 0,
	_game_results_statistic_games_completed,
	_game_results_statistic_games_won,
	_game_results_statistic_games_tied,
	_game_results_statistic_rounds_completed,
	_game_results_statistic_rounds_won,
	_game_results_statistic_in_round_score,
	_game_results_statistic_in_game_total_score,
	_game_results_statistic_kills,
	_game_results_statistic_assists,
	_game_results_statistic_deaths,
	_game_results_statistic_betrayals,
	_game_results_statistic_suicides,
	_game_results_statistic_most_kills_in_a_row,
	_game_results_statistic_seconds_alive,
	_game_results_statistic_ctf_flag_scores,
	_game_results_statistic_ctf_flag_grabs,
	_game_results_statistic_ctf_flag_carrier_kills,
	_game_results_statistic_ctf_flag_returns,
	_game_results_statistic_assault_bomb_arms,
	_game_results_statistic_assault_bomb_grabs,
	_game_results_statistic_assault_bomb_disarms,
	_game_results_statistic_assault_bomb_detonations,
	_game_results_statistic_oddball_time_with_ball,
	_game_results_statistic_oddball_unused,
	_game_results_statistic_oddball_kills_as_carrier,
	_game_results_statistic_oddball_ball_carrier_kills,
	_game_results_statistic_king_time_on_hill,
	_game_results_statistic_king_total_control_time,
	_game_results_statistic_king_unused0,
	_game_results_statistic_king_unused1,
	_game_results_statistic_unused0,
	_game_results_statistic_unused1,
	_game_results_statistic_unused2,
	_game_results_statistic_vip_takedowns,
	_game_results_statistic_vip_kills_as_vip,
	_game_results_statistic_vip_guard_time,
	_game_results_statistic_vip_time_as_vip,
	_game_results_statistic_vip_lives_as_vip,
	_game_results_statistic_juggernaut_kills,
	_game_results_statistic_juggernaut_kills_as_juggernaut,
	_game_results_statistic_juggernaut_total_control_time,
	_game_results_statistic_total_wp,
	_game_results_statistic_juggernaut_unused,
	_game_results_statistic_territories_owned,
	_game_results_statistic_territories_captures,
	_game_results_statistic_territories_ousts,
	_game_results_statistic_territories_time_in_territory,
	_game_results_statistic_infection_zombie_kills,
	_game_results_statistic_infection_infections,
	_game_results_statistic_infection_time_as_human,

	k_game_results_statistic_count
};

enum e_game_results_medal
{
	_game_results_medal_extermination = 0,
	_game_results_medal_perfection,
	_game_results_medal_multiple_kill_2,
	_game_results_medal_multiple_kill_3,
	_game_results_medal_multiple_kill_4,
	_game_results_medal_multiple_kill_5,
	_game_results_medal_multiple_kill_6,
	_game_results_medal_multiple_kill_7,
	_game_results_medal_multiple_kill_8,
	_game_results_medal_multiple_kill_9,
	_game_results_medal_multiple_kill_10,
	_game_results_medal_5_kills_in_a_row,
	_game_results_medal_10_kills_in_a_row,
	_game_results_medal_15_kills_in_a_row,
	_game_results_medal_20_kills_in_a_row,
	_game_results_medal_25_kills_in_a_row,
	_game_results_medal_30_kills_in_a_row,
	_game_results_medal_sniper_kill_5,
	_game_results_medal_sniper_kill_10,
	_game_results_medal_shotgun_kill_5,
	_game_results_medal_shotgun_kill_10,
	_game_results_medal_collision_kill_5,
	_game_results_medal_collision_kill_10,
	_game_results_medal_sword_kill_5,
	_game_results_medal_sword_kill_10,
	_game_results_medal_juggernaut_kill_5,
	_game_results_medal_juggernaut_kill_10,
	_game_results_medal_zombie_kill_5,
	_game_results_medal_zombie_kill_10,
	_game_results_medal_human_kill_5,
	_game_results_medal_human_kill_10,
	_game_results_medal_human_kill_15,
	_game_results_medal_koth_kill_5,
	_game_results_medal_shotgun_kill_sword,
	_game_results_medal_vehicle_impact_kill,
	_game_results_medal_vehicle_hijack,
	_game_results_medal_aircraft_hijack,
	_game_results_medal_deadplayer_kill,
	_game_results_medal_player_kill_spreeplayer,
	_game_results_medal_spartanlaser_kill,
	_game_results_medal_stickygrenade_kill,
	_game_results_medal_sniper_kill,
	_game_results_medal_bashbehind_kill,
	_game_results_medal_bash_kill,
	_game_results_medal_flame_kill,
	_game_results_medal_driver_assist_gunner,
	_game_results_medal_assault_bomb_planted,
	_game_results_medal_assault_player_kill_carrier,
	_game_results_medal_vip_player_kill_vip,
	_game_results_medal_juggernaut_player_kill_juggernaut,
	_game_results_medal_oddball_carrier_kill_player,
	_game_results_medal_ctf_flag_captured,
	_game_results_medal_ctf_flag_player_kill_carrier,
	_game_results_medal_ctf_flag_carrier_kill_player,
	_game_results_medal_infection_survive,
	_game_results_medal_unused1,
	_game_results_medal_unused2,
	_game_results_medal_unused3,

	k_game_results_medal_count
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

enum e_game_results_damage_statistic
{
	_game_results_damage_statistic_damage_kills = 0,
	_game_results_damage_statistic_damage_deaths,
	_game_results_damage_statistic_damage_betrayals,
	_game_results_damage_statistic_damage_suicides,
	_game_results_damage_statistic_damage_headshots,

	k_game_results_damage_statistic_count
};

enum e_game_results_player_vs_player_statistic
{
	_game_results_player_vs_player_statistic_player_vs_player_kills = 0,
	_game_results_player_vs_player_statistic_player_vs_player_deaths,

	k_game_results_player_vs_player_statistic_count
};

struct s_integer_statistic_update
{
	word statistic;
};

struct s_game_results_player_data
{
	bool player_exists;
	byte machine_index;

	union
	{
#pragma pack(push, 1)
		struct
		{
			s_player_identifier player_identifier;
			byte __padA[0x6];
		} s;
#pragma pack(pop)

		byte data[sizeof(s)];
	};

	s_player_configuration configuration;

	byte player_standing;
	byte __pad1631[0xB];

	long player_score;
};
static_assert(sizeof(s_game_results_player_data) == 0xB90);

class s_game_results_player_data_update
{
public:
	bool valid;
	bool data_valid;
	byte __pad2[0x6];
	s_game_results_player_data update;
};
static_assert(sizeof(s_game_results_player_data_update) == 0xB98);

struct s_game_results_team_data
{
	bool team_exists;
	byte team_standing;
	word team_score;
};
static_assert(sizeof(s_game_results_team_data) == 0x4);

struct s_game_results_team_data_update
{
	bool valid;
	bool data_valid;

	s_game_results_team_data update;
};
static_assert(sizeof(s_game_results_team_data_update) == 0x6);

struct s_game_results_player_medal_statistics_update
{
	bool valid;
	byte __pad[1]; // statistics_valid?
	s_integer_statistic_update statistics[k_game_results_medal_count];
};
static_assert(sizeof(s_game_results_player_medal_statistics_update) == 0x76);

struct s_game_results_player_achievement_statistics_update
{
	bool valid;
	byte __pad[1]; // statistics_valid?
	s_integer_statistic_update statistics[k_achievement_count];
};
static_assert(sizeof(s_game_results_player_achievement_statistics_update) == 0x60);

struct s_game_results_player_damage_statistics_update
{
	bool valid;
	byte __pad1[1]; // statistics_valid?
	s_integer_statistic_update statistics[k_game_results_damage_statistic_count];
};
static_assert(sizeof(s_game_results_player_damage_statistics_update) == 0xC);

struct s_game_results_player_statistics
{
	s_integer_statistic_update statistics[k_game_results_statistic_count];

	s_integer_statistic_update medals[k_game_results_medal_count];
	s_integer_statistic_update achievements[k_achievement_count];

	s_game_results_player_damage_statistics_update damage[k_damage_reporting_type_count];
};
static_assert(sizeof(s_game_results_player_statistics) == 0x438);

struct s_game_results_player_statistics_update
{
	bool valid;
	bool statistics_valid;
	s_integer_statistic_update statistics[k_game_results_statistic_count];
	s_game_results_player_medal_statistics_update medals;
	s_game_results_player_achievement_statistics_update achievements;
	s_game_results_player_damage_statistics_update damage[k_damage_reporting_type_count];
};
static_assert(sizeof(s_game_results_player_statistics_update) == 0x43E);

struct s_game_results_player_vs_player_statistics
{
	s_integer_statistic_update statistics[k_game_results_player_vs_player_statistic_count];
};
static_assert(sizeof(s_game_results_player_vs_player_statistics) == 0x4);

struct s_game_results_player_vs_player_statistics_update
{
	bool valid;
	byte __pad1[1]; // statistics_valid?
	s_game_results_player_vs_player_statistics update;
};
static_assert(sizeof(s_game_results_player_vs_player_statistics_update) == 0x6);

struct s_game_results_team_statistics
{
	s_integer_statistic_update statistics[k_game_results_statistic_count];
};
static_assert(sizeof(s_game_results_team_statistics) == 0x66);

struct s_game_results_team_statistics_update
{
	bool team_valid;
	byte __pad1[1]; // statistics_valid?
	s_game_results_team_statistics update;
};
static_assert(sizeof(s_game_results_team_statistics_update) == 0x68);

struct s_game_results_statistics
{
	c_static_array<s_game_results_player_statistics, 16> player;

	// player_vs_player[subject_player_absolute_index][reference_player_absolute_index].statistics
	c_static_array<c_static_array<s_game_results_player_vs_player_statistics, 16>, 16> player_vs_player;

	c_static_array<s_game_results_team_statistics, 16> team;
};
static_assert(sizeof(s_game_results_statistics) == 0x4DE0);

struct s_game_results_statistics_update
{
	c_static_array<s_game_results_player_statistics_update, 16> player;

	// player_vs_player[subject_player_absolute_index][reference_player_absolute_index].statistics
	c_static_array<c_static_array<s_game_results_player_vs_player_statistics_update, 16>, 16> player_vs_player;

	c_static_array<s_game_results_team_statistics_update, 16> team;
};
static_assert(sizeof(s_game_results_statistics_update) == 0x5060);

#pragma pack(push, 1)
struct s_game_results_machine_data
{
	s_machine_identifier machine_identifier;

	bool machine_exists;
	bool machine_connected;
	bool machine_host;
	bool machine_initial_host;
	bool machine_voluntary_quit;

	byte __pad17[1];

	struct
	{
		// TODO
		byte __data[0xA];
	} machine_bandwidth_estimate;
};
static_assert(sizeof(s_game_results_machine_data) == 0x20);
#pragma pack(pop)

struct s_game_results_machine_data_update
{
	bool machine_valid;
	byte __pad1[1];

	s_game_results_machine_data update;
};
static_assert(sizeof(s_game_results_machine_data_update) == 0x22);

struct s_game_results_incremental_update
{
	bool started;
	dword start_time;
	bool finished;
	dword finish_time;
	bool finalized;
	dword finish_reason;
	c_static_array<s_game_results_player_data_update, 16> player_updates; // 0x1648 - 0xB98
	c_static_array<s_game_results_team_data_update, 16> team_updates;
	s_game_results_statistics_update statistics;
	c_static_array<s_game_results_machine_data_update, 17> machines;
	byte __pad1[6];
};
static_assert(sizeof(s_game_results_incremental_update) == 0x10CA0);

struct s_game_results_incremental
{
	bool finalized;
	bool started;
	dword start_time;
	bool finished;
	dword finish_time;
	c_static_array<s_game_results_player_data, 16> players;
	c_static_array<s_game_results_team_data, 16> teams;
	s_game_results_statistics statistics;
	c_static_array<s_game_results_machine_data, 17> machines;
};
static_assert(sizeof(s_game_results_incremental) == 0x10950);

struct s_game_results_event
{
	byte type;
	byte player_index;
	byte union_storage[0x1E];
	dword time;
};
static_assert(sizeof(s_game_results_event) == 0x24);

struct c_game_results
{
	byte finish_reason;
	bool initialized;
	bool finalized;
	// is this game result specific version of `s_game_matchmaking_options`?
	s_game_matchmaking_options matchmaking_options;

	bool team_game;
	byte __data65[7]; // pad?
	qword game_instance;
	c_game_variant game_variant;
	c_static_wchar_string<32> map_variant_name;
	long map_id;
	c_static_string<260> scenario_path;

	bool started;
	dword start_time;
	bool finished;
	dword finish_time;

	byte __data24C[2];
	bool simulation_aborted;
	byte __data24F[1]; // pad?

	c_static_array<s_game_results_player_data, 16> players;
	c_static_array<s_game_results_team_data, 16> teams;
	s_game_results_statistics statistics;
	c_static_array<s_game_results_event, 1000> events;
	c_static_array<s_game_results_machine_data, 17> machines;
};
static_assert(sizeof(c_game_results) == 0x19A10);

struct s_game_results_globals
{
	bool recording;
	bool recording_paused;
	bool updating;
	long event_index;
	dword __time8;
	c_game_results* results;
};
static_assert(sizeof(s_game_results_globals) == 0x10);

class c_simulation_view;
struct c_game_results_replicator
{
	c_simulation_view* m_view;
	bool m_has_fatal_error;
	bool m_sending_updates;
	bool m_receiving_updates;
	long m_incremental_update_number;
	dword __unknownC;
	s_game_results_incremental m_incremental;
	dword m_last_update_time; // updates every 10000ms
	byte __data1B464[4];
};
static_assert(sizeof(c_game_results_replicator) == 0x10968);

struct s_integer_statistic_definition
{
	const char* name;

	// type?
	dword __unknown4;

	word minimum_value;
	word maximum_value;
	dword encoding_bits;
};
static_assert(sizeof(s_integer_statistic_definition) == 0x10);

void game_results_notify_player_indices_changed();
void game_results_statistic_set(datum_index absolute_player_index, e_game_team team_index, long statistic, long value);