#include "scenario_map_variant.h"
#include <memory\tls.h>
#include <game\game_engine.h>
#include <game\game_engine_util.h>
#include <game\game.h>

c_map_variant::c_map_variant()
{
	DECLFUNC(0xAB2F0, void, __thiscall, c_map_variant*)(this);
}

void c_map_variant::create_default(e_map_id map_id)
{
	DECLFUNC(0xAB380, void, __thiscall, c_map_variant*, e_map_id)(this, map_id);
}

c_map_variant* game_engine_get_runtime_map_variant()
{
	ASSERT(game_engine_running());
	TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals);
	return &game_engine_globals->map_variant;
}

datum_index c_map_variant::get_chunk_gamestate_index(long chunk_index)
{
	ASSERT(VALID_INDEX(chunk_index, k_number_of_map_variant_simulation_entities));
	return simulation_gamestate_indices[chunk_index];
}

// $TODO:
// this method was stripped out of client builds
// only used by forge, there's no need to reimplement this yet
void c_map_variant::place_new_object(long map_variant_index, long unknown, bool do_not_force)
{
	if (game_is_server() && game_is_distributed() && game_engine_is_sandbox() && !game_is_playback())
	{
		long object_index = map_variant_index / 8;
		long gamestate_index = game_engine_get_runtime_map_variant()->simulation_gamestate_indices[map_variant_index / 8];
		if (gamestate_index != NONE)
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index != NONE)
			{
				c_simulation_object_update_flags update_mask;
				update_mask.set_unsafe(static_cast<ulong64>(unknown) << (4 * (map_variant_index % 8)));
				if (do_not_force)
				{
					simulation_entity_update(entity_index, object_index, update_mask);
				}
				else
				{
					simulation_entity_force_update(entity_index, object_index, update_mask);
				}
			}
		}
	}
}