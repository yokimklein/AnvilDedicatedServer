#include "game_engine_juggernaut.h"

c_game_engine_juggernaut_variant* c_game_engine_juggernaut_variant::constructor()
{
	*(size_t*)this = base_address(0xD714D0); // set vftable
	return this;
}

void c_game_engine_juggernaut_variant::set(c_game_engine_juggernaut_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_allied_against_juggernaut(variant->get_allied_against_juggernaut());
	set_respawn_on_lone_juggernaut(variant->get_respawn_on_lone_juggernaut());
	set_destination_zones_enabled(variant->get_destination_zones_enabled());
	set_score_to_win_round(static_cast<short>(variant->get_score_to_win_round()));

	zero_array(m_pad);

	set_initial_juggernaut(variant->get_initial_juggernaut());
	set_next_juggernaut(variant->get_next_juggernaut());
	set_zone_movement(variant->get_zone_movement());
	set_zone_order(variant->get_zone_order());
	set_kill_points(variant->get_kill_points());
	set_juggernaut_kill_points(variant->get_juggernaut_kill_points());
	set_kill_as_juggernaut_points(variant->get_kill_as_juggernaut_points());
	set_destination_arrival_points(variant->get_destination_arrival_points());
	set_suicide_points(variant->get_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_juggernaut_delay(variant->get_juggernaut_delay());
	set_juggernaut_traits(get_juggernaut_traits(), force);

	zero_array(m_pad1);
}

void c_game_engine_juggernaut_variant::set(s_game_engine_juggernaut_variant_definition const* definition)
{
	DECLFUNC(0x1AA590, void, __thiscall, c_game_engine_juggernaut_variant*, s_game_engine_juggernaut_variant_definition const*)(this, definition);
}

bool c_game_engine_juggernaut_variant::get_allied_against_juggernaut() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_allied_against_juggernaut);
}

void c_game_engine_juggernaut_variant::set_allied_against_juggernaut(bool allied_against_juggernaut)
{
	m_variant_flags.set(_juggernaut_variant_flags_allied_against_juggernaut, allied_against_juggernaut);
}

bool c_game_engine_juggernaut_variant::get_respawn_on_lone_juggernaut() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_respawn_on_lone_juggernaut);
}

void c_game_engine_juggernaut_variant::set_respawn_on_lone_juggernaut(bool respawn_on_lone_juggernaut)
{
	m_variant_flags.set(_juggernaut_variant_flags_respawn_on_lone_juggernaut, respawn_on_lone_juggernaut);
}

bool c_game_engine_juggernaut_variant::get_destination_zones_enabled() const
{
	return m_variant_flags.test(_juggernaut_variant_flags_destination_zones_enabled);
}

void c_game_engine_juggernaut_variant::set_destination_zones_enabled(bool destination_zones_enabled)
{
	m_variant_flags.set(_juggernaut_variant_flags_destination_zones_enabled, destination_zones_enabled);
}

void c_game_engine_juggernaut_variant::set_score_to_win_round(short score_to_win_round)
{
	if (!VALID_INDEX(score_to_win_round, 500))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid score_to_win_round setting '%d' outside range '[%d, %d]'!", score_to_win_round, 0, 500);

		m_score_to_win_round = 15;
	}
	else
	{
		m_score_to_win_round = score_to_win_round;
	}
}

e_juggernaut_initial_juggernaut_settings c_game_engine_juggernaut_variant::get_initial_juggernaut() const
{
	return m_initial_juggernaut;
}

void c_game_engine_juggernaut_variant::set_initial_juggernaut(e_juggernaut_initial_juggernaut_settings initial_juggernaut)
{
	if (!VALID_INDEX(initial_juggernaut, k_juggernaut_initial_juggernaut_settings))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid initial_juggernaut setting '%d' outside range '[0, %d)'!", initial_juggernaut, k_juggernaut_next_juggernaut_settings);

		m_initial_juggernaut = k_juggernaut_initial_juggernaut_settings_default;
	}
	else
	{
		m_initial_juggernaut = initial_juggernaut;
	}
}

e_juggernaut_next_juggernaut_settings c_game_engine_juggernaut_variant::get_next_juggernaut() const
{
	return m_next_juggernaut;
}

void c_game_engine_juggernaut_variant::set_next_juggernaut(e_juggernaut_next_juggernaut_settings next_juggernaut)
{
	if (!VALID_INDEX(next_juggernaut, k_juggernaut_next_juggernaut_settings))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid next_juggernaut setting '%d' outside range '[0, %d)'!", next_juggernaut, k_juggernaut_next_juggernaut_settings);

		m_zone_movement = k_juggernaut_next_juggernaut_settings_default;
	}
	else
	{
		m_zone_movement = next_juggernaut;
	}
}

