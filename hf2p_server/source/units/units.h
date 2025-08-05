#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/units/units.hpp

#include <cseries\cseries.h>
#include <ai\sector.h>
#include <game\aim_assist.h>
#include <game\player_configuration.h>
#include <objects\objects.h>
#include <objects\target_tracking.h>
#include <motor\mover.h>

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
};

enum e_unit_flags
{
	_unit_flags_bit0,
	_unit_flags_bit1,
	_unit_flags_bit2,
	_unit_flags_camo,

	k_unit_flags_count
};

enum e_unit_control_flags
{
	k_unit_control_flags_count
};

enum e_unit_drop_type
{
	_unit_drop_type_none = -1,
	_unit_drop_type_drop,
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
	real score;
	long team_index;
	long player_index;
};
static_assert(sizeof(s_unit_attacker) == 0x10);

struct _unit_datum
{
	//long awake_tick_count; // seems to have been removed since ms23
	long actor_index;
	long simulation_actor_index;
	c_flags<e_unit_flags, ulong, k_unit_flags_count> flags;
	c_enum<e_game_team, long, _game_team_none, k_multiplayer_team_count> team;
	long player_index;
	long last_weak_player_index;
	long game_time_at_last_unit_effect;
	long unit_control_flags;
	long desired_animation_seat;
	real_vector3d facing_vector;
	real_vector3d aiming_vector;
	real_vector3d melee_aiming_vector;
	real_vector3d field_1D0;
	real_vector3d looking_vector;
	real_vector3d previous_looking_vector;
	real_vector3d field_1F4;
	real_point3d gaze_position;
	real_vector3d throttle;
	real_vector3d control_throttle;
	uchar next_spawn_control_context;
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
	c_static_array<long, 4> active_equipment_object_indices;
	dword_flags active_equipment_slots; // new since ms23, bits correspond to which equipment slots are currently active
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
	byte seat_acceleration_state[0x6C + 4]; // 4 additional bytes since ms23
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
	short melee_inhibit_time;
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
	object_header_block_reference animation_storage;
	object_header_block_reference speech_storage; // seat storage?
	ulong unit_ai_flags;
	c_sector_ref pathfinding_sector;
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
	long emblem_unknown_index1; // used in unit_delete & unit_disconnect_from_structure_bsp - accesses global array with size 0xCD
	long emblem_unknown_index2; // used in unit_delete & unit_disconnect_from_structure_bsp - accesses global array with size 0xCD
	long : 32;
};
static_assert(sizeof(_unit_datum) == 0x410);
static_assert(0x00 == OFFSETOF(_unit_datum, actor_index));
static_assert(0x04 == OFFSETOF(_unit_datum, simulation_actor_index));
static_assert(0x10 == OFFSETOF(_unit_datum, player_index));
static_assert(0x30 == OFFSETOF(_unit_datum, aiming_vector));
static_assert(0x54 == OFFSETOF(_unit_datum, looking_vector));
static_assert(0x9C == OFFSETOF(_unit_datum, next_spawn_control_context));
static_assert(0x140 == OFFSETOF(_unit_datum, current_weapon_set));
static_assert(0x144 == OFFSETOF(_unit_datum, desired_weapon_set));
static_assert(0x148 == OFFSETOF(_unit_datum, weapon_object_indices));
static_assert(0x168 == OFFSETOF(_unit_datum, equipment_object_indices));
static_assert(0x178 == OFFSETOF(_unit_datum, active_equipment_object_indices));
static_assert(0x188 == OFFSETOF(_unit_datum, active_equipment_slots));
static_assert(0x18C == OFFSETOF(_unit_datum, equipment_pickup_time));
static_assert(0x190 == OFFSETOF(_unit_datum, consumable_energy_level));
static_assert(0x194 == OFFSETOF(_unit_datum, consumable_energy_restored_game_time));
static_assert(0x1BC == OFFSETOF(_unit_datum, seat_power));
static_assert(0x240 == OFFSETOF(_unit_datum, predicted_player_index));
static_assert(0x244 == OFFSETOF(_unit_datum, predicted_simulation_actor_index));
static_assert(0x274 == OFFSETOF(_unit_datum, active_camouflage));
static_assert(0x278 == OFFSETOF(_unit_datum, active_camouflage_maximum));
static_assert(0x27C == OFFSETOF(_unit_datum, active_camouflage_regrowth));
static_assert(0x280 == OFFSETOF(_unit_datum, active_camouflage_end_time));
static_assert(0x292 == OFFSETOF(_unit_datum, emp_timer));
static_assert(0x294 == OFFSETOF(_unit_datum, emp_campaign_metagame_timer));
static_assert(0x29C == OFFSETOF(_unit_datum, delayed_damage_category));
static_assert(0x2A0 == OFFSETOF(_unit_datum, delayed_damage_peak));
static_assert(0x2A4 == OFFSETOF(_unit_datum, delayed_damage_owner_weak_object_index));
static_assert(0x2B4 == OFFSETOF(_unit_datum, hologram_creator_weak_unit_index));
static_assert(0x2B8 == OFFSETOF(_unit_datum, hologram_creation_time));
static_assert(0x2BC == OFFSETOF(_unit_datum, hologram_ticks_left));
static_assert(0x2C0 == OFFSETOF(_unit_datum, hologram_definition_index));
static_assert(0x2C4 == OFFSETOF(_unit_datum, hologram_shimmer_value));
static_assert(0x2C8 == OFFSETOF(_unit_datum, hologram_destination));
static_assert(0x338 == OFFSETOF(_unit_datum, melee_inhibit_time));

struct unit_datum
{
	long definition_index;
	_object_datum object;
	_motor_datum motor;
	_unit_datum unit;
};
static_assert(sizeof(unit_datum) == 0x598);

struct s_new_unit_action_grenade
{
	long unk1;
	byte unk2_1;
	byte unk2_2;
	byte unk_type;
	bool unk2_4; // from client?
	byte unk3_1;
	char unk3_2;
	byte unk3_3;
	byte unk3_4;
	long throw_type_index;
	long projectile_index;
};

void __fastcall unit_set_actively_controlled(datum_index unit_index, bool actively_controlled);
void __fastcall unit_inventory_cycle_weapon_set_identifier(datum_index unit_index);
void __fastcall unit_delete_all_weapons_internal(datum_index unit_index);
void __fastcall unit_inventory_set_weapon_index(datum_index unit_index, datum_index inventory_index, datum_index item_index, e_unit_drop_type drop_type);
void __fastcall unit_drop_item(datum_index unit_index, datum_index equipment_index, e_unit_drop_type drop_type);
void __fastcall unit_control(datum_index unit_index, void* unit_control_data);
void __fastcall unit_set_aiming_vectors(datum_index unit_index, real_vector3d* aiming_vector, real_vector3d* looking_vector);
void __fastcall unit_add_initial_loadout(datum_index unit_index); // saber/hf2p function
void __fastcall unit_delete_equipment(datum_index unit_index, long slot_index);
void __fastcall unit_active_camouflage_ding(datum_index unit_index, real camouflage_decay, real regrowth_seconds);
void __fastcall unit_active_camouflage_disable(datum_index unit_index, real regrowth_seconds);
void __fastcall unit_active_camouflage_set_level(datum_index unit_index, real regrowth_seconds, long camouflage_end_time);
void __fastcall unit_active_camouflage_set_maximum(datum_index unit_index, real camouflage_maximum);
long unit_get_current_or_last_weak_player_index(datum_index unit_index);