#pragma once
#include <cseries\cseries.h>
#include <game\aim_target.h>
#include <objects\damage.h>
#include <memory\data.h>
#include <game\player_configuration.h>
#include <game\game_engine_player_traits.h>
#include <game\game_engine_simulation.h>

enum e_player_respawn_failure
{
	_player_respawn_failure_none = 0,
	_player_respawn_failure_in_editor,
	_player_respawn_failure_cinematic_playing,
	_player_respawn_failure_scripting_says_it_isnt_safe,
	_player_respawn_failure_waiting_on_respawn_timer,
	_player_respawn_failure_friend_in_combat,
	_player_respawn_failure_nearby_enemies_pursuing_friend,
	_player_respawn_failure_friend_airborne_or_moving,
	_player_respawn_failure_friend_in_vehicle,
	_player_respawn_failure_everybody_is_dead,
	_player_respawn_failure_waiting_for_checkpoint,
	_player_respawn_failure_can_not_find_valid_location,

	k_player_respawn_failure_count
};

struct s_player_shot_info
{
	word __unknown0;
	s_damage_reporting_info damage_reporting_info;
	char __data6[0x6];
};
static_assert(sizeof(s_player_shot_info) == 0xC);

struct s_player_datum : s_datum_header
{
	short unknown;
	dword_flags player_flags;
	s_player_identifier player_identifier;
	ulong left_game_time;
	s_machine_identifier machine_identifier; // secure_address / xnaddr
	short machine_index;
	short machine_input_user_index;
	long machine_controller_index;
	short location; // s_scenario_location
	datum_index unit_index;
	datum_index dead_unit_index;
	datum_index previous_unit_index;
	ulong unknown_flags2;
	ushort unknown_simulation_ticks2; // hf2p cooldown?
	ushort unknown_simulation_ticks3; // hf2p cooldown?
	ushort unknown_simulation_ticks4; // hf2p cooldown?
	ushort unknownflags3;
	uchar unknown_simulation_ticks;
	uchar next_spawn_control_context;
	uchar field_4C;
	uchar field_4B;
	long character_type_index;
	ulong loadout_index; // consumable_index;
	uchar melee_recovery_timer;
	uchar melee_soft_recovery_timer;
	uchar grenade_supression_timer;
	bool is_sprinting;
	bool is_crouching;
	bool is_firing_left;
	bool is_firing_right;
	uchar field_5B;
	ushort magnification_level;
	bool sprint_longerupted_firing;
	bool sprint_longerupted_zoom;
	short sprint_cooldown;
	short sprint_cooldown2;
	real_vector3d position;
	s_player_configuration configuration;
	s_player_configuration desired_configuration;
	long single_player_respawn_timer;
	bool early_respawn_requested;
	bool respawn_in_progress;
	bool respawn_forced;
	long respawn_timer;
	long respawn_timer_countdown_seconds;
	ulong respawn_time_penalty;
	char blocking_teleporter;
	long dead_timer;
	long in_game_timer;
	long alive_timer;
	long unknown_input_global_ticks;
	long grenade_regeneration_timer;
	long unknown_object_index;
	long last_aim_target_time;
	ushort ticks_sprinting;
	ushort field_2CE6;
	ushort sprint_restoration_timer;
	ushort sprint_depleted_timer;
	bool sprint_disabled;
	ushort unknown_ticks_4;
	c_aim_target_object aim_target_object;
	long field_2CFC;
	struct {
		char mask;
		long object_index;
		long field_8;
	}
	tracking_objects[8];
	ushort respawn_duration_seconds;
	bool first_spawn;
	uchar respawn_failed_reason;
	struct // tank_mode
	{
		long tank_mode_last_used_tick;
		real tank_mode_duration;
		real tank_mode_damage_acceleration_scale;
		real tank_mode_damage_absorption_scale;
		real tank_mode_field_10;
		real tank_mode_field_14;
	};
	struct // reactive armor
	{
		long reactive_armor_last_used_tick;
		real reactive_armor_duration;
		real reactive_armor_damage_reflection_scale;
		real reactive_armor_field_0C;
	};
	long stamina_restore_near_death_timer;
	bool grenade_scavenger_modifier_used;
	byte : 3;
	long last_aim_assist_target_player_index;
	short unused;
	char game_engine_vehicle_use_timer;
	char game_engine_simulation_update_timer;
	char game_engine_simulation_update_timer2;
	bool was_sneaking;
	bool is_sneaking;
	byte : 8;
	ushort aim_assist_update_timer;
	ushort lives;
	long griefer_player_index;
	ulong last_killed_game_time;
	c_player_traits traits;
	c_static_array<dword, 3> powerup_pickup_time;
	long spectating_player_index;
	bool map_editor_rotating;
	real_point2d map_editor_throttle;
	real_euler_angles2d map_editor_rotation;
	ushort time_in_hill;
	ushort time_outside_Hill;
	ushort time_hill_Left;
	short : 16;
	union // nemesis mechanics
	{
		c_static_array<short, 16> nemesis_mechanics_nemesis_counts; // killing_player, nemesis medal
		c_static_array<short, 16> nemesis_mechanics_avenger_counts; // dead_player, avenger medal
	};
	char revenge_shield_boost_unknown80;
	char field_18B5;
	char revenge_shield_boost_multiplier;
	char field_18B7;
	ulong last_hit_marker_game_time;
	long revenge_shield_boost_player_index;
	s_damage_reporting_info revenge_shield_boost_damage;
	bool revenge_taken;
	char : 8;
	char : 8;
	char : 8;
	byte __unknown_data[0x24];
	struct // assassination info
	{
		long assassination_victim_unit_index;
		bool is_assassination_victim;
		real_point3d assasination_authorative_position;
		real_vector3d assasination_authorative_forward;
		uchar assassination_state[60];
	};
	c_static_array<s_player_shot_info, 8> shot_info;
	short spawn_count;
	short : 16;
};
static_assert(sizeof(s_player_datum) == 0x19B0);
static_assert(0x04 == OFFSETOF(s_player_datum, player_flags));
static_assert(0x30 == OFFSETOF(s_player_datum, unit_index));
static_assert(0x49 == OFFSETOF(s_player_datum, next_spawn_control_context));
static_assert(0x4C == OFFSETOF(s_player_datum, character_type_index));
static_assert(0x70 == OFFSETOF(s_player_datum, configuration));
static_assert(0x1842 == OFFSETOF(s_player_datum, lives));
static_assert(0x1848 == OFFSETOF(s_player_datum, last_killed_game_time));
static_assert(0x1874 == OFFSETOF(s_player_datum, spectating_player_index));
static_assert(0x18B4 == OFFSETOF(s_player_datum, revenge_shield_boost_unknown80));
static_assert(0x18B6 == OFFSETOF(s_player_datum, revenge_shield_boost_multiplier));
static_assert(0x18EC == OFFSETOF(s_player_datum, assassination_victim_unit_index));
static_assert(0x18F0 == OFFSETOF(s_player_datum, is_assassination_victim));
static_assert(0x18F4 == OFFSETOF(s_player_datum, assasination_authorative_position));
static_assert(0x1900 == OFFSETOF(s_player_datum, assasination_authorative_forward));

