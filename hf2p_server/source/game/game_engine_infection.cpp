#include "game_engine_infection.h"

c_game_engine_infection_variant* c_game_engine_infection_variant::constructor()
{
	*(size_t*)this = BASE_ADDRESS(0xD71320); // set vftable
	return this;
}

void c_game_engine_infection_variant::set(c_game_engine_infection_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	get_miscellaneous_options_writeable()->set_teams_enabled(false);
	set_respawn_on_haven_move(variant->get_respawn_on_haven_move());
	set_safe_havens(variant->get_safe_havens());
	set_next_zombie(variant->get_next_zombie());
	set_initial_zombie_count(variant->get_initial_zombie_count());
	set_safe_haven_movement_time(variant->get_safe_haven_movement_time());
	set_zombie_kill_points(variant->get_zombie_kill_points());
	set_infection_points(variant->get_infection_points());
	set_safe_haven_arrival_points(variant->get_safe_haven_arrival_points());
	set_suicide_points(variant->get_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_last_man_bonus_points(variant->get_last_man_bonus_points());
	set_zombie_traits(variant->get_zombie_traits(), force);
	set_first_zombie_traits(variant->get_first_zombie_traits(), force);
	set_safe_haven_defender_traits(variant->get_safe_haven_defender_traits(), force);
	set_last_human_traits(variant->get_last_human_traits(), force);

	zero_array(m_pad1);
}

void c_game_engine_infection_variant::set(s_game_engine_infection_variant_definition const* definition)
{
	DECLFUNC(0x1ADCB0, void, __thiscall, c_game_engine_infection_variant*, s_game_engine_infection_variant_definition const*)(this, definition);
}

bool c_game_engine_infection_variant::get_respawn_on_haven_move() const
{
	return m_variant_flags.test(_infection_variant_flags_respawn_on_haven_move);
}

void c_game_engine_infection_variant::set_respawn_on_haven_move(bool respawn_on_haven_move)
{
	return m_variant_flags.set(_infection_variant_flags_respawn_on_haven_move, respawn_on_haven_move);
}

e_infection_safe_havens_settings c_game_engine_infection_variant::get_safe_havens() const
{
	return m_safe_havens;
}

void c_game_engine_infection_variant::set_safe_havens(e_infection_safe_havens_settings safe_havens)
{
	if (!VALID_INDEX(safe_havens, k_infection_safe_havens_settings))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid safe_havens setting '%d' outside range '[0, %d)'!", safe_havens, k_infection_safe_havens_settings);

		m_safe_havens = k_infection_safe_havens_settings_default;
	}
	else
	{
		m_safe_havens = safe_havens;
	}
}

e_infection_next_zombie_settings c_game_engine_infection_variant::get_next_zombie() const
{
	return m_next_zombie;
}

void c_game_engine_infection_variant::set_next_zombie(e_infection_next_zombie_settings next_zombie)
{
	if (!VALID_INDEX(next_zombie, k_infection_next_zombie_settings))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid next_zombie setting '%d' outside range '[0, %d)'!", next_zombie, k_infection_next_zombie_settings);

		m_next_zombie = k_infection_next_zombie_settings_default;
	}
	else
	{
		m_next_zombie = next_zombie;
	}
}

e_infection_initial_zombie_count_settings c_game_engine_infection_variant::get_initial_zombie_count() const
{
	return m_initial_zombie_count;
}

void c_game_engine_infection_variant::set_initial_zombie_count(e_infection_initial_zombie_count_settings initial_zombie_count)
{
	if (!VALID_INDEX(initial_zombie_count, k_infection_initial_zombie_count_settings))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid initial_zombie_count setting '%d' outside range '[0, %d)'!", initial_zombie_count, k_infection_initial_zombie_count_settings);

		m_initial_zombie_count = k_infection_initial_zombie_count_settings_default;
	}
	else
	{
		m_initial_zombie_count = initial_zombie_count;
	}
}

short c_game_engine_infection_variant::get_safe_haven_movement_time() const
{
	return m_safe_haven_movement_time;
}

