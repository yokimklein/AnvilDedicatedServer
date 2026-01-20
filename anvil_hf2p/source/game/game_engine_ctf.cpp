#include "game_engine_ctf.h"
#include <items\weapons.h>
#include <game\game_engine_team.h>

c_game_engine_ctf_variant* c_game_engine_ctf_variant::constructor()
{
	*(size_t*)this = base_address(0xD71470); // set vftable
	return this;
}

void c_game_engine_ctf_variant::set(c_game_engine_ctf_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	get_miscellaneous_options_writeable()->set_teams_enabled(true);
	set_home_flag_waypoint(variant->get_home_flag_waypoint());
	set_game_type(variant->get_game_type());
	set_respawn(variant->get_respawn());
	set_score_to_win(variant->get_score_to_win());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_flag_reset_time(variant->get_flag_reset_time());
	set_touch_return_timeout(variant->get_touch_return_timeout());
	set_flag_at_home_to_score(variant->get_flag_at_home_to_score());
	set_carrier_traits(variant->get_carrier_traits(), force);

	zero_array(m_pad1);
}

void c_game_engine_ctf_variant::set(s_game_engine_ctf_variant_definition const* definition)
{
	DECLFUNC(0x1AE560, void, __thiscall, c_game_engine_ctf_variant*, s_game_engine_ctf_variant_definition const*)(this, definition);
}

bool c_game_engine_ctf_variant::get_flag_at_home_to_score() const
{
	return m_variant_flags.test(_ctf_variant_flags_flag_at_home_to_score_flag);
}

void c_game_engine_ctf_variant::set_flag_at_home_to_score(bool flag_at_home_to_score)
{
	m_variant_flags.set(_ctf_variant_flags_flag_at_home_to_score_flag, flag_at_home_to_score);
}

e_ctf_home_flag_waypoint_settings c_game_engine_ctf_variant::get_home_flag_waypoint() const
{
	return m_home_flag_waypoint;
}

void c_game_engine_ctf_variant::set_home_flag_waypoint(e_ctf_home_flag_waypoint_settings home_flag_waypoint)
{
	if (!VALID_INDEX(home_flag_waypoint, k_ctf_home_flag_waypoint_settings))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid home_flag_waypoint setting '%d' outside range '[0, %d)'!", home_flag_waypoint, k_ctf_home_flag_waypoint_settings);

		m_home_flag_waypoint = k_ctf_home_flag_waypoint_settings_default;
	}
	else
	{
		m_home_flag_waypoint = home_flag_waypoint;
	}
}

e_ctf_game_type_settings c_game_engine_ctf_variant::get_game_type() const
{
	return m_game_type;
}

void c_game_engine_ctf_variant::set_game_type(e_ctf_game_type_settings game_type)
{
	if (!VALID_INDEX(m_game_type, k_ctf_game_type_settings))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid game_type setting '%d' outside range '[0, %d)'!", game_type, k_ctf_game_type_settings);

		m_game_type = k_ctf_game_type_settings_default;
	}
	else
	{
		m_game_type = game_type;
	}
}

e_ctf_respawn_settings c_game_engine_ctf_variant::get_respawn() const
{
	return m_respawn;
}

void c_game_engine_ctf_variant::set_respawn(e_ctf_respawn_settings respawn)
{
	if (!VALID_INDEX(respawn, k_ctf_respawn_settings))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid respawn setting '%d' outside range '[0, %d)'!", respawn, k_ctf_respawn_settings);

		m_respawn = k_ctf_respawn_settings_default;
	}
	else
	{
		m_respawn = respawn;
	}
}

short c_game_engine_ctf_variant::get_touch_return_timeout() const
{
	return m_touch_return_timeout;
}

void c_game_engine_ctf_variant::set_touch_return_timeout(short touch_return_timeout)
{
	m_touch_return_timeout = touch_return_timeout;
}

short c_game_engine_ctf_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

void c_game_engine_ctf_variant::set_sudden_death_time(short sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_ctf_sudden_death_times))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _ctf_sudden_death_time_infinite, _ctf_sudden_death_time_5_minute);

		m_sudden_death_time = k_ctf_sudden_death_times_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

short c_game_engine_ctf_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_ctf_variant::set_score_to_win(short score_to_win)
{
	if (!VALID_INDEX(score_to_win, 50))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 50);

		m_score_to_win = 5;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

short c_game_engine_ctf_variant::get_flag_reset_time() const
{
	return m_flag_reset_time;
}

void c_game_engine_ctf_variant::set_flag_reset_time(short flag_reset_time)
{
	if (!VALID_INDEX(flag_reset_time, 300))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid flag_reset_time setting '%d' outside range '[%d, %d]'!", flag_reset_time, 0, 300);

		m_flag_reset_time = 30;
	}
	else
	{
		m_flag_reset_time = flag_reset_time;
	}
}

c_player_traits* c_game_engine_ctf_variant::get_carrier_traits_writeable()
{
	return &m_carrier_traits;
}

c_player_traits const* c_game_engine_ctf_variant::get_carrier_traits() const
{
	return &m_carrier_traits;
}

void c_game_engine_ctf_variant::set_carrier_traits(c_player_traits const* traits, bool force)
{
	m_carrier_traits.set(traits, force);
}

// $TODO:
//void flag_reset(datum_index weapon_index)
//{
//	weapon_datum* weapon = (weapon_datum*)object_get_and_verify_type(weapon_index, _object_mask_weapon);
//	ASSERT(!game_is_predicted());
//
//	e_game_team team_index = weapon->weapon.team_index;
//	if (team_index == _game_team_none)
//	{
//		return;
//	}
//
//	e_multiplayer_team_designator team_designator = game_engine_team_index_to_team_designator(team_index);
//	if (neutral_flag_ctf())
//	{
//		team_designator = _multiplayer_team_designator_neutral;
//	}
//
//	if (!get_flag_spawn_position(team_designator, &flag_spawn_position, &spawn_position_area))
//	{
//		return;
//	}
//
//	if (weapon->object.parent_object_index != NONE)
//	{
//		object_detach(weapon_index);
//	}
//}