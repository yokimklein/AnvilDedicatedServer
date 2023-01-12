#include "scenario_map_variant.h"
#include "assert.h"
#include "..\memory\tls.h"
#include "..\game\game_engine.h"
#include "..\game\game_engine_util.h"

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