#include "game_engine_vip.h"

c_game_engine_vip_variant* c_game_engine_vip_variant::constructor()
{
	*(size_t*)this = BASE_ADDRESS(0xD713E0); // set vftable
	return this;
}

void c_game_engine_vip_variant::set(c_game_engine_vip_variant const* variant, bool force)
{
	assert(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_single_vip(variant->get_single_vip());
	set_destination_zones_enabled(variant->get_destination_zones_enabled());
	set_end_round_on_vip_death(variant->get_should_end_round_on_vip_death());

	set_score_to_win_round(static_cast<short>(variant->get_score_to_win_round()));
	set_kill_points(variant->get_kill_points());
	set_takedown_points(variant->get_takedown_points());
	set_kill_as_vip_points(variant->get_kill_as_vip_points());
	set_vip_death_points(variant->get_vip_death_points());
	set_destination_arrival_points(variant->get_destination_arrival_points());
	set_suicide_points(variant->get_suicide_points());
	set_vip_suicide_points(variant->get_vip_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_vip_selection(variant->get_vip_selection());
	set_zone_movement(variant->get_zone_movement());
	set_zone_order(variant->get_zone_order());
	set_influence_radius(variant->get_influence_radius());

	zero_array(m_pad1);

	set_vip_team_traits(get_vip_team_traits(), force);
	set_vip_influence_traits(get_vip_influence_traits(), force);
	set_vip_traits(get_vip_traits(), force);
}

void c_game_engine_vip_variant::set(s_game_engine_vip_variant_definition const* definition)
{
	DECLFUNC(0x1A9020, void, __thiscall, c_game_engine_vip_variant*, s_game_engine_vip_variant_definition const*)(this, definition);
}

bool c_game_engine_vip_variant::get_single_vip() const
{
	return m_variant_flags.test(_vip_variant_flags_single_vip);
}

void c_game_engine_vip_variant::set_single_vip(bool single_vip)
{
	m_variant_flags.set(_vip_variant_flags_single_vip, single_vip);
}

bool c_game_engine_vip_variant::get_destination_zones_enabled() const
{
	return m_variant_flags.test(_vip_variant_flags_destination_zones_enabled);
}

void c_game_engine_vip_variant::set_destination_zones_enabled(bool destination_zones_enabled)
{
	m_variant_flags.set(_vip_variant_flags_destination_zones_enabled, destination_zones_enabled);
}

bool c_game_engine_vip_variant::get_should_end_round_on_vip_death() const
{
	return m_variant_flags.test(_vip_variant_flags_end_round_on_vip_death);
}

void c_game_engine_vip_variant::set_end_round_on_vip_death(bool end_round_on_vip_death)
{
	m_variant_flags.set(_vip_variant_flags_end_round_on_vip_death, end_round_on_vip_death);
}

void c_game_engine_vip_variant::set_score_to_win_round(short score_to_win_round)
{
	if (!VALID_INDEX(score_to_win_round, 500))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid score_to_win_round setting '%d' outside range '[%d, %d]'!", score_to_win_round, 0, 500);

		m_score_to_win_round = 15;
	}
	else
	{
		m_score_to_win_round = score_to_win_round;
	}
}

char c_game_engine_vip_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_vip_variant::set_kill_points(char kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 0;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

char c_game_engine_vip_variant::get_takedown_points() const
{
	return m_takedown_points;
}

void c_game_engine_vip_variant::set_takedown_points(char takedown_points)
{
	if (!VALID_INDEX(takedown_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid takedown_points setting '%d' outside range '[%d, %d]'!", takedown_points, -10, 10);

		m_takedown_points = 0;
	}
	else
	{
		m_takedown_points = takedown_points;
	}
}

char c_game_engine_vip_variant::get_kill_as_vip_points() const
{
	return m_kill_as_vip_points;
}

void c_game_engine_vip_variant::set_kill_as_vip_points(char kill_as_vip_points)
{
	if (!VALID_INDEX(kill_as_vip_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid kill_as_vip_points setting '%d' outside range '[%d, %d]'!", kill_as_vip_points, -10, 10);

		m_kill_as_vip_points = 0;
	}
	else
	{
		m_kill_as_vip_points = kill_as_vip_points;
	}
}

char c_game_engine_vip_variant::get_vip_death_points() const
{
	return m_vip_death_points;
}

void c_game_engine_vip_variant::set_vip_death_points(char vip_death_points)
{
	if (!VALID_INDEX(vip_death_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid vip_death_points setting '%d' outside range '[%d, %d]'!", vip_death_points, -10, 10);

		m_vip_death_points = 0;
	}
	else
	{
		m_vip_death_points = vip_death_points;
	}
}

char c_game_engine_vip_variant::get_destination_arrival_points() const
{
	return m_destination_arrival_points;
}

void c_game_engine_vip_variant::set_destination_arrival_points(char destination_arrival_points)
{
	if (!VALID_INDEX(destination_arrival_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid destination_arrival_points setting '%d' outside range '[%d, %d]'!", destination_arrival_points, -10, 10);

		m_destination_arrival_points = 0;
	}
	else
	{
		m_destination_arrival_points = destination_arrival_points;
	}
}

char c_game_engine_vip_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_vip_variant::set_suicide_points(char suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = 0;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

char c_game_engine_vip_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_vip_variant::set_betrayal_points(char betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = 0;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

char c_game_engine_vip_variant::get_vip_suicide_points() const
{
	return m_vip_suicide_points;
}

void c_game_engine_vip_variant::set_vip_suicide_points(char vip_suicide_points)
{
	if (!VALID_INDEX(vip_suicide_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid vip_suicide_points setting '%d' outside range '[%d, %d]'!", vip_suicide_points, -10, 10);

		m_vip_suicide_points = 0;
	}
	else
	{
		m_vip_suicide_points = vip_suicide_points;
	}
}

e_vip_vip_selection_settings c_game_engine_vip_variant::get_vip_selection() const
{
	return m_vip_selection;
}

void c_game_engine_vip_variant::set_vip_selection(e_vip_vip_selection_settings vip_selection)
{
	if (!VALID_INDEX(vip_selection, k_vip_vip_selection_settings))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid vip_selection setting '%d' outside range '[0, %d)'!", vip_selection, k_vip_vip_selection_settings);

		m_vip_selection = k_vip_vip_selection_settings_default;
	}
	else
	{
		m_vip_selection = vip_selection;
	}
}

e_vip_zone_movement_settings c_game_engine_vip_variant::get_zone_movement() const
{
	return m_zone_movement;
}

long c_game_engine_vip_variant::get_zone_movement_time_in_seconds() const
{
	//assert(VALID_INDEX(m_zone_movement, k_number_of_vip_zone_movement_settings));
	assert(VALID_INDEX(get_zone_movement(), k_number_of_vip_zone_movement_settings));

	return k_vip_zone_movement_values[get_zone_movement()];
}

void c_game_engine_vip_variant::set_zone_movement(e_vip_zone_movement_settings zone_movement)
{
	if (!VALID_INDEX(zone_movement, k_number_of_vip_zone_movement_settings))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid zone_movement setting '%d' outside range '[0, %d)'!", zone_movement, k_number_of_vip_zone_movement_settings);

		m_zone_movement = k_vip_zone_movement_settings_default;
	}
	else
	{
		m_zone_movement = zone_movement;
	}
}

e_vip_zone_order_settings c_game_engine_vip_variant::get_zone_order() const
{
	return m_zone_order;
}

void c_game_engine_vip_variant::set_zone_order(e_vip_zone_order_settings zone_order)
{
	if (!VALID_INDEX(zone_order, k_vip_zone_order_settings))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid zone_order setting '%d' outside range '[0, %d)'!", zone_order, k_vip_zone_order_settings);

		m_zone_order = k_vip_zone_order_settings_default;
	}
	else
	{
		m_zone_order = zone_order;
	}
}

short c_game_engine_vip_variant::get_influence_radius() const
{
	return m_influence_radius;
}

void c_game_engine_vip_variant::set_influence_radius(short influence_radius)
{
	if (!VALID_INDEX(influence_radius, 50))
	{
		//generate_event(_event_level_warning, "game_engine:vip: invalid influence_radius setting '%d' outside range '[%d, %d]'!", influence_radius, 0, 50);

		m_influence_radius = 0;
	}
	else
	{
		m_influence_radius = influence_radius;
	}
}

c_player_traits* c_game_engine_vip_variant::get_vip_team_traits_writeable()
{
	return &m_vip_team_traits;
}

c_player_traits const* c_game_engine_vip_variant::get_vip_team_traits() const
{
	return &m_vip_team_traits;
}

void c_game_engine_vip_variant::set_vip_team_traits(c_player_traits const* traits, bool force)
{
	m_vip_team_traits.set(traits, force);
}

c_player_traits* c_game_engine_vip_variant::get_vip_influence_traits_writeable()
{
	return &m_vip_influence_traits;
}

c_player_traits const* c_game_engine_vip_variant::get_vip_influence_traits() const
{
	return &m_vip_influence_traits;
}

void c_game_engine_vip_variant::set_vip_influence_traits(c_player_traits const* traits, bool force)
{
	m_vip_influence_traits.set(traits, force);
}

c_player_traits* c_game_engine_vip_variant::get_vip_traits_writeable()
{
	return &m_vip_traits;
}

c_player_traits const* c_game_engine_vip_variant::get_vip_traits() const
{
	return &m_vip_traits;
}

void c_game_engine_vip_variant::set_vip_traits(c_player_traits const* traits, bool force)
{
	m_vip_traits.set(traits, force);
}