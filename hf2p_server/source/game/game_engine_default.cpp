#include "game_engine_default.h"
#include <game\game_engine.h>

c_game_engine_base_variant* c_game_engine_base_variant::constructor()
{
	*(size_t*)this = BASE_ADDRESS(0xD71410); // set vftable
	return this;
};

void c_game_engine_base_variant::set(c_game_engine_base_variant const* variant, bool force)
{
	assert(variant != NULL);

	set_name(variant->get_name());
	set_description(variant->get_description());
	m_miscellaneous_options.set(variant->get_miscellaneous_options(), force);
	m_respawn_options.set(variant->get_respawn_options(), force);
	m_social_options.set(variant->get_social_options(), force);
	m_map_override_options.set(variant->get_map_override_options(), force);
	set_team_scoring_method(variant->get_team_scoring_method());
}

void c_game_engine_base_variant::set(s_game_engine_base_variant_definition const* definition, e_game_engine_type engine_index)
{
	DECLFUNC(0x1705C0, void, __thiscall, c_game_engine_base_variant*, s_game_engine_base_variant_definition const*, e_game_engine_type)(this, definition, engine_index);
}

void c_game_engine_base_variant::get_game_engine_name(c_static_wchar_string<1024>* out_game_engine_name) const
{
	game_engine_get_multiplayer_string(get_game_engine_name_string_id(), out_game_engine_name);
}

void c_game_engine_base_variant::get_game_engine_description(c_static_wchar_string<1024>* game_engine_description) const
{
	game_engine_get_multiplayer_string(get_game_engine_default_description_string_id(), game_engine_description);
}

char const* c_game_engine_base_variant::get_name() const
{
	return m_name;
}

void c_game_engine_base_variant::set_name(char const* name)
{
	csstrnzcpy(m_name, name, 32);
}

char const* c_game_engine_base_variant::get_description() const
{
	return m_metadata.description;
}

void c_game_engine_base_variant::set_description(char const* description)
{
	csstrnzcpy(m_metadata.description, description, 128);
}

c_game_engine_miscellaneous_options* c_game_engine_base_variant::get_miscellaneous_options_writeable()
{
	return &m_miscellaneous_options;
}

c_game_engine_miscellaneous_options const* c_game_engine_base_variant::get_miscellaneous_options() const
{
	return &m_miscellaneous_options;
}

c_game_engine_respawn_options* c_game_engine_base_variant::get_respawn_options_writeable()
{
	return &m_respawn_options;
}

c_game_engine_respawn_options const* c_game_engine_base_variant::get_respawn_options() const
{
	return &m_respawn_options;
}

c_game_engine_social_options* c_game_engine_base_variant::get_social_options_writeable()
{
	return &m_social_options;
}

c_game_engine_social_options const* c_game_engine_base_variant::get_social_options() const
{
	return &m_social_options;
}

c_game_engine_map_override_options* c_game_engine_base_variant::get_map_override_options_writeable()
{
	return &m_map_override_options;
}

c_game_engine_map_override_options const* c_game_engine_base_variant::get_map_override_options() const
{
	return &m_map_override_options;
}

bool c_game_engine_base_variant::get_built_in() const
{
	return m_flags.test(_base_variant_flags_built_in);
}

void c_game_engine_base_variant::set_built_in(bool built_in)
{
	m_flags.set(_base_variant_flags_built_in, built_in);
}

short c_game_engine_base_variant::get_team_scoring_method() const
{
	return m_team_scoring_method;
}

void c_game_engine_base_variant::set_team_scoring_method(short team_scoring_method)
{
	m_team_scoring_method = team_scoring_method;
}