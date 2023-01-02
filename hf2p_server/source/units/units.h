#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/units/units.hpp

#include "..\cseries\cseries.h"
#include "..\game\aim_assist.h"

enum e_weapon_set
{
	_weapon_set_primary = 0,
	_weapon_set_secondary,

	k_weapon_set_count
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