class c_player_in_game_iterator : public c_data_iterator<s_player_datum>
{
public:
	using c_data_iterator<s_player_datum>::c_data_iterator;
	bool next()
	{
		for (m_datum = (s_player_datum*)data_iterator_next(&m_iterator);
			m_datum && TEST_BIT(m_datum->player_flags, 1);
			m_datum = (s_player_datum*)data_iterator_next(&m_iterator))
		{
		}

		return m_datum != nullptr;
	}
};

#pragma pack(push, 1)
struct s_players_global_data
{
	long players_in_game_count;

	bool input_disabled;
	bool mostly_inhibited;

	bool weapon_pickup_disabled;
	bool __unknown7;
	bool equipment_use_disabled;

	byte __data9[3];

	long unknown_todo; // TODO: additional 4 byte field added somewhere above here since ms23

	dword machine_valid_mask;
	c_static_array<s_machine_identifier, k_maximum_machines> machine_identifiers;

	bool local_machine_exists;
	s_machine_identifier local_machine_identifier;
	byte __pad131[0x3];

	long local_machine_index;
	bool scripted_dont_allow_respawning;

	byte __data139;

	c_enum<e_player_respawn_failure, short, _player_respawn_failure_none, k_player_respawn_failure_count> respawn_failure;

	// player_positions_initialize_for_new_structure_bsp
	// players_update_after_game
	bool __unknown13C;

	byte __data13D[0x3];

	real_point3d switching_player_position;
	real_vector3d switching_player_forward;

	// player_positions_initialize_for_new_structure_bsp
	// players_update_after_game
	long begin_zone_set_switch_trigger_volume_index;
	long commit_zone_set_switch_trigger_volume_index;

	// players_update_after_game
	// if (player_index != -1 && ++__unknown160 > 12)
	//    __unknown160 = 0
	short __unknown160;

	short __unknown162;

	// players_update_after_game
	long player_index;

	// players_update_after_game
	long zoneset_index;

	// memset in `players_initialize_for_new_map`
	// zone_set_trigger_volume_index
	c_static_flags<1024> zone_set_switch_flags;

	// `terminal_was_completed`
	// - returns whether or not the given terminal was read to completion
	word terminal_completed_flags;

	// `terminal_was_accessed`
	// - returns whether or not the given terminal was accessed
	word terminal_accessed_flags;

	// `terminal_is_being_read`
	// - returns whether or not a terminal is currently being read
	bool terminal_being_read;

	byte __data1F1[0x3];

	byte __data1F4[0x40];
};
static_assert(sizeof(s_players_global_data) == 0x238);
static_assert(0x14 == OFFSETOF(s_players_global_data, machine_identifiers));
#pragma pack(pop)

bool player_identifier_is_valid(s_player_identifier const* identifier);
const char* player_identifier_get_string(s_player_identifier const* identifier);
long player_mapping_get_input_user(datum_index player_index);
void __fastcall player_set_facing(datum_index player_index, real_vector3d* forward);
void __fastcall player_control_set_facing(long input_user_index, real_vector3d* forward);
long player_index_from_absolute_player_index(short absolute_player_index);
void __fastcall player_increment_control_context(datum_index player_index);
bool __fastcall player_is_local(datum_index player_index);
void __fastcall player_clear_assassination_state(datum_index player_index);
long get_player_action_control_context_identifier_bits();
s_machine_identifier* players_get_machine_identifier(long machine_index);