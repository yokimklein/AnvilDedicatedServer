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
	_player_active_in_game_bit = 0,
	_player_left_game_bit,
	_player_blocking_teleporter_bit,
	_player_initial_spawn_bit,
	_player_action_suppress_primary_bit,
	_player_action_suppress_secondary_bit,
	_player_action_suppress_tertiary_bit,
	_player_action_dual_wield_grenade_latch_bit,
	_player_standing_in_teleporter_bit,
	_player_standing_in_teleporter_teleported_bit,
	_player_vehicle_entrance_ban_bit,
	_player_play_coop_spawn_effect_bit,
	_player_sitting_out_bit,
	_player_joined_in_progress_bit,

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

	k_player_flag_count
};

enum e_multiplayer_powerup_flavor
{
	_powerup_flavor_red = 0,
	_powerup_flavor_blue,
	_powerup_flavor_yellow,

	k_multiplayer_powerup_flavor_count,
	_powerup_flavor_none = -1
};

struct s_simulation_unit_melee_damage_event_data;
struct _simulation_unit_melee_damage_event_data
{
	real_point3d impact_point;
	real_vector3d impact_normal;
	long damage_definition_index;
	long clang_damage_definition_index;
	short breakable_surface_set_index;
	short breakable_surface_index;
	long instanced_geometry_instance_index;
	long surface_index;
	long global_material_index;
	long object_material_index;
	real scale;
	s_damage_reporting_info damage_reporting_info;
};

struct s_player_shot_info
{
	short shot_id;
	s_damage_reporting_info damage_reporting_info;
	word pad;
	long game_time;
};
static_assert(sizeof(s_player_shot_info) == 0xC);

struct s_tracking_object
{
	byte __data[0x4];
	long object_index;
	long __time8;
};
static_assert(sizeof(s_tracking_object) == 0xC);

struct s_king_player_info
{
	short ticks_on_hill;
	short ticks_outside_hill;
	short ticks_on_hill_contested;
	short pad;
};
static_assert(sizeof(s_king_player_info) == 0x8);

struct s_juggernaut_player_info
{
	long juggernaut_index;
};
static_assert(sizeof(s_juggernaut_player_info) == 0x4);

struct multiplayer_player_info
{
	long player_display_index;
	short teleporter_index;
	char in_vehicle_timer;
	char without_unit_timer;
	char with_unit_timer;
	bool was_crouched;
	bool is_crouched;
	byte pad;
	short player_display_count;
	short remaining_lives;
	long last_betrayer_player_index;
	long time_of_death;
	c_player_traits player_traits;
	long powerup_pickup_time[k_multiplayer_powerup_flavor_count];
	long dead_camera_target_player_index;

	struct
	{
		bool player_locked_for_manipulation;
		bool pad[3];
		real_vector2d roll_distance_amount;
		real_vector2d yaw_pitch_angles;
	} map_editor_data;

	union
	{
		s_king_player_info king_player_info;
		s_juggernaut_player_info juggernaut_player_info;
	};
};
static_assert(sizeof(multiplayer_player_info) == 0x60);

struct player_datum : s_datum_header
{
	short unknown;
	c_flags<e_player_flags, long, k_player_flag_count> flags;
	s_player_identifier player_identifier;
	ulong left_game_time;
	s_machine_identifier machine_identifier; // secure_address / xnaddr
	short machine_index;
	short machine_input_user_index;
	long machine_controller_index;
	s_cluster_reference cluster_reference;
	datum_index unit_index;
	datum_index dead_unit_index;
	datum_index failed_teleport_unit_index;
	ulong latched_control_flags;

	struct // ---------- cooldown_reset ----------
	{
		// these are used in `players_update_after_game`, `player_spawn`, `sub_53C860`, struct?
		// if `player->equipment_cooldown_ticks == 0` do something with equipment
		// gameplay_modifier: `cooldown_reset` related
		word equipment_cooldown_ticks;
		word cooldown_reset_unknown42;
		word cooldown_reset_unknown44;
	};

	word_flags latched_action_flags;
	byte outside_of_world_timer;
	byte next_spawn_control_context;
	uchar field_4C;
	uchar field_4B;

	long active_loadout_index;
	ulong consumable_index;

	byte melee_recovery_ticks;
	byte melee_soft_recovery_ticks;
	byte grenade_suppression_timer;

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
	long respawn_timer_countdown_ticks;
	long respawn_timer_countdown_seconds;
	ulong respawn_time_penalty;
	char telefrag_timer;
	long dead_timer;
	long in_game_timer;
	long alive_timer;
	long unknown_input_global_ticks;
	long grenade_regeneration_timer;

	long aim_assist_object_index;
	long aim_assist_timestamp;

	ushort ticks_sprinting;
	ushort field_2CE6;
	ushort sprint_restoration_timer;
	ushort sprint_depleted_timer;
	bool sprint_disabled;

	word vehicle_entrance_ban_ticks;

	c_aim_target_object cached_target;
	long cached_target_untargeted_ticks;
	c_static_array<s_tracking_object, 8> tracking_objects;

	short recently_spawned_timer;
	bool recently_spawned_timer_is_initial_spawn;
	byte respawn_failure_reason;

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

	multiplayer_player_info multiplayer;

	union // nemesis mechanics
	{
		c_static_array<short, 16> nemesis_mechanics_nemesis_counts; // killing_player, nemesis medal
		c_static_array<short, 16> nemesis_mechanics_avenger_counts; // dead_player, avenger medal
	};

	char revenge_shield_boost_unknown80;
	char field_18B5; // padding?
	short revenge_shield_boost_multiplier;
	long revenge_unknown1; // last_hit_marker_game_time?
	long revenge_unknown2; // last_hit_marker_game_time?
	long revenge_shield_boost_player_index;
	s_damage_reporting_info revenge_shield_boost_damage;
	bool revenge_taken;
	char : 8;
	char : 8;
	char : 8;
	byte __unknown_data[0x20];

