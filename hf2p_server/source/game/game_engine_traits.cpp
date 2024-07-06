#include "game_engine_traits.h"

bool c_game_engine_miscellaneous_options::get_teams_enabled()
{
	return this->m_flags.test(_game_engine_miscellaneous_option_teams_enabled);
}


byte c_game_engine_respawn_options::get_lives_per_round()
{
	return this->m_lives_per_round;
}

void __fastcall game_engine_assemble_player_traits(datum_index absolute_player_index)
{
	INVOKE(0x11D7E0, game_engine_assemble_player_traits, absolute_player_index);
}

bool c_game_engine_social_options::get_observers_enabled()
{
	// Observers enabled flag nuked in ODST
	return false;
	//return m_flags.test(_game_engine_social_options_observers_enabled);
}