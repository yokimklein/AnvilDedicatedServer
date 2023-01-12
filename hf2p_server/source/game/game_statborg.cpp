#include "game_statborg.h"
#include "game_engine_util.h"
#include "..\memory\tls.h"
#include "game.h"
#include "assert.h"
#include "..\simulation\game_interface\simulation_game_action.h"

c_game_statborg* game_engine_get_statborg()
{
	if (current_game_engine())
		return &get_tls()->game_engine_globals->statborg;
	// TODO finish for survival
	// if (campaign_survival_enabled)
	//	return &get_tls()->survival_mode_globals->statborg;
	return nullptr;
}

void c_game_statborg::player_added(datum_index player_index)
{
	if (!game_is_predicted())
		this->clear_player_stats(player_index);
}

void c_game_statborg::clear_player_stats(datum_index player_index)
{
	short absolute_player_index = (word)player_index;
	assert(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	csmemset(&this->players_statistics[absolute_player_index], 0, 0x34);
	c_flags<long, ulong64, 64> update_flags = {};
	update_flags.set(absolute_player_index, true);
	simulation_action_game_statborg_update(&update_flags);
}