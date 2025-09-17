#pragma once
#include "camera\camera.h"

class c_flying_camera : public c_camera
{
public:
	//void constructor(long user_index)
	//{
	//	//INVOKE_CLASS_MEMBER(0x0, c_flying_camera, constructor, user_index);
	//}

	void set_lock_in_xy_plane(bool value)
	{
		m_flags.set(_lock_in_plane, value);
	}

	void set_collision(bool value)
	{
		m_flags.set(_has_collision, value);
	}

	void enable_orientation(bool value)
	{
		m_flags.set(_orientation_enabled, value);
	}

	void enable_movement(bool value)
	{
		m_flags.set(_movement_enabled, value);
	}

	//protected:
	enum e_flags
	{
		_zoomed = 0,
		_lock_in_plane,
		_has_collision,
		_orientation_enabled,
		_movement_enabled,
		_panning_enabled,

		k_number_of_flags
	};

	real_point3d m_position;
	real_euler_angles2d m_orientation;
	real m_roll;
	real m_time_boosting;
	real m_time_moving_vertical;

	bool __unknown30;
	real __unknown34;

	c_flags<e_flags, long, k_number_of_flags> m_flags;

	byte unused[0x10];
};
static_assert(sizeof(c_flying_camera) == 0x4C);