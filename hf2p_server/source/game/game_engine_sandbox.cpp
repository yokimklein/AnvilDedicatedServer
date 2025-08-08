#include "game_engine_sandbox.h"

c_game_engine_sandbox_variant* c_game_engine_sandbox_variant::constructor()
{
	*(size_t*)this = base_address(0xD71350); // set vftable
	return this;
}

void c_game_engine_sandbox_variant::set(c_game_engine_sandbox_variant const* variant, bool force)
{
	ASSERT(variant != NULL);

	c_game_engine_base_variant::set(variant, force);

	set_edit_mode(variant->get_edit_mode());
	set_respawn_time(variant->get_respawn_time());
	set_open_channel_voice(variant->get_open_channel_voice());
	set_player_traits(variant->get_player_traits(), force);
}

void c_game_engine_sandbox_variant::set(s_game_engine_sandbox_variant_definition const* definition)
{
	DECLFUNC(0x1ABB90, void, __thiscall, c_game_engine_sandbox_variant*, s_game_engine_sandbox_variant_definition const*)(this, definition);
}

bool c_game_engine_sandbox_variant::get_open_channel_voice() const
{
	return m_variant_flags.test(_sandbox_variant_flags_open_channel_voice);
}

void c_game_engine_sandbox_variant::set_open_channel_voice(bool open_channel_voice)
{
	m_variant_flags.set(_sandbox_variant_flags_open_channel_voice, open_channel_voice);
}

e_sandbox_edit_mode_settings c_game_engine_sandbox_variant::get_edit_mode() const
{
	return m_edit_mode;
}

void c_game_engine_sandbox_variant::set_edit_mode(e_sandbox_edit_mode_settings edit_mode)
{
	if (!VALID_INDEX(edit_mode, k_sandbox_edit_mode_settings))
	{
		//generate_event(_event_level_warning, "game_engine:sandbox: invalid edit_mode setting '%d' outside range '[0, %d)'!", edit_mode, k_sandbox_edit_mode_settings);

		m_edit_mode = k_sandbox_edit_mode_settings_default;
	}
	else
	{
		m_edit_mode = edit_mode;
	}
}

short c_game_engine_sandbox_variant::get_respawn_time() const
{
	return m_respawn_time;
}

void c_game_engine_sandbox_variant::set_respawn_time(short respawn_time)
{
	if (!VALID_INDEX(respawn_time, k_sandbox_respawn_times))
	{
		//generate_event(_event_level_warning, "game_engine:sandbox: invalid respawn_time setting '%d' outside range '[%d, %d]'!", respawn_time, _sandbox_respawn_time_instant, _sandbox_respawn_time_60_seconds);

		m_respawn_time = k_sandbox_respawn_times_default;
	}
	else
	{
		m_respawn_time = respawn_time;
	}
}

c_player_traits* c_game_engine_sandbox_variant::get_player_traits_writeable()
{
	return &m_player_traits;
}

c_player_traits const* c_game_engine_sandbox_variant::get_player_traits() const
{
	return &m_player_traits;
}

void c_game_engine_sandbox_variant::set_player_traits(c_player_traits const* traits, bool force)
{
	m_player_traits.set(traits, force);
}