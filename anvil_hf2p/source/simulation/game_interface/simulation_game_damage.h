#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>
#include <math\real_math.h>
#include <objects\damage.h>

enum e_damage_section_response_type
{
	_damage_section_receives_all_damage,
	_damage_section_receives_area_effect_damage,
	_damage_section_receives_local_damage,

	k_damage_section_response_type_count = 3
};

enum e_damage_aftermath_flags
{
	_damage_aftermath_body_depleted_bit = 0,
	_damage_aftermath_shield_depleted_bit,
	_damage_aftermath_area_of_effect_bit,
	_damage_aftermath_any_damage_dealt_bit,
	_damage_aftermath_damaged_by_friendly_bit,
	_damage_aftermath_silent_bit,
	_damage_aftermath_killed_instantly_bit,
	_damage_aftermath_primary_recipient_bit,
	_damage_aftermath_outside_aoe_dmg_range_bit,
	_damage_aftermath_object_destroyed_bit,
	_damage_aftermath_emp_bit,

	k_damage_aftermath_flags_count
};

struct s_simulation_damage_section_response_data
{
	long damage_section_index;
	long response_index; // max of 16
	e_damage_section_response_type damage_section_response_type;
};
static_assert(sizeof(s_simulation_damage_section_response_data) == 0xC);

struct s_simulation_damage_aftermath_event_data
{
	long damage_definition_index;
	long __unknown4;
	short damage_owner_player_index;
	bool direction_valid;
	char : 8;
	real_vector3d direction;
	bool epicenter_valid;
	real_vector3d epicenter_direction_vector;
	real epicenter_direction;
	real scale;
	real shake_scale;
	c_flags<e_damage_aftermath_flags, long, k_damage_aftermath_flags_count> flags;
	real shield_damage;
	real body_damage;
	short body_part;
	short node_index;
	long ping_type;
	long special_death_type;
	s_damage_reporting_info damage_reporting_info;
};
static_assert(sizeof(s_simulation_damage_aftermath_event_data) == 0x50);

struct s_damage_aftermath_result_data
{
	s_damage_reporting_info damage_reporting_info;
	c_flags<e_damage_aftermath_flags, long, k_damage_aftermath_flags_count> flags;
	long damage_definition_index;
	datum_index damage_owner_player_index;
	datum_index object_index;
	char __unknown14[4];
	real_vector3d direction;
	bool epicenter_valid;
	char pad[3];
	real_vector3d epicenter_direction_vector;
	real scale;
	real shake_scale;
	long body_part;
	long node_index;
	real body_damage;
	real shield_damage;
	char __unknown4C[4];
	long ping_type;
	long special_death_type;
};
static_assert(sizeof(s_damage_aftermath_result_data) == 0x58);

void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type);
void simulation_action_damage_aftermath(datum_index object_index, s_damage_aftermath_result_data const* result_data);
bool should_send_damage_aftermath_event(datum_index object_index, s_damage_aftermath_result_data const* result_data);
bool damage_aftermath_is_important(datum_index object_index, s_damage_aftermath_result_data const* result_data);
void build_damage_aftermath_event_data(datum_index object_index, s_damage_aftermath_result_data const* result_data, s_simulation_damage_aftermath_event_data* out_event_data, datum_index out_object_references[2]);
void simulation_action_damage_aftermath_exclusive_list(datum_index object_index, s_damage_aftermath_result_data const* result_data, datum_index const* player_indices, long player_count);