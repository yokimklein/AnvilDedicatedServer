#include "scenario_map_variant.h"
#include "assert.h"
#include "..\memory\tls.h"
#include "..\game\game_engine.h"
#include "..\game\game_engine_util.h"
#include "..\game\game.h"

c_map_variant::c_map_variant()
{
	FUNCTION_DEF(0xAB2F0, void, __thiscall, c_map_variant_ctor, c_map_variant* thisptr);
	c_map_variant_ctor(this);
}

void c_map_variant::create_default(e_map_id map_id)
{
	FUNCTION_DEF(0xAB380, e_map_id, __thiscall, create_default, c_map_variant* thisptr, e_map_id map_id);
	create_default(this, map_id);
}

c_map_variant* game_engine_get_runtime_map_variant()
{
	assert(game_engine_running());
	auto tls = get_tls();
	return &tls->game_engine_globals->map_variant;
}

datum_index c_map_variant::get_chunk_gamestate_index(long chunk_index)
{
	assert(VALID_INDEX(chunk_index, k_number_of_map_variant_simulation_entities));
	return this->simulation_gamestate_indices[chunk_index];
}

void c_map_variant::place_new_object(long map_variant_index, long unknown, bool do_not_force)
{
	if (game_is_server() && game_is_distributed() && game_engine_is_sandbox() && !game_is_playback())
	{
		long object_index = map_variant_index / 8;
		long gamestate_index = game_engine_get_runtime_map_variant()->simulation_gamestate_indices[map_variant_index / 8];
		if (gamestate_index != -1)
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index != -1)
			{
				c_flags<long, ulong64, 64> update_mask = {};
				update_mask = (unknown << (4 * (map_variant_index % 8)));
				if (do_not_force)
					simulation_entity_update(entity_index, object_index, &update_mask);
				else
					simulation_entity_force_update(entity_index, object_index, &update_mask);
			}
		}
	}
}