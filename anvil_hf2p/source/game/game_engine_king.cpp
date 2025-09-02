#include "game_engine_king.h"

c_game_engine_king_variant* c_game_engine_king_variant::constructor()
{
	*(size_t*)this = base_address(0xD713B0); // set vftable
	return this;
}

void c_game_engine_king_variant::set(c_game_engine_king_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_opaque_hill(variant->get_opaque_hill());
	set_score_to_win(variant->get_score_to_win());
	set_moving_hill(variant->get_moving_hill());
	set_moving_hill_order(variant->get_moving_hill_order());
	set_inside_hill_points(variant->get_inside_hill_points());
	set_outside_hill_points(variant->get_outside_hill_points());
	set_uncontested_hill_bonus(variant->get_uncontested_hill_bonus());
	set_kill_points(variant->get_kill_points());
	set_inside_hill_traits(variant->get_inside_hill_traits(), force);

	zero_array(m_pad1);
}

void c_game_engine_king_variant::set(s_game_engine_king_variant_definition const* definition)
{
	DECLFUNC(0x1AC240, void, __thiscall, c_game_engine_king_variant*, s_game_engine_king_variant_definition const*)(this, definition);
}

bool c_game_engine_king_variant::get_opaque_hill() const
{
	return m_variant_flags.test(_king_flags_setting_opaque_hill);
}

void c_game_engine_king_variant::set_opaque_hill(bool opaque_hill)
{
	m_variant_flags.set(_king_flags_setting_opaque_hill, opaque_hill);
}

short c_game_engine_king_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_king_variant::set_score_to_win(short score_to_win)
{
	if (!VALID_INDEX(score_to_win, 1000))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, 0, 1000);

		m_score_to_win = 100;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

e_king_moving_hill_settings c_game_engine_king_variant::get_moving_hill() const
{
	return m_moving_hill;
}

short c_game_engine_king_variant::get_hill_move_in_seconds() const
{
	e_king_moving_hill_settings setting = get_moving_hill();

	const long minimum_value = _king_moving_hill_settings_10_seconds;
	if (IN_RANGE_INCLUSIVE(setting, minimum_value, _king_moving_hill_settings_5_minutes))
		return k_king_moving_hill_values[setting - minimum_value];

	return k_king_moving_hill_values[k_king_moving_hill_settings_default - minimum_value];
}

void c_game_engine_king_variant::set_moving_hill(e_king_moving_hill_settings moving_hill)
{
	if (!VALID_INDEX(moving_hill, k_king_moving_hill_settings))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid moving_hill setting '%d' outside range '[0, %d)'!", moving_hill, k_king_moving_hill_settings);

		m_moving_hill = k_king_moving_hill_settings_default;
	}
	else
	{
		m_moving_hill = moving_hill;
	}
}

e_king_moving_hill_order_settings c_game_engine_king_variant::get_moving_hill_order() const
{
	return m_moving_hill_order;
}

void c_game_engine_king_variant::set_moving_hill_order(e_king_moving_hill_order_settings moving_hill_order)
{
	if (!VALID_INDEX(moving_hill_order, k_king_moving_hill_order_settings))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid moving_hill_order setting '%d' outside range '[0, %d)'!", moving_hill_order, k_king_moving_hill_order_settings);

		m_moving_hill_order = k_king_moving_hill_order_settings_default;
	}
	else
	{
		m_moving_hill_order = moving_hill_order;
	}
}

char c_game_engine_king_variant::get_uncontested_hill_bonus() const
{
	return m_uncontested_hill_bonus;
}

void c_game_engine_king_variant::set_uncontested_hill_bonus(char uncontested_hill_bonus)
{
	if (!VALID_INDEX(uncontested_hill_bonus + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid uncontested_hill_bonus setting '%d' outside range '[%d, %d]'!", uncontested_hill_bonus, -10, 10);

		m_uncontested_hill_bonus = 0;
	}
	else
	{
		m_uncontested_hill_bonus = uncontested_hill_bonus;
	}
}

char c_game_engine_king_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_king_variant::set_kill_points(char kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

char c_game_engine_king_variant::get_inside_hill_points() const
{
	return m_inside_hill_points;
}

void c_game_engine_king_variant::set_inside_hill_points(char inside_hill_points)
{
	if (!VALID_INDEX(inside_hill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid inside_hill_points setting '%d' outside range '[%d, %d]'!", inside_hill_points, -10, 10);

		m_inside_hill_points = 0;
	}
	else
	{
		m_inside_hill_points = inside_hill_points;
	}
}

char c_game_engine_king_variant::get_outside_hill_points() const
{
	return m_outside_hill_points;
}

void c_game_engine_king_variant::set_outside_hill_points(char outside_hill_points)
{
	if (!VALID_INDEX(outside_hill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:king: invalid outside_hill_points setting '%d' outside range '[%d, %d]'!", outside_hill_points, -10, 10);

		m_outside_hill_points = 0;
	}
	else
	{
		m_outside_hill_points = outside_hill_points;
	}
}

c_player_traits* c_game_engine_king_variant::get_inside_hill_traits_writeable()
{
	return &m_inside_hill_traits;
}

c_player_traits const* c_game_engine_king_variant::get_inside_hill_traits() const
{
	return &m_inside_hill_traits;
}

void c_game_engine_king_variant::set_inside_hill_traits(c_player_traits const* traits, bool force)
{
	m_inside_hill_traits.set(traits, force);
}