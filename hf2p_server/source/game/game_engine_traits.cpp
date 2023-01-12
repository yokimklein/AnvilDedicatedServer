#include "game_engine_traits.h"
#include "..\dllmain.h"

bool c_game_engine_miscellaneous_options::get_teams_enabled()
{
	return this->m_flags.test(_game_engine_miscellaneous_option_teams_enabled);
}


byte c_game_engine_respawn_options::get_lives_per_round()
{
	return this->m_lives_per_round;
}

void game_engine_assemble_player_traits(datum_index absolute_player_index)
{
	FUNCTION_DEF(0x11D7E0, void, __fastcall, game_engine_assemble_player_traits_call, datum_index absolute_player_index);
	game_engine_assemble_player_traits_call(absolute_player_index);
}