#include "game_engine_slayer.h"

c_game_engine_slayer_variant* c_game_engine_slayer_variant::constructor()
{
	*(size_t*)this = BASE_ADDRESS(0xD71500); // set vftable
	return this;
}

void c_game_engine_slayer_variant::set(c_game_engine_slayer_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_score_to_win(variant->get_score_to_win());
	set_kill_points(variant->get_kill_points());
	set_assist_points(variant->get_assist_points());
	set_death_points(variant->get_death_points());
	set_suicide_points(variant->get_suicide_points());
	set_betrayal_points(variant->get_betrayal_points());
	set_leader_killed_points(variant->get_leader_killed_points());
	set_elimination_points(variant->get_elimination_points());
	set_assassination_points(variant->get_assassination_points());
	set_headshot_points(variant->get_headshot_points());
	set_melee_points(variant->get_melee_points());
	set_sticky_points(variant->get_sticky_points());
	set_splatter_points(variant->get_splatter_points());
	set_killing_spree_points(variant->get_killing_spree_points());
	set_leader_traits(get_leader_traits(), force);
}

void c_game_engine_slayer_variant::set(s_game_engine_slayer_variant_definition const* definition)
{
	DECLFUNC(0x1AC9F0, void, __thiscall, c_game_engine_slayer_variant*, s_game_engine_slayer_variant_definition const*)(this, definition);
}

short c_game_engine_slayer_variant::get_score_to_win() const
{
	return m_score_to_win;
}

void c_game_engine_slayer_variant::set_score_to_win(short score_to_win)
{
	if (!VALID_INDEX(score_to_win + 1, 501))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid score_to_win setting '%d' outside range '[%d, %d]'!", score_to_win, -1, 520);

		m_score_to_win = 25;
	}
	else
	{
		m_score_to_win = score_to_win;
	}
}

char c_game_engine_slayer_variant::get_kill_points() const
{
	return m_kill_points;
}

