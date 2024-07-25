#pragma once
#include <cseries\cseries.h>
#include <memory\data.h>

enum e_damage_section_response_type
{
	_damage_section_receives_all_damage,
	_damage_section_receives_area_effect_damage,
	_damage_section_receives_local_damage,

	k_damage_section_response_type_count = 3
};

struct s_simulation_damage_section_response_data
{
	long damage_section_index;
	long response_index; // max of 16
	c_enum<e_damage_section_response_type, long, k_damage_section_response_type_count> damage_section_response_type;
};
static_assert(sizeof(s_simulation_damage_section_response_data) == 0xC);

void simulation_action_damage_section_response(datum_index object_index, long damage_section_index, long response_index, e_damage_section_response_type damage_section_response_type);