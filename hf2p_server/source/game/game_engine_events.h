#pragma once
#include <memory/data.h>
#include <game\game_engine_event_definitions.h>

struct s_game_engine_event_response_definition
{

};

s_game_engine_event_response_definition* __fastcall audience_member_find_response(datum_index player_index, s_game_engine_event_data const* event_data);