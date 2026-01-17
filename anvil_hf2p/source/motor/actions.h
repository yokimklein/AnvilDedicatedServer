#pragma once
#include <math\real_math.h>
#include <ai\actor_moving_hints.h>

struct c_interaction_target
{
	union
	{
		ulong m_data;
		long m_object_index;
		c_hint_ref m_hint_ref;
	};
	bool m_object_interaction;
};

struct s_action_state
{
	enum e_player_melee_flag
	{
		// $TODO:
	};

	ulong flags;
	short timer;
	char throw_state;
	bool throw_predicted;
	char throw_ticks;
	char throw_full_power_ticks;
	char throw_completion_ticks;
	long throw_object_index;
	long throw_type_index;
	char smash_type;
	char smash_blind;
	long smash_object_index;
	short smash_object_ticks;
	short resurrection_ticks;
	real_point3d perch_jump_target_point;
	bool phase_destination_valid;
	real_point3d phase_destination;
	long player_melee_state;
	char player_melee_prediction;
	byte player_melee_scale;
	char player_melee_tick;
	char player_melee_attack_tick;
	char player_melee_combo_tick;
	char player_melee_end_tick;
	bool player_melee_reached_attack_frame;
	bool player_melee_resolved;
	short emit_ai_ticks;
	short emit_ai_squad_index;
	short emit_ai_fire_team_index;
	short emit_ai_starting_location_index;
	short berserk_stow_weapon;
	bool turret_deployed;
	long morph_character_definition_index;
	bool morph_propogate;
	bool morph_to_pending;
	bool morph_hostile;
	c_interaction_target interaction_target;
	long infection_spew_spawner_index;
	long last_player_melee_resolved_time;
};
