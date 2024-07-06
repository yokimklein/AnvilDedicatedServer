#include "game_engine_util.h"
#include <memory\tls.h>
#include <game\game.h>
#include <game\game_engine.h>
#include "assert.h"

// TODO
bool game_engine_is_sandbox()
{
	return false;
}

c_game_engine* current_game_engine()
{
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	if (VALID_INDEX(game_engine_globals->game_engine_index, k_game_engine_type_count))
		return game_engines[game_engine_globals->game_engine_index.get()];

	return nullptr;
}

bool game_engine_running()
{
	return current_game_engine() != nullptr;
}

c_game_variant* current_game_variant()
{
	assert(game_options_get() != NULL);
	return &game_options_get()->game_variant;
}

void __fastcall game_engine_send_event(s_game_engine_event_data* event_data)
{
	INVOKE(0x11C0C0, game_engine_send_event, event_data);
}