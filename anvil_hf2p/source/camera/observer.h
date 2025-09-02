#pragma once
#include <cseries\cseries.h>

struct s_observer_command
{
	dword_flags flags;
	union
	{
		struct
		{
			real_point3d focus_position;
			real_vector3d focus_offset;
			real_point2d crosshair_location;
			real focus_distance;
			real field_of_view;
			real_vector3d forward;
			real_vector3d up;
		};

		real parameters[16];
	};

	real_vector3d focus_velocity;
	real_matrix4x3 focus_space;

	dword __unknown84;

	real_point3d center;
	real timer;

	long parent_objects[2];
	long number_of_parents_objects;

	real_point3d physics_pill_position;
	real physics_pill_height;
	real physics_pill_radius;

	union
	{
		struct
		{
			byte position_flags;
			byte focus_offset_flags;
			byte look_shift_flags;
			byte distance_flags;
			byte field_of_view_flags;
			byte orientation_flags;
		};
		byte parameter_flags[6];
	};

	union
	{
		struct
		{
			real position_timer;
			real focus_offset_timer;
			real look_shift_timer;
			real distance_timer;
			real field_of_view_timer;
			real orientation_timer;
		};
		real parameter_timers[6];
	};

	byte __dataD8[0x14];
};
static_assert(sizeof(s_observer_command) == 0xEC);
static_assert(0x84 == OFFSETOF(s_observer_command, __unknown84));
static_assert(0xB8 == OFFSETOF(s_observer_command, parameter_flags));
static_assert(0xC0 == OFFSETOF(s_observer_command, parameter_timers));
static_assert(0xD8 == OFFSETOF(s_observer_command, __dataD8));