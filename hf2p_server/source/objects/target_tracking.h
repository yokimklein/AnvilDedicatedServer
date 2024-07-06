#pragma once
#include <game\aim_target.h>

class c_tracked_target
{
	c_aim_target_object aim_target;
	short : 16;
	short : 16;
	real looking_theta;
	short : 16;
	short : 16;
	bool locked;
};

class c_target_tracking_system
{
	c_tracked_target tracked_targets[2];
	long best_target_index;
	long : 32;
};
static_assert(sizeof(c_target_tracking_system) == 0x40);