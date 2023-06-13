#pragma once
#include <units\units.h>

enum e_biped_flag
{
	_biped_flag0,

	k_biped_flag_count
};

struct s_biped_data : s_unit_data
{
	long unknown1;
	long unknown2;
	c_flags<e_biped_flag, word, k_biped_flag_count> biped_flags;
};
// TODO: verify size & fields

FUNCTION_DEF(0x4409F0, bool, __fastcall, biped_calculate_melee_aiming, datum_index biped_index, real_vector3d* melee_aiming_vector);
bool __fastcall biped_update_melee_turning(datum_index biped_index);