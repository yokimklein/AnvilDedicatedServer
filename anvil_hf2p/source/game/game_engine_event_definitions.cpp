#include "game_engine_event_definitions.h"

void game_engine_initialize_event(e_multiplayer_event_type event_type, string_id event_name, s_game_engine_event_data* event_data)
{
	event_data->event_type = event_type;
	event_data->event_name = event_name;
	event_data->event_identifier = NONE;
	event_data->audience_player_index = NONE;
	event_data->cause_player_index = NONE;
	event_data->cause_team_index = NONE;
	event_data->effect_player_index = NONE;
	event_data->effect_team_index = NONE;
	event_data->event_quantity = 0;
	event_data->territories_event_data.territory_index = NONE;
	event_data->objective_game_role_event_data.objective_game_role_index = NONE;
}