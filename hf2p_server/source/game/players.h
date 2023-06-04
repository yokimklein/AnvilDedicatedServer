#pragma once
#include "..\cseries\cseries.h"
#include "..\game\aim_target.h"
#include "..\objects\damage.h"
#include "..\memory\data.h"
#include "player_configuration.h"
#include "game_engine_player_traits.h"
#include "..\dllmain.h"

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
	uchar control_context_identifier;
	uchar field_4C;
	uchar field_4B;
	__int16 character_type_index;
	__int16 previous_character_type_index;
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
	__int16 sprint_cooldown;
	__int16 sprint_cooldown2;
	real_vector3d position;
	s_player_configuration configuration;
	s_player_configuration desired_configuration;
	int single_player_respawn_timer;
	bool early_respawn_requested;
	bool respawn_in_progress;
	bool respawn_forced;
	int respawn_timer;
	int respawn_timer_countdown_seconds;
	ulong respawn_time_penalty;
	char blocking_teleporter;
	int dead_timer;
	int in_game_timer;
	int alive_timer;
	int unknown_input_global_ticks;
	int grenade_regeneration_timer;
	int unknown_object_index;
	int last_aim_target_time;
	ushort ticks_sprinting;
	ushort field_2CE6;
	ushort sprint_restoration_timer;
	ushort sprint_depleted_timer;
	bool sprint_disabled;
	ushort unknown_ticks_4;
	c_aim_target_object aim_target_object;
	int field_2CFC;
	struct {
		char mask;
		long object_index;
		long field_8;
	}
	tracking_objects[8];
	ushort respawn_duration_seconds;
	bool first_spawn;
	uchar respawn_failed_reason;
	struct {
		long LastUsedTick;
		long Duration;
		long DamageAccelerationScale;
		long field_C;
		float field_10;
		float field_14;
	} tank_mode_state;
	struct {
		long last_used_game_time;
		float duration;
		float damage_acceleration_scale;
		long field_C;
	} reactive_armor_state;
	long stamina_restore_near_death_timer;
	bool grenade_scavenger_ability_used;
	long last_aim_assist_target_player_index;
	short unused;
	char game_engine_vehicle_use_timer;
	char game_engine_simulation_update_timer;
	char game_engine_simulation_update_timer2;
	bool was_sneaking;
	bool is_sneaking;
	ushort aim_assist_update_timer;
	ushort lives;
	long game_grief_player_index;
	ulong last_killed_game_time;
	c_player_traits traits;
	long powerup_pickup_times[3];
	long spectating_player_index;
	bool sandbox_rotating;
	float sandbox_extend_distance;
	real_vector3d sandbox_rotation_throttle;
	ushort time_in_hill;
	ushort time_outside_Hill;
	ushort time_hill_Left;
	short : 16;
	short nemesis_totals[16];
	char field_2E14;
	char field_2E15;
	char revenge_shield_boost_multiplier;
	char field_2E17;
	ulong last_hit_marker_game_time;
	long revenge_player_Index;
	s_damage_reporting_info revenge_event_type;
	bool revenge_taken;
	char : 8;
	char : 8;
	char : 8;
	uchar unknown_damage_reporting_timer;
	char unknown_damage_reporting_timer2;
	s_damage_reporting_info unknown_damagereportinginfo;
	s_damage_reporting_info unknown_damagereportinginfo2;
	s_damage_reporting_info unknown_damagereportinginfo3;
	char field_2E38;
	char field_2E39;
	short field_2E3A;
	short field_2E3C;
	short field_2E3E;
	long last_ejected_from_vehicle_index;
	ulong last_ejected_from_vehicle_game_time;
	long assassination_victim_unit_index;
	bool is_assassination_victim;
	real_point3d assasination_authorative_position;
	real_vector3d assasination_authorative_froward;
	uchar assassination_state[60];
	struct {
		ushort field_0;
		s_damage_reporting_info damage_reporting_info;
		ulong time;
	} damage_history[8];
	long field_2F04;

	byte unknown_data[8];
};
static_assert(sizeof(s_player_datum) == 0x19B0);

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
long player_mapping_get_input_user(word player_index);
void player_set_facing(datum_index player_index, real_vector3d* forward);
void player_control_set_facing(long input_user_index, real_vector3d* forward);
long player_index_from_absolute_player_index(short absolute_player_index);
void __fastcall player_increment_control_context(datum_index player_index);

FUNCTION_DEF(0xBA0F0, void, __fastcall, player_clear_assassination_state, datum_index player_index);