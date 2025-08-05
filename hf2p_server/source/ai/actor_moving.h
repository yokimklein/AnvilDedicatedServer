#pragma once
#include <cseries/cseries.h>
#include <ai/ai_actions.h>
#include <ai/sector.h>
#include <ai/actor_firing_position.h>

struct c_destination_orders
{
	short m_destination_type;
	short m_flags;

	union
	{
		struct
		{
			c_ai_point3d point;
			c_sector_ref sector_ref;
		} m_raw;

		struct
		{
			firing_position_ref firing_position_index;
		} m_firing_position;

		struct
		{
			long prop_index;
		} m_prop;
	};

	long m_ignore_target_object_index;
	real m_scripted_speed;
	real m_scripted_goal_throttle;
	real m_destination_tolerance;
	real m_raw_path_destination_tolerance;
	real_vector3d m_destination_facing;
	c_ai_action m_destination_action;
};
static_assert(sizeof(c_destination_orders) == 0x68);