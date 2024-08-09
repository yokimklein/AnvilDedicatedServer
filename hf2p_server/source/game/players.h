#pragma once
#include <cseries\cseries.h>
#include <game\aim_target.h>
#include <objects\damage.h>
#include <memory\data.h>
#include <game\player_configuration.h>
#include <game\game_engine_player_traits.h>

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