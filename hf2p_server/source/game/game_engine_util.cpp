#include "game_engine_util.h"
#include "..\memory\tls.h"
#include "game.h"
#include "assert.h"
#include "..\dllmain.h"

// TODO
bool game_engine_is_sandbox()
{
	return false;
}

c_game_engine* current_game_engine()
{
	auto tls = get_tls();
	auto game_engine_globals = tls->game_engine_globals;
	if (game_engine_globals->game_engine_index < _game_engine_base_variant || game_engine_globals->game_engine_index >= k_game_engine_type_count)
		return nullptr;
	else
		return game_engines[game_engine_globals->game_engine_index.get()];
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

void game_engine_send_event(s_game_engine_event_data* event_data)
{
	FUNCTION_DEF(0x11C0C0, void, __fastcall, game_engine_send_event_call, s_game_engine_event_data* event_data);
	game_engine_send_event_call(event_data);
}