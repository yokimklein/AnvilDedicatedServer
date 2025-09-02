#pragma once
#include <cseries\cseries.h>
#include <units\units.h>

struct s_player_interaction
{
	short type;

	union
	{
		struct
		{
			short seat_index;
		} enter_vehicle;

		struct
		{
			byte_flags pick_up_flags;
		} pick_up_weapon;
	} data;

	long object_index;
};
static_assert(sizeof(s_player_interaction) == 0x8);

struct s_player_action_context
{
	s_player_interaction interaction;
	long melee_target_unit_index;
};
static_assert(sizeof(s_player_action_context) == 0xC);

struct player_action
{
	dword control_context;
	byte control_context_identifier;
	dword_flags control_flags;
	real_euler_angles2d angles;
	real_point2d throttle;
	struct
	{
		bool rotation_valid;
		bool player_locked_for_manipulation;
		real_euler_angles2d rotation_yaw_pitch;
		word_flags flags;
	};
	real trigger;
	real secondary_trigger;
	word_flags action_flags;
	s_unit_weapon_set weapon_set;
	word grenade_index;
	word zoom_level;
	s_player_action_context action_context;
	s_aim_assist_targeting_result targeting;
	bool velocity_exceeds_motion_tracker_threshold;
	qword action_test_flags;
};
static_assert(sizeof(player_action) == 0x80);