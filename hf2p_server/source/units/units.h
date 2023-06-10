#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/units/units.hpp

#include "..\cseries\cseries.h"
#include "..\game\aim_assist.h"
#include "..\objects\objects.h"
#include "..\objects\target_tracking.h"
#include "..\dllmain.h"

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
};

enum e_unit_flags
{
	k_unit_flags_count
};

enum e_unit_control_flags
{
	k_unit_control_flags_count
};

enum e_unit_drop_type : long
{
	_unit_drop_type_none = -1,
	_unit_drop_type_0,
	_unit_drop_type_delete,
	_unit_drop_type_2,
	_unit_drop_type_3,
	_unit_drop_type_response_to_deletion,

	k_unit_drop_type_count
};

struct s_unit_weapon_set
{
	word set_identifier;
	c_static_array<byte, k_weapon_set_count> weapon_indices;
};

struct unit_control_data
{
	long animation_seat;
	short aiming_speed;
	s_unit_weapon_set weapon_set;
	word grenade_index;
	word zoom_level;
	dword_flags control_flags;
	real_vector3d throttle;
	real primary_trigger;
	real secondary_trigger;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d looking_vector;
	real_point3d gaze_position;
	s_aim_assist_targeting_result aim_assist_data;
};
static_assert(sizeof(unit_control_data) == 0x80);

struct s_unit_predicted_weapon_state
{
	short rounds_loaded;
	short rounds_left;
	long : 32;
};
static_assert(sizeof(s_unit_predicted_weapon_state) == 0x8);

struct s_unit_attacker
{
	long game_time;
	float score;
	long team_index;
	long player_index;
};
static_assert(sizeof(s_unit_attacker) == 0x10);

struct s_motor_data : s_object_data
{
	s_object_header_block_reference motor_tasks;
	s_object_header_block_reference motor_state;
	s_object_header_block_reference action_state_storage;
};
static_assert(sizeof(s_motor_data) == 0x188);

struct s_unit_data : s_motor_data
{
	//long awake_tick_count; // seems to have been removed since ms23
	long actor_index; // 0x188
	long simulation_actor_index; // 0x18C
	c_flags<e_unit_flags, ulong, k_unit_flags_count> unit_flags;
	long team;
	long player_index; // 0x198
	long last_weak_player_index;
	long game_time_at_last_unit_effect;
	long unit_control_flags;
	long desired_animation_seat;
	real_vector3d facing_vector;
	real_vector3d aiming_vector; // 0x1B8
	real_vector3d melee_aiming_vector;
	real_vector3d field_1D0;
	real_vector3d looking_vector; // 0x1DC
	real_vector3d previous_looking_vector;
	real_vector3d field_1F4;
	real_point3d gaze_position;
	real_vector3d throttle;
	real_vector3d control_throttle;
	uchar control_context_identifier; // 0x224
	char aiming_speed;
	s_damage_reporting_info special_death_damage_reporting_info;
	char special_death_type;
	short special_death_node_index;
	real_vector3d special_death_node_acceleration;
	real primary_trigger;
	real secondary_trigger;
	s_aim_assist_targeting_result aim_assist_data;
	c_target_tracking_system target_tracking;
	c_flags<e_unit_control_flags, ulong, k_unit_control_flags_count> persistent_control_flags;
	char weapon_drop_delay_ticks;
	char left_eye_node;
	char right_eye_node;
	uchar horizontal_aiming_change;
	uchar primary_fire_timer;
	uchar secondary_fire_timer;
	short seat_index;
	real ambient_illumination;
	real self_illumination;
	real mouth_aperture;
	real mouth_time;
	s_unit_weapon_set current_weapon_set; // 0x2C8
	s_unit_weapon_set desired_weapon_set; // 0x2CC
	c_static_array<long, 4> weapon_object_indices; // offset 0x2D0
	c_static_array<long, 4> weapon_last_used_at_game_time;
	c_static_array<long, 4> equipment_object_indices; // offset 0x2F0
	c_static_array<long, 4> active_equipment_object_indices;
	long equipment_pickup_time;

	// TODO: an extra 4 byte field was added BEFORE consumable_energy_level and AFTER equipment_object_indices
	long unknown_todo;

