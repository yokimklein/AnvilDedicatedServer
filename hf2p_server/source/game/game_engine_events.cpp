#include "game_engine_events.h"

s_game_engine_event_response_definition* __fastcall audience_member_find_response(datum_index player_index, s_game_engine_event_data const* event_data)
{
	assert(player_index != NONE);
	assert(event_data);
	return INVOKE(0xD0E40, audience_member_find_response, player_index, event_data);
}