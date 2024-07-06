#pragma once
#include <units\units.h>

enum e_biped_flag
{
	_biped_flag0,

	k_biped_flag_count
};

struct s_biped_data : s_unit_data
{
	c_flags<e_biped_flag, word, k_biped_flag_count> biped_flags;
};
static_assert(0x598 == OFFSETOF(s_biped_data, biped_flags));
// TODO: verify size & fields

bool __fastcall biped_calculate_melee_aiming(datum_index biped_index, real_vector3d* melee_aiming_vector);
bool __fastcall biped_update_melee_turning(datum_index biped_index);