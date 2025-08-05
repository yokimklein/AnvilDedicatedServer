#include "game_engine_territories.h"

c_game_engine_territories_variant* c_game_engine_territories_variant::constructor()
{
	*(size_t*)this = BASE_ADDRESS(0xD71380); // set vftable
	return this;
}

void c_game_engine_territories_variant::set(c_game_engine_territories_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_one_sided(variant->get_one_sided());
	set_lock_after_first_capture(variant->get_lock_after_first_capture());
	set_respawn_on_capture(variant->get_respawn_on_capture());
	set_capture_time(variant->get_capture_time());
	set_sudden_death_time(variant->get_sudden_death_time());
	set_defender_traits(variant->get_defender_traits(), force);
	set_attacker_traits(variant->get_attacker_traits(), force);
}

void c_game_engine_territories_variant::set(s_game_engine_territories_variant_definition const* definition)
{
	DECLFUNC(0x1AB3C0, void, __thiscall, c_game_engine_territories_variant*, s_game_engine_territories_variant_definition const*)(this, definition);
}

bool c_game_engine_territories_variant::get_one_sided() const
{
	return m_variant_flags.test(_territories_variant_flags_one_sided);
}

void c_game_engine_territories_variant::set_one_sided(bool one_sided)
{
	return m_variant_flags.set(_territories_variant_flags_one_sided, one_sided);
}

bool c_game_engine_territories_variant::get_lock_after_first_capture() const
{
	return m_variant_flags.test(_territories_variant_flags_lock_after_first_capture);
}

void c_game_engine_territories_variant::set_lock_after_first_capture(bool lock_after_first_capture)
{
	return m_variant_flags.set(_territories_variant_flags_lock_after_first_capture, lock_after_first_capture);
}

e_territories_respawn_on_capture_settings c_game_engine_territories_variant::get_respawn_on_capture() const
{
	return m_respawn_on_capture;
}

void c_game_engine_territories_variant::set_respawn_on_capture(e_territories_respawn_on_capture_settings respawn_on_capture)
{
	if (!VALID_INDEX(respawn_on_capture, k_territories_respawn_on_capture_settings))
	{
		//generate_event(_event_level_warning, "game_engine:territories: invalid respawn_on_capture setting '%d' outside range '[0, %d)'!", respawn_on_capture, k_territories_respawn_on_capture_settings);

		m_respawn_on_capture = k_territories_respawn_on_capture_settings_default;
	}
	else
	{
		m_respawn_on_capture = respawn_on_capture;
	}
}

short c_game_engine_territories_variant::get_capture_time() const
{
	return m_capture_time;
}

void c_game_engine_territories_variant::set_capture_time(short capture_time)
{
	if (!VALID_INDEX(capture_time, k_territories_capture_time_settings))
	{
		//generate_event(_event_level_warning, "game_engine:territories: invalid capture_time setting '%d' outside range '[%d, %d]'!", capture_time, _territories_capture_time_settings_instant, _territories_capture_time_settings_2_minutes);

		m_capture_time = k_territories_capture_time_settings_default;
	}
	else
	{
		m_capture_time = capture_time;
	}
}

short c_game_engine_territories_variant::get_sudden_death_time() const
{
	return m_sudden_death_time;
}

bool c_game_engine_territories_variant::get_sudden_death_enabled() const
{
	return m_sudden_death_time.get() != _territories_sudden_death_settings_off;
}

void c_game_engine_territories_variant::set_sudden_death_time(short sudden_death_time)
{
	if (!VALID_INDEX(sudden_death_time + 1, k_territories_sudden_death_settings))
	{
		//generate_event(_event_level_warning, "game_engine:territories: invalid sudden_death_time setting '%d' outside range '[%d, %d]'!", sudden_death_time, _territories_sudden_death_settings_indefinite, _territories_sudden_death_settings_5_minutes);

		m_sudden_death_time = k_territories_sudden_death_settings_default;
	}
	else
	{
		m_sudden_death_time = sudden_death_time;
	}
}

c_player_traits* c_game_engine_territories_variant::get_defender_traits_writeable()
{
	return &m_defender_traits;
}

c_player_traits const* c_game_engine_territories_variant::get_defender_traits() const
{
	return &m_defender_traits;
}

void c_game_engine_territories_variant::set_defender_traits(c_player_traits const* traits, bool force)
{
	m_defender_traits.set(traits, force);
}

c_player_traits* c_game_engine_territories_variant::get_attacker_traits_writeable()
{
	return &m_attacker_traits;
}

c_player_traits const* c_game_engine_territories_variant::get_attacker_traits() const
{
	return &m_attacker_traits;
}

void c_game_engine_territories_variant::set_attacker_traits(c_player_traits const* traits, bool force)
{
	m_attacker_traits.set(traits, force);
}