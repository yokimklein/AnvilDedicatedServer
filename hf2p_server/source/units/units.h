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
static_assert(sizeof(s_motor_data) == 0x184);

struct s_unit_data : s_motor_data
{
	long awake_tick_count;
	long actor_index;
	long simulation_actor_index;
	c_flags<e_unit_flags, ulong, k_unit_flags_count> unit_flags;
	long team;
	long player_index;
	long last_weak_player_index;
	long game_time_at_last_unit_effect;
	long unit_control_flags;
	long desired_animation_seat;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d unknown_vector;
	real_vector3d field_1D0;
	real_vector3d looking_vector;
	real_vector3d previous_looking_vector;
	real_vector3d field_1F4;
	real_point3d gaze_position;
	real_vector3d throttle;
	real_vector3d control_throttle;
	uchar control_context_identifier;
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
	s_unit_weapon_set current_weapon_set;
	s_unit_weapon_set desired_weapon_set;
	c_static_array<long, 4> weapon_object_indices;
	c_static_array<long, 4> weapon_last_used_at_game_time;
	c_static_array<long, 4> equipment_object_indices;
	long active_equipment_object_indices[4];
	long equipment_pickup_time;
	long consumable_energy_level;
	long consumable_energy_restored_game_time;
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
	real active_camouflage_maximum;
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
	long hologram_creator_weak_unit_index;
	long hologram_creation_time;
	long hologram_ticks_left;
	long hologram_definition_index;
	real hologram_shimmer_value;
	real_point3d hologram_destination;
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
static_assert(sizeof(s_unit_data) == 0x590);

FUNCTION_DEF(0x423010, void, __fastcall, unit_set_actively_controlled, datum_index unit_index, bool unknown);

void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index);