	struct // assassination info
	{
		long assassination_victim_unit_index;
		bool is_assassination_victim;
		char : 8;
		char : 8;
		char : 8;
		real_point3d assasination_authorative_position;
		real_vector3d assasination_authorative_forward;
		c_typed_opaque_data<struct s_simulation_unit_melee_damage_event_data, sizeof(_simulation_unit_melee_damage_event_data), __alignof(_simulation_unit_melee_damage_event_data) - 1> melee_damage_event_data;
	};
	c_static_array<s_player_shot_info, 8> shot_info;
	short spawn_count;
	short : 16;
};
static_assert(sizeof(player_datum) == 0x19B0);
static_assert(0x04 == OFFSETOF(player_datum, flags));
static_assert(0x30 == OFFSETOF(player_datum, unit_index));
static_assert(0x40 == OFFSETOF(player_datum, equipment_cooldown_ticks));
static_assert(0x49 == OFFSETOF(player_datum, next_spawn_control_context));
static_assert(0x4C == OFFSETOF(player_datum, active_loadout_index));
static_assert(0x70 == OFFSETOF(player_datum, configuration));
static_assert(0x1758 == OFFSETOF(player_datum, respawn_timer_countdown_ticks));
static_assert(0x175C == OFFSETOF(player_datum, respawn_timer_countdown_seconds));
static_assert(0x178E == OFFSETOF(player_datum, vehicle_entrance_ban_ticks));
static_assert(0x1842 == OFFSETOF(player_datum, multiplayer.remaining_lives));
static_assert(0x1848 == OFFSETOF(player_datum, multiplayer.time_of_death));
static_assert(0x1874 == OFFSETOF(player_datum, multiplayer.dead_camera_target_player_index));
static_assert(0x18B4 == OFFSETOF(player_datum, revenge_shield_boost_unknown80));
static_assert(0x18B6 == OFFSETOF(player_datum, revenge_shield_boost_multiplier));
static_assert(0x18C0 == OFFSETOF(player_datum, revenge_shield_boost_player_index));
static_assert(0x18C4 == OFFSETOF(player_datum, revenge_shield_boost_damage));
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
	datum_index get_index() const;
	short get_absolute_index() const;

protected:
	c_data_iterator<player_datum> m_iterator;
};

#pragma pack(push, 1)
struct players_global_data
{
	long players_in_game_count;
	bool input_disabled;
	bool mostly_inhibited;
	bool weapon_pickup_disabled;
	bool sprint_inhibited;
	bool equipment_use_disabled;
	byte __data9[3];
	long unknown_todo; // TODO: additional 4 byte field added somewhere above here since ms23

	dword machine_valid_mask;
	c_static_array<s_machine_identifier, k_maximum_machines> machine_identifiers;

	bool local_machine_exists;
	__declspec(align(1)) s_machine_identifier local_machine_identifier;
	byte __pad131[0x3];

	long local_machine_index;
	bool scripted_dont_allow_respawning;
	byte __data139;
	c_enum<e_player_respawn_failure, short, _player_respawn_failure_none, k_player_respawn_failure_count> respawn_failure;

	// player_positions_initialize_for_new_structure_bsp
	// players_update_after_game
	bool force_player_positions_initialize;

	byte __data13D[0x3];

	real_point3d switching_player_position;
	real_vector3d switching_player_forward;

	long begin_zone_set_switch_trigger_volume_index;
	long commit_zone_set_switch_trigger_volume_index;
	short zone_set_switch_check_recursive_ticks;
	short __unknown162;
	long zone_set_switch_player_index;
	long zone_set_switch_previous_zone_set_index;
	dword disabled_zone_set_switch_trigger_volume_flags[32];
	word terminal_completed_flags;
	word terminal_accessed_flags;
	bool terminal_being_read;
	byte __data1F1[0x3];

	dword combined_pvs[8];
	dword combined_pvs_local[8];
};
static_assert(sizeof(players_global_data) == 0x238);
static_assert(0x14 == OFFSETOF(players_global_data, machine_identifiers));
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
void __fastcall player_notify_vehicle_ejection_finished(datum_index player_index);
struct s_player_navpoint_data;
enum e_navpoint_action;
void player_navpoint_data_set_action(s_player_navpoint_data* waypoint, e_navpoint_action action);
void __fastcall player_swap(long player_absolute_index, long swap_player_absolute_index);
void __fastcall player_delete(datum_index player_index);

// consumable costs & cooldowns
s_s3d_player_loadout* player_get_loadout(player_datum* player);
const s_s3d_player_loadout* player_get_loadout(const player_datum* player);
bool player_has_consumable(const player_datum* player, long consumable_slot);
long __fastcall player_get_consumable_cost(const player_datum* player, long consumable_slot);
bool _fastcall player_can_use_consumable(const player_datum* player, long consumable_slot);
long player_get_energy_regeneration_duration(const player_datum* player, long energy_level);
long player_get_consumable_energy_restored_game_time(const player_datum* player, long energy_level, real energy_regeneration_rate);
const real* player_get_loadout_modifiers(const player_datum* player);
long player_get_modifier_value_integer(const player_datum* player, e_modifiers modifier_index, long default_value);
real player_get_modifier_value_real(const player_datum* player, e_modifiers modifier_index, real default_value);
real player_get_consumable_cooldown_init(const player_datum* player, long consumable_slot);
real player_get_consumable_cooldown(const player_datum* player, long consumable_slot);
void player_set_consumable_cooldown(player_datum* player, real cooldown, real cooldown_init);
long player_get_energy_regeneration_count(const player_datum* player);