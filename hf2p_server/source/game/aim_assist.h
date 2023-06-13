#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/game/aim_assist.hpp

#include <cseries\cseries.h>
#include <game\aim_target.h>

struct s_aim_assist_targeting_result
{
	c_aim_target_object aim_arget;
	dword model_target;
	real primary_autoaim_level;
	real secondary_autoaim_level;
	real_vector3d lead_vector;
	dword_flags flags;
};
static_assert(sizeof(s_aim_assist_targeting_result) == 0x28);