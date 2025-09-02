#pragma once
#include <math\real_math.h>

struct s_particle_state
{
	real_vector3d m_position;
	real_vector3d m_velocity;
	real_vector3d m_axis;
	float m_physical_rotation;
	float m_manual_rotation;
	float m_animated_frame;
	float m_manual_frame;
	float m_rotational_velocity;
	float m_frame_velocity;
	float m_birth_time;
	float m_inverse_lifespan;
	float m_age;
	real_quaternion	m_color;
	real_quaternion	m_initial_color;
	real_quaternion	m_random;
	real_quaternion	m_random2;
	float m_size;
	float m_aspect;
	float m_intensity;
	float m_black_point;
	float m_palette_v;
	float m_unknown3;
	float m_particle_scale;
};
