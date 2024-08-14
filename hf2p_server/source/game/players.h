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

// there's a high likelihood of Halo Online using the Halo Reach player flags enum
enum e_player_flags
{
	// game_engine_player_is_playing
	// players_update_activation
	// c_game_engine::apply_player_update
	// halo reach x360: bit 0
	// halo 4 x360:     bit 0
	_player_active_in_game_bit = 0,

	// game_engine_player_is_playing
	// player_rejoined_game
	// players_joined_in_progress_allow_spawn
	// halo 4 x360:     bit 1
	// halo reach x360: bit 1
	_player_left_game_bit,

	// player_update_after_game
	// c_game_engine::apply_player_update
	// halo reach x360: bit 2
	// halo 4 x360:     bit 2
	_player_blocking_teleporter_bit,

	// game_engine_update_after_game_update_state
	// player_spawn
	// player_reset
	// players_detach_from_map
	// c_game_engine::apply_player_update
	// halo reach x360: bit 3
	// halo 4 x360:     bit 3
	_player_initial_spawn_bit,

	// halo 4 adds 2 new flags here
	// bit 4, game_engine_display_role_selection_ui

	// player_prepare_action
	// unit_action_vehicle_exit_finished
	// player_suppress_action
	// halo reach x360: bit 4
	// halo 4 x360:     bit 6
	_player_unknown_bit4,

	// player_prepare_action
	// player_suppress_action
	// halo reach x360: bit 5
	// halo 4 x360:     bit 7
	_player_unknown_bit5,

	// player_prepare_action
	// player_suppress_action
	// halo reach x360: bit 6
	// halo 4 x360:     bit 8
	_player_unknown_bit6,

	// player_suppress_action
	// halo reach x360: bit 7
	// halo 4 x360:     bit 9
	_player_unknown_bit7,

	// teleporter_flag_object_as_having_teleported_recursive
	//  should pass `_object_mask_unit` test
	// update_players_before_teleporters
	// update_players_after_teleporters
	// c_teleporter_area::update_players
	// halo reach x360: bit 8
	// halo 4 x360:     bit 10
	_player_unknown_bit8,

	// teleporter_flag_object_as_having_teleported_recursive
	//  should pass `_object_mask_unit` test
	// update_players_after_teleporters
	// c_teleporter_area::update_players
	// halo reach x360: bit 9
	// halo 4 x360:     bit 11
	_player_unknown_bit9,

	// player_update_after_game
	// player_notify_vehicle_ejection_finished
	// c_game_engine::apply_player_update
	// halo reach x360: bit 10
	// halo 4 x360:     bit 12
	_player_vehicle_entrance_ban_bit,

	// player_spawn
	// players_coop_update_respawn
	// halo reach x360: bit 11
	// halo 4 x360:     bit 13
	_player_unknown_bit11,

	// game_engine_player_is_playing
	// game_engine_update_player_sitting_out
	// c_game_engine::apply_player_update
	// halo reach x360: bit 12
	// halo 4 x360:     bit 14
	_player_sitting_out_bit,

	// player_reset
	// players_joined_in_progress_allow_spawn
	// halo reach x360: bit 13
	// halo 4 x360:     bit 15
	_player_unknown_bit13,

	// game_engine_update_coop_spawning
	// halo reach x360: bit 14
	// halo 4 x360:     bit 16
	// look_training_hack?
	_player_unknown_bit14,

	// players_coop_update_respawn
	// survival_mode_respawn_dead_players
	// halo reach x360: bit 15
	// halo 4 x360:     bit 17
	_player_unknown_bit15,

	// player_prepare_action
	// unit_action_melee_player_update
	// player_submit_actions
	// halo reach x360: bit 16
	// halo 4 x360:     bit 18
	_player_unknown_bit16,

	// unit_action_melee_player_update
	// halo reach x360: bit 19
	// halo 4 x360:     bit 23

	// halo 3:     count unknown
	// halo reach: count 20
	// halo 4:     count 31

	k_player_flags_count // unconfirmed size
};

struct s_player_shot_info
{
	word __unknown0;
	s_damage_reporting_info damage_reporting_info;
	char __data6[0x6];
};
static_assert(sizeof(s_player_shot_info) == 0xC);

struct player_datum : s_datum_header
{
	short unknown;
	c_flags<e_player_flags, long, k_player_flags_count> flags;
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
	long active_loadout_index;
	ulong consumable_index;
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
	short revenge_shield_boost_multiplier;
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
static_assert(sizeof(player_datum) == 0x19B0);
static_assert(0x04 == OFFSETOF(player_datum, flags));
static_assert(0x30 == OFFSETOF(player_datum, unit_index));
static_assert(0x49 == OFFSETOF(player_datum, next_spawn_control_context));
static_assert(0x4C == OFFSETOF(player_datum, active_loadout_index));
static_assert(0x70 == OFFSETOF(player_datum, configuration));
static_assert(0x1842 == OFFSETOF(player_datum, lives));
static_assert(0x1848 == OFFSETOF(player_datum, last_killed_game_time));
static_assert(0x1874 == OFFSETOF(player_datum, spectating_player_index));
static_assert(0x18B4 == OFFSETOF(player_datum, revenge_shield_boost_unknown80));
static_assert(0x18B6 == OFFSETOF(player_datum, revenge_shield_boost_multiplier));
static_assert(0x18EC == OFFSETOF(player_datum, assassination_victim_unit_index));
static_assert(0x18F0 == OFFSETOF(player_datum, is_assassination_victim));
static_assert(0x18F4 == OFFSETOF(player_datum, assasination_authorative_position));
static_assert(0x1900 == OFFSETOF(player_datum, assasination_authorative_forward));

struct c_player_in_game_iterator
{
public:
	void begin();
	bool next();
	player_datum* get_datum();
	long get_index() const;
	short get_absolute_index() const;

protected:
	c_data_iterator<player_datum> m_iterator;
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