void c_game_engine_slayer_variant::set_kill_points(char kill_points)
{
	if (!VALID_INDEX(kill_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid kill_points setting '%d' outside range '[%d, %d]'!", kill_points, -10, 10);

		m_kill_points = 1;
	}
	else
	{
		m_kill_points = kill_points;
	}
}

char c_game_engine_slayer_variant::get_assist_points() const
{
	return m_assist_points;
}

void c_game_engine_slayer_variant::set_assist_points(char assist_points)
{
	if (!VALID_INDEX(assist_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid assist_points setting '%d' outside range '[%d, %d]'!", assist_points, -10, 10);

		m_assist_points = 0;
	}
	else
	{
		m_assist_points = assist_points;
	}
}

char c_game_engine_slayer_variant::get_death_points() const
{
	return m_death_points;
}

void c_game_engine_slayer_variant::set_death_points(char death_points)
{
	if (!VALID_INDEX(death_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid death_points setting '%d' outside range '[%d, %d]'!", death_points, -10, 10);

		m_death_points = 0;
	}
	else
	{
		m_death_points = death_points;
	}
}

char c_game_engine_slayer_variant::get_suicide_points() const
{
	return m_suicide_points;
}

void c_game_engine_slayer_variant::set_suicide_points(char suicide_points)
{
	if (!VALID_INDEX(suicide_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid suicide_points setting '%d' outside range '[%d, %d]'!", suicide_points, -10, 10);

		m_suicide_points = -1;
	}
	else
	{
		m_suicide_points = suicide_points;
	}
}

char c_game_engine_slayer_variant::get_betrayal_points() const
{
	return m_betrayal_points;
}

void c_game_engine_slayer_variant::set_betrayal_points(char betrayal_points)
{
	if (!VALID_INDEX(betrayal_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid betrayal_points setting '%d' outside range '[%d, %d]'!", betrayal_points, -10, 10);

		m_betrayal_points = -1;
	}
	else
	{
		m_betrayal_points = betrayal_points;
	}
}

char c_game_engine_slayer_variant::get_leader_killed_points() const
{
	return m_leader_killed_points;
}

void c_game_engine_slayer_variant::set_leader_killed_points(char leader_killed_points)
{
	if (!VALID_INDEX(leader_killed_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid leader_killed_points setting '%d' outside range '[%d, %d]'!", leader_killed_points, -10, 10);

		m_leader_killed_points = 0;
	}
	else
	{
		m_leader_killed_points = leader_killed_points;
	}
}

char c_game_engine_slayer_variant::get_elimination_points() const
{
	return m_elimination_points;
}

void c_game_engine_slayer_variant::set_elimination_points(char elimination_points)
{
	if (!VALID_INDEX(elimination_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid elimination_points setting '%d' outside range '[%d, %d]'!", elimination_points, -10, 10);

		m_elimination_points = 0;
	}
	else
	{
		m_elimination_points = elimination_points;
	}
}

char c_game_engine_slayer_variant::get_assassination_points() const
{
	return m_assassination_points;
}

void c_game_engine_slayer_variant::set_assassination_points(char assassination_points)
{
	if (!VALID_INDEX(assassination_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid assassination_points setting '%d' outside range '[%d, %d]'!", assassination_points, -10, 10);

		m_assassination_points = 0;
	}
	else
	{
		m_assassination_points = assassination_points;
	}
}

char c_game_engine_slayer_variant::get_headshot_points() const
{
	return m_headshot_points;
}

void c_game_engine_slayer_variant::set_headshot_points(char headshot_points)
{
	if (!VALID_INDEX(headshot_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid headshot_points setting '%d' outside range '[%d, %d]'!", headshot_points, -10, 10);

		m_headshot_points = 0;
	}
	else
	{
		m_headshot_points = headshot_points;
	}
}

char c_game_engine_slayer_variant::get_melee_points() const
{
	return m_melee_points;
}

void c_game_engine_slayer_variant::set_melee_points(char melee_points)
{
	if (!VALID_INDEX(melee_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid melee_points setting '%d' outside range '[%d, %d]'!", melee_points, -10, 10);

		m_melee_points = 0;
	}
	else
	{
		m_melee_points = melee_points;
	}
}

char c_game_engine_slayer_variant::get_sticky_points() const
{
	return m_sticky_points;
}

void c_game_engine_slayer_variant::set_sticky_points(char sticky_points)
{
	if (!VALID_INDEX(sticky_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid sticky_points setting '%d' outside range '[%d, %d]'!", sticky_points, -10, 10);

		m_sticky_points = 0;
	}
	else
	{
		m_sticky_points = sticky_points;
	}
}

char c_game_engine_slayer_variant::get_splatter_points() const
{
	return m_splatter_points;
}

void c_game_engine_slayer_variant::set_splatter_points(char splatter_points)
{
	if (!VALID_INDEX(splatter_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid splatter_points setting '%d' outside range '[%d, %d]'!", splatter_points, -10, 10);

		m_splatter_points = 0;
	}
	else
	{
		m_splatter_points = splatter_points;
	}
}

char c_game_engine_slayer_variant::get_killing_spree_points() const
{
	return m_killing_spree_points;
}

void c_game_engine_slayer_variant::set_killing_spree_points(char killing_spree_points)
{
	if (!VALID_INDEX(killing_spree_points + 10, 20))
	{
		//generate_event(_event_level_warning, "game_engine:slayer: invalid killing_spree_points setting '%d' outside range '[%d, %d]'!", killing_spree_points, -10, 10);

		m_killing_spree_points = 0;
	}
	else
	{
		m_killing_spree_points = killing_spree_points;
	}
}

c_player_traits* c_game_engine_slayer_variant::get_leader_traits_writeable()
{
	return &m_leader_traits;
}

c_player_traits const* c_game_engine_slayer_variant::get_leader_traits() const
{
	return &m_leader_traits;
}

void c_game_engine_slayer_variant::set_leader_traits(c_player_traits const* traits, bool force)
{
	m_leader_traits.set(traits, force);
}