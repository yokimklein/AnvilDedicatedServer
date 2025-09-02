#include "game_engine_assault.h"

c_game_engine_assault_variant* c_game_engine_assault_variant::constructor()
{
	*(size_t*)this = base_address(0xD714A0); // set vftable
	return this;
}

void c_game_engine_assault_variant::set(c_game_engine_assault_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	get_miscellaneous_options_writeable()->set_teams_enabled(true);
	set_game_type(variant->get_game_type());
	set_respawn(variant->get_respawn());
	set_enemy_bomb_waypoint(variant->get_enemy_bomb_waypoint());
	set_reset_bomb_on_disarm(variant->get_reset_bomb_on_disarm());
	set_score_to_win(variant->get_score_to_win());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_bomb_arming_time(variant->get_bomb_arming_time());
	set_bomb_disarming_time(variant->get_bomb_disarming_time());
	set_bomb_fuse_time(variant->get_bomb_fuse_time());
	set_bomb_reset_time(variant->get_bomb_reset_time());
	set_carrier_traits(variant->get_carrier_traits(), force);
	set_arming_traits(variant->get_arming_traits(), force);

	zero_array(m_pad1);
}

void c_game_engine_assault_variant::set(s_game_engine_assault_variant_definition const* definition)
{
	DECLFUNC(0x1A7B50, void, __thiscall, c_game_engine_assault_variant*, s_game_engine_assault_variant_definition const*)(this, definition);
}

bool c_game_engine_assault_variant::get_reset_bomb_on_disarm() const
{
	return m_variant_flags.test(_assault_variant_flags_reset_bomb_on_disarm);
}

void c_game_engine_assault_variant::set_reset_bomb_on_disarm(bool reset_bomb_on_disarm)
{
	m_variant_flags.set(_assault_variant_flags_reset_bomb_on_disarm, reset_bomb_on_disarm);
}

bool c_game_engine_assault_variant::get_siege_mode() const
{
	return m_variant_flags.test(_assault_variant_flags_siege_mode);
}

void c_game_engine_assault_variant::set_siege_mode(bool siege_mode)
{
	m_variant_flags.set(_assault_variant_flags_siege_mode, siege_mode);
}

e_assault_respawn_settings c_game_engine_assault_variant::get_respawn() const
{
	return m_respawn;
}

void c_game_engine_assault_variant::set_respawn(e_assault_respawn_settings respawn)
{
	if (!VALID_INDEX(respawn, k_assault_respawn_settings))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid respawn setting '%d' outside range '[0, %d)'!", respawn, k_assault_respawn_settings);

		m_respawn = k_assault_respawn_settings_default;
	}
	else
	{
		m_respawn = respawn;
	}
}

e_assault_game_type_settings c_game_engine_assault_variant::get_game_type() const
{
	return m_game_type;
}

void c_game_engine_assault_variant::set_game_type(e_assault_game_type_settings game_type)
{
	if (!VALID_INDEX(game_type, k_assault_game_type_settings))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid game_type setting '%d' outside range '[0, %d)'!", game_type, k_assault_game_type_settings);

		m_game_type = k_assault_game_type_settings_default;
	}
	else
	{
		m_game_type = game_type;
	}
}

e_assault_enemy_bomb_waypoint_settings c_game_engine_assault_variant::get_enemy_bomb_waypoint() const
{
	return m_enemy_bomb_waypoint;
}

void c_game_engine_assault_variant::set_enemy_bomb_waypoint(e_assault_enemy_bomb_waypoint_settings enemy_bomb_waypoint)
{
	if (!VALID_INDEX(enemy_bomb_waypoint, k_assault_enemy_bomb_waypoint_settings))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid enemy_bomb_waypoint setting '%d' outside range '[0, %d)'!", enemy_bomb_waypoint, k_assault_enemy_bomb_waypoint_settings);

		m_enemy_bomb_waypoint = k_assault_enemy_bomb_waypoint_settings_default;
	}
	else
	{
		m_enemy_bomb_waypoint = enemy_bomb_waypoint;
	}
}

short c_game_engine_assault_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_assault_variant::set_score_to_win(short score_to_win)
{
	if (!VALID_INDEX(score_to_win, 50))
	{
		//generate_event(_event_level_warning, "game_engine:ctf: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 50);

		m_score_to_win = 3;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

short c_game_engine_assault_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

void c_game_engine_assault_variant::set_sudden_death_time(short sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_assault_variant_sudden_death_time))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _assault_variant_sudden_death_time_infinite, _assault_variant_sudden_death_time_5_minute);

		m_sudden_death_time = k_assault_variant_sudden_death_time_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

short c_game_engine_assault_variant::get_bomb_reset_time() const
{
	return m_bomb_reset_time;
}

void c_game_engine_assault_variant::set_bomb_reset_time(short bomb_reset_time)
{
	if (!VALID_INDEX(bomb_reset_time, 90))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid bomb_reset_time setting '%d' outside range '[%d, %d]'!", bomb_reset_time, 0, 90);

		m_bomb_reset_time = 30;
	}
	else
	{
		m_bomb_reset_time = bomb_reset_time;
	}
}

short c_game_engine_assault_variant::get_bomb_arming_time() const
{
	return m_bomb_arming_time;
}

void c_game_engine_assault_variant::set_bomb_arming_time(short bomb_arming_time)
{
	if (!VALID_INDEX(bomb_arming_time, 90))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid bomb_arming_time setting '%d' outside range '[%d, %d]'!", bomb_arming_time, 0, 90);

		m_bomb_arming_time = 35;
	}
	else
	{
		m_bomb_arming_time = bomb_arming_time;
	}
}

short c_game_engine_assault_variant::get_bomb_disarming_time() const
{
	return m_bomb_disarming_time;
}

void c_game_engine_assault_variant::set_bomb_disarming_time(short bomb_disarming_time)
{
	if (!VALID_INDEX(bomb_disarming_time, 90))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid bomb_disarming_time setting '%d' outside range '[%d, %d]'!", bomb_disarming_time, 0, 90);

		m_bomb_disarming_time = 35;
	}
	else
	{
		m_bomb_disarming_time = bomb_disarming_time;
	}
}

short c_game_engine_assault_variant::get_bomb_fuse_time() const
{
	return m_bomb_fuse_time;
}

void c_game_engine_assault_variant::set_bomb_fuse_time(short bomb_fuse_time)
{
	if (!VALID_INDEX(bomb_fuse_time, 90))
	{
		//generate_event(_event_level_warning, "game_engine:assault: invalid bomb_fuse_time setting '%d' outside range '[%d, %d]'!", bomb_fuse_time, 0, 90);

		m_bomb_fuse_time = 35;
	}
	else
	{
		m_bomb_fuse_time = bomb_fuse_time;
	}
}

c_player_traits* c_game_engine_assault_variant::get_carrier_traits_writeable()
{
	return &m_carrier_traits;
}

c_player_traits const* c_game_engine_assault_variant::get_carrier_traits() const
{
	return &m_carrier_traits;
}

void c_game_engine_assault_variant::set_carrier_traits(c_player_traits const* traits, bool force)
{
	m_carrier_traits.set(traits, force);
}

c_player_traits* c_game_engine_assault_variant::get_arming_traits_writeable()
{
	return &m_arming_traits;
}

c_player_traits const* c_game_engine_assault_variant::get_arming_traits() const
{
	return &m_arming_traits;
}

void c_game_engine_assault_variant::set_arming_traits(c_player_traits const* traits, bool force)
{
	m_arming_traits.set(traits, force);
}