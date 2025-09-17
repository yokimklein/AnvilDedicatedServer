#include "game_engine_simulation.h"
#include <memory\tls.h>
#include <game\game_engine_util.h>
#include <game\game_engine_event_definitions.h>
#include <game\game_engine_events.h>
#include <game\game_engine.h>

// do these belong in game_globals.cpp?
void game_engine_globals_set_statborg_gamestate_index(datum_index index)
{
	ASSERT(current_game_engine());
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);

	game_engine_globals->statborg_gamestate_index = index;
}

datum_index game_engine_globals_get_statborg_gamestate_index()
{
	if (current_game_engine())
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		return game_engine_globals->statborg_gamestate_index;
	}
	return NONE;
}

void game_engine_globals_set_gamestate_index(datum_index index)
{
	ASSERT(current_game_engine());
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);

	game_engine_globals->game_engine_gamestate_index = index;
}

datum_index game_engine_globals_get_gamestate_index()
{
	if (current_game_engine())
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		return game_engine_globals->game_engine_gamestate_index;
	}
	return NONE;
}

e_simulation_entity_type game_engine_globals_get_simulation_entity_type()
{
	c_game_engine* game_engine = current_game_engine();
	if (game_engine)
		return game_engine->get_simulation_entity_type();
	return k_simulation_entity_type_none;
}

datum_index game_engine_globals_get_player_gamestate_index(short absolute_player_index)
{
	ASSERT(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	if (current_game_engine())
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
		return game_engine_globals->player_gamestate_indices[absolute_player_index];
	}
	return NONE;
}

void game_engine_globals_set_player_gamestate_index(short absolute_player_index, datum_index index)
{
	ASSERT(current_game_engine());
	ASSERT(absolute_player_index >= 0 && absolute_player_index < k_maximum_multiplayer_players);
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	game_engine_globals->player_gamestate_indices[absolute_player_index] = index;
}

void game_engine_event_build_player_response_list(s_game_engine_event_data const* game_engine_event, datum_index* player_response_list, long player_response_list_max_count, long* player_response_list_count_out)
{
	ASSERT(player_response_list);
	ASSERT(player_response_list_max_count == k_maximum_multiplayer_players);
	ASSERT(player_response_list_count_out);
	csmemset(player_response_list, NONE, sizeof(*player_response_list) * player_response_list_max_count);
	*player_response_list_count_out = 0;
	long audience_player_index = game_engine_event->audience_player_index;
	if (audience_player_index == NONE)
	{
		c_player_in_game_iterator player_iterator;
		player_iterator.begin();
		long i = 0;
		while (player_iterator.next())
		{
			if (i >= player_response_list_max_count) break;

			datum_index player_index = player_iterator.get_index();
			if (audience_member_find_response(player_index, game_engine_event))
			{
				player_response_list[i++] = player_index;
			}
		}
		*player_response_list_count_out = i;
	}
	else if (audience_member_find_response(game_engine_event->audience_player_index, game_engine_event))
	{
		player_response_list[0] = game_engine_event->audience_player_index;
		*player_response_list_count_out = 1;
	}
}