#include "game_engine_event_definitions.h"

void game_engine_initialize_event(e_game_engine_event_type event_type, string_id event_name, s_game_engine_event_data* event_data)
{
	event_data->type = event_type;
	event_data->name = event_name;
	event_data->index = -1;
	event_data->target_player_index = -1;
	event_data->capuse_player_index = -1;
	event_data->cause_team_index = -1;
	event_data->effect_player_index = -1;
	event_data->effect_team_index = -1;
	event_data->points = 0;
	event_data->unknown = -1;
}