void c_game_engine_infection_variant::set_safe_haven_movement_time(short safe_haven_movement_time)
{
	if (!VALID_INDEX(safe_haven_movement_time, 120))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid safe_haven_movement_time setting '%d' outside range '[%d, %d]'!", safe_haven_movement_time, 0, 120);

		m_safe_haven_movement_time = 30;
	}
	else
	{
		m_safe_haven_movement_time = safe_haven_movement_time;
	}
}

char c_game_engine_infection_variant::get_zombie_kill_points() const
{
	return m_zombie_kill_points;
}

void c_game_engine_infection_variant::set_zombie_kill_points(char zombie_kill_points)
{
	if (!VALID_INDEX(zombie_kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid zombie_kill_points setting '%d' outside range '[%d, %d]'!", zombie_kill_points, -10, 10);

		m_zombie_kill_points = 1;
	}
	else
	{
		m_zombie_kill_points = zombie_kill_points;
	}
}

char c_game_engine_infection_variant::get_infection_points() const
{
	return m_infection_points;
}

void c_game_engine_infection_variant::set_infection_points(char infection_points)
{
	if (!VALID_INDEX(infection_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid infection_points setting '%d' outside range '[%d, %d]'!", infection_points, -10, 10);

		m_infection_points = 0;
	}
	else
	{
		m_infection_points = infection_points;
	}
}

char c_game_engine_infection_variant::get_safe_haven_arrival_points() const
{
	return m_safe_haven_arrival_points;
}

void c_game_engine_infection_variant::set_safe_haven_arrival_points(char safe_haven_arrival_points)
{
	if (!VALID_INDEX(safe_haven_arrival_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid safe_haven_arrival_points setting '%d' outside range '[%d, %d]'!", safe_haven_arrival_points, -10, 10);

		m_safe_haven_arrival_points = 0;
	}
	else
	{
		m_safe_haven_arrival_points = safe_haven_arrival_points;
	}
}

char c_game_engine_infection_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_infection_variant::set_suicide_points(char suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = -1;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

char c_game_engine_infection_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_infection_variant::set_betrayal_points(char betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = -1;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

char c_game_engine_infection_variant::get_last_man_bonus_points() const
{
	return m_last_man_bonus_points;
}

void c_game_engine_infection_variant::set_last_man_bonus_points(char last_man_bonus_points)
{
	if (!VALID_INDEX(last_man_bonus_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:infection: invalid last_man_bonus_points setting '%d' outside range '[%d, %d]'!", last_man_bonus_points, -10, 10);

		m_last_man_bonus_points = 0;
	}
	else
	{
		m_last_man_bonus_points = last_man_bonus_points;
	}
}

c_player_traits* c_game_engine_infection_variant::get_zombie_traits_writeable()
{
	return &m_zombie_traits;
}

c_player_traits const* c_game_engine_infection_variant::get_zombie_traits() const
{
	return &m_zombie_traits;
}

void c_game_engine_infection_variant::set_zombie_traits(c_player_traits const* traits, bool force)
{
	m_zombie_traits.set(traits, force);
}

c_player_traits* c_game_engine_infection_variant::get_first_zombie_traits_writeable()
{
	return &m_first_zombie_traits;
}

c_player_traits const* c_game_engine_infection_variant::get_first_zombie_traits() const
{
	return &m_first_zombie_traits;
}

void c_game_engine_infection_variant::set_first_zombie_traits(c_player_traits const* traits, bool force)
{
	m_first_zombie_traits.set(traits, force);
}

c_player_traits* c_game_engine_infection_variant::get_safe_haven_defender_traits_writeable()
{
	return &m_safe_haven_defender_traits;
}

c_player_traits const* c_game_engine_infection_variant::get_safe_haven_defender_traits() const
{
	return &m_safe_haven_defender_traits;
}

void c_game_engine_infection_variant::set_safe_haven_defender_traits(c_player_traits const* traits, bool force)
{
	m_safe_haven_defender_traits.set(traits, force);
}

c_player_traits* c_game_engine_infection_variant::get_last_human_traits_writeable()
{
	return &m_last_human_traits;
}

c_player_traits const* c_game_engine_infection_variant::get_last_human_traits() const
{
	return &m_last_human_traits;
}

void c_game_engine_infection_variant::set_last_human_traits(c_player_traits const* traits, bool force)
{
	m_last_human_traits.set(traits, force);
}