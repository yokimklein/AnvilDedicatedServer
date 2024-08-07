#include "game_engine_util.h"
#include <memory\tls.h>
#include <game\game.h>
#include <game\game_engine.h>
#include "assert.h"
#include <tag_files\string_ids.h>
#include <scenario\scenario.h>
#include <cache\cache_files.h>
#include <game\game_globals.h>
#include <game\multiplayer_definitions.h>
#include <tag_files\tag_groups.h>

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

short __fastcall game_engine_get_multiplayer_weapon_selection_absolute_index(long name)
{
	return INVOKE(0x11C250, game_engine_get_multiplayer_weapon_selection_absolute_index, name);
}

string_id __fastcall game_engine_get_multiplayer_weapon_selection_name(short absolute_index)
{
	switch (absolute_index)
	{
		case -3:
			return STRING_ID(game_engine, random);
		case -1:
			return STRING_ID(global, default);
		case -2:
			return STRING_ID(gui, unchanged);
	}

	return multiplayer_universal_data_get_weapon_selection_name_from_absolute_index(absolute_index);
}