e_juggernaut_zone_movement_settings c_game_engine_juggernaut_variant::get_zone_movement() const
{
	return m_zone_movement;
}

long c_game_engine_juggernaut_variant::get_zone_movement_time_in_seconds() const
{
	//ASSERT(VALID_INDEX(m_zone_movement, k_number_of_juggernaut_zone_movement_settings));
	ASSERT(VALID_INDEX(get_zone_movement(), k_number_of_juggernaut_zone_movement_settings));

	return k_juggernaut_zone_movement_values[get_zone_movement()];
}

void c_game_engine_juggernaut_variant::set_zone_movement(e_juggernaut_zone_movement_settings zone_movement)
{
	if (!VALID_INDEX(zone_movement, k_number_of_juggernaut_zone_movement_settings))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid zone_movement setting '%d' outside range '[0, %d)'!", zone_movement, k_number_of_juggernaut_zone_movement_settings);

		m_zone_movement = k_juggernaut_zone_movement_settings_default;
	}
	else
	{
		m_zone_movement = zone_movement;
	}
}

e_juggernaut_zone_order_settings c_game_engine_juggernaut_variant::get_zone_order() const
{
	return m_zone_order;
}

void c_game_engine_juggernaut_variant::set_zone_order(e_juggernaut_zone_order_settings zone_order)
{
	if (!VALID_INDEX(zone_order, k_juggernaut_zone_order_settings))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid zone_order setting '%d' outside range '[0, %d)'!", zone_order, k_juggernaut_zone_order_settings);

		m_zone_order = k_juggernaut_zone_order_settings_default;
	}
	else
	{
		m_zone_order = zone_order;
	}
}

char c_game_engine_juggernaut_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_juggernaut_variant::set_kill_points(char kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

char c_game_engine_juggernaut_variant::get_juggernaut_kill_points() const
{
	return m_juggernaut_kill_points;
}

void c_game_engine_juggernaut_variant::set_juggernaut_kill_points(char juggernaut_kill_points)
{
	if (!VALID_INDEX(juggernaut_kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid juggernaut_kill_points setting '%d' outside range '[%d, %d]'!", juggernaut_kill_points, -10, 10);

		m_juggernaut_kill_points = 1;
	}
	else
	{
		m_juggernaut_kill_points = juggernaut_kill_points;
	}
}

char c_game_engine_juggernaut_variant::get_kill_as_juggernaut_points() const
{
	return m_kill_as_juggernaut_points;
}

void c_game_engine_juggernaut_variant::set_kill_as_juggernaut_points(char kill_as_juggernaut_points)
{
	if (!VALID_INDEX(kill_as_juggernaut_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid kill_as_juggernaut_points setting '%d' outside range '[%d, %d]'!", kill_as_juggernaut_points, -10, 10);

		m_kill_as_juggernaut_points = 1;
	}
	else
	{
		m_kill_as_juggernaut_points = kill_as_juggernaut_points;
	}
}

char c_game_engine_juggernaut_variant::get_destination_arrival_points() const
{
	return m_destination_arrival_points;
}

void c_game_engine_juggernaut_variant::set_destination_arrival_points(char destination_arrival_points)
{
	if (!VALID_INDEX(destination_arrival_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid destination_arrival_points setting '%d' outside range '[%d, %d]'!", destination_arrival_points, -10, 10);

		m_destination_arrival_points = 1;
	}
	else
	{
		m_destination_arrival_points = destination_arrival_points;
	}
}

char c_game_engine_juggernaut_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_juggernaut_variant::set_suicide_points(char suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = 1;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

char c_game_engine_juggernaut_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_juggernaut_variant::set_betrayal_points(char betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = 1;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

byte c_game_engine_juggernaut_variant::get_juggernaut_delay() const
{
	return m_juggernaut_delay;
}

void c_game_engine_juggernaut_variant::set_juggernaut_delay(byte juggernaut_delay)
{
	if (!VALID_INDEX(juggernaut_delay, 10))
	{
		//generate_event(_event_level_warning, "game_engine:juggernaut: invalid juggernaut_delay setting '%d' outside range '[%d, %d]'!", juggernaut_delay, 0, 10);

		m_juggernaut_delay = 0;
	}
	else
	{
		m_juggernaut_delay = juggernaut_delay;
	}
}

c_player_traits* c_game_engine_juggernaut_variant::get_juggernaut_traits_writeable()
{
	return &m_juggernaut_traits;
}

c_player_traits const* c_game_engine_juggernaut_variant::get_juggernaut_traits() const
{
	return &m_juggernaut_traits;
}

void c_game_engine_juggernaut_variant::set_juggernaut_traits(c_player_traits const* traits, bool force)
{
	m_juggernaut_traits.set(traits, force);
}