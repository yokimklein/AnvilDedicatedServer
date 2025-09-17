#include "game_engine_spawning.h"
#include <game\game_time.h>
#include <game\game_engine_util.h>
#include <game\players.h>
#include <memory\tls.h>
#include <game\game.h>
#include <game\game_engine.h>

long game_engine_get_pre_round_ticks()
{
	return game_seconds_integer_to_ticks(k_pre_round_seconds);
}

bool __fastcall game_engine_should_spawn_player(datum_index player_index)
{
	if (!current_game_engine())
		return true;
	if (!game_engine_player_is_playing(player_index) || !game_engine_in_round())
		return false;

	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(players, player_index);
	if ((game_is_survival() || game_is_multiplayer()) && ((game_engine_round_time_get() + player->respawn_timer_countdown_ticks) < game_engine_get_pre_round_ticks()))
	{
		game_engine_player_set_spawn_timer(player_index, game_engine_get_pre_round_ticks() - game_engine_round_time_get());
		return false;
	}
	else
	{
		return player->respawn_forced || player->respawn_timer_countdown_ticks <= 0;
	}
}