	long consumable_energy_level; // offset 0x318
	long consumable_energy_restored_game_time; // offset 0x31C
	short weapon_firing_time;
	char current_grenade_index;
	char desired_grenade_index;
	c_static_array<uchar, 4> grenade_counts;
	char current_zoom_level;
	char desired_zoom_level;
	char : 8;
	char aimning_change;
	long weapon_control_last_active_time;
	long motion_control_unit_index;
	long weapon_control_unit_index;
	long last_vehicle_index;
	long last_vehicle_exit_game_time;
	short fake_squad_index;
	char impulse_vehicle_from_seat_ticks;
	char seat_power_valid_flags;
	real seat_power;
	real integrated_light_power;
	real integrated_light_battery;
	real integrated_night_vision_power;
	real open_state;
	byte seat_acceleration_state[0x6C];
	long predicted_player_index;
	long predicted_simulation_actor_index;
	long predicted_simulation_actor_squad_index;
	long predicted_simulation_actor_cell_index;
	long predicted_simulation_actor_spawn_point_index;
	c_static_array<s_unit_predicted_weapon_state, 4> predicted_weapon_state;
	real active_camouflage;
	real active_camouflage_maximum; // 0x400 - TODO, this is -4 bytes offset
	real active_camouflage_regrowth;
	long active_camouflage_end_time;
	real last_used_healthpack_game_time;
	real healthpack_vitality;
	real healthpack_time_scale;
	uchar map_editor_helper_flags;
	short emp_timer;
	short emp_campaign_metagame_timer;
	real crouch_fraction;
	short delayed_damage_category;
	short delayed_damage_timer;
	real delayed_damage_peak;
	long delayed_damage_owner_weak_object_index;
	long flaming_death_attacker_object_index;
	real run_blindly_angle;
	real run_blindly_angular_velocity;

	long unknown_todo2; // TODO: a new field was added somewhere before this point - find out what this is

	long hologram_creator_weak_unit_index; // 0x43C
	long hologram_creation_time; // 0x440
	long hologram_ticks_left; // 0x444
	long hologram_definition_index; // 0x448
	real hologram_shimmer_value; // 0x44C
	real_point3d hologram_destination; // 0x450
	long sync_action_type;
	real_point3d sync_action_origin;
	real_vector3d sync_action_forward;
	real_vector3d sync_action_up;
	bool sync_action_critical_participant;
	byte field_480[0x28];
	long time_of_death;
	real movement_stun;
	short movement_stun_ticks;
	short : 16;
	long : 32;
	short : 16;
	short : 16;
	c_static_array<s_unit_attacker, 4> attackers;
	c_static_array<long, 4> attacker_weapon_unit_indices;
	real boost_power_meter;
	real boost_recharge_delay;
	long last_enemies_moving_nearby_tick;
	long last_damaged_tick;
	long last_in_soft_kill_volume_time;
	long last_in_phantom_volume_time;
	long unknown_tick;
	long unknown_team_index;
	long unknown_object_index;
	s_object_header_block_reference animation_storage;
	s_object_header_block_reference speech_storage;
	ulong unit_ai_flags;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
	long : 32;
};
static_assert(sizeof(s_unit_data) == 0x598);
static_assert(0x188 == OFFSETOF(s_unit_data, actor_index));
static_assert(0x18C == OFFSETOF(s_unit_data, simulation_actor_index));
static_assert(0x198 == OFFSETOF(s_unit_data, player_index));
static_assert(0x1B8 == OFFSETOF(s_unit_data, aiming_vector));
static_assert(0x1DC == OFFSETOF(s_unit_data, looking_vector));
static_assert(0x224 == OFFSETOF(s_unit_data, control_context_identifier));
static_assert(0x2C8 == OFFSETOF(s_unit_data, current_weapon_set));
static_assert(0x2CC == OFFSETOF(s_unit_data, desired_weapon_set));
static_assert(0x2D0 == OFFSETOF(s_unit_data, weapon_object_indices));
static_assert(0x2F0 == OFFSETOF(s_unit_data, equipment_object_indices));
static_assert(0x318 == OFFSETOF(s_unit_data, consumable_energy_level));
static_assert(0x31C == OFFSETOF(s_unit_data, consumable_energy_restored_game_time));
//static_assert(0x400 == OFFSETOF(s_unit_data, active_camouflage_maximum));
static_assert(0x43C == OFFSETOF(s_unit_data, hologram_creator_weak_unit_index));
static_assert(0x440 == OFFSETOF(s_unit_data, hologram_creation_time));
static_assert(0x444 == OFFSETOF(s_unit_data, hologram_ticks_left));
static_assert(0x448 == OFFSETOF(s_unit_data, hologram_definition_index));
static_assert(0x44C == OFFSETOF(s_unit_data, hologram_shimmer_value));
static_assert(0x450 == OFFSETOF(s_unit_data, hologram_destination));

FUNCTION_DEF(0x423010, void, __fastcall, unit_set_actively_controlled, datum_index unit_index, bool unknown);
void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index);
void __fastcall unit_delete_all_weapons_internal(datum_index unit_index);
void __fastcall unit_inventory_set_weapon_index(datum_index unit_index, datum_index inventory_index, datum_index item_index, e_unit_drop_type drop_type);
void __fastcall unit_control(datum_index unit_index, void* unit_control_data);
void __fastcall unit_set_aiming_vectors(datum_index unit_index, real_vector3d* aiming_vector, real_vector3d* looking_vector);