#include "game_engine_event_definitions.h"

void game_engine_initialize_event(e_multiplayer_event_type event_type, string_id type, s_game_engine_event_data* event_data)
{
	event_data->event_type = event_type;
	event_data->type = type;
	event_data->identifier = -1;
	event_data->audience_player_index = -1;
	event_data->cause_player_index = -1;
	event_data->cause_team_index = -1;
	event_data->effect_player_index = -1;
	event_data->effect_team_index = -1;
	event_data->event_quantity = 0;
	event_data->dummy = -1;
}