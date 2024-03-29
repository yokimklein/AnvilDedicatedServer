#pragma once
#include "..\..\cseries\cseries.h"
#include "..\..\memory\data.h"

enum e_simulation_entity_type
{
	k_simulation_entity_type_none = -1,
	_simulation_entity_type_game_engine_slayer,
	_simulation_entity_type_game_engine_ctf,
	_simulation_entity_type_game_engine_assault,
	_simulation_entity_type_game_engine_oddball,
	_simulation_entity_type_game_engine_king,
	_simulation_entity_type_game_engine_territories,
	_simulation_entity_type_game_engine_juggernaut,
	_simulation_entity_type_game_engine_sandbox,
	_simulation_entity_type_game_engine_infection,
	_simulation_entity_type_game_engine_vip,
	_simulation_entity_type_game_engine_player,
	_simulation_entity_type_game_engine_statborg,
	_simulation_entity_type_breakable_surface_group,
	_simulation_entity_type_map_variant,
	_simulation_entity_type_unit,
	_simulation_entity_type_item,
	_simulation_entity_type_generic,
	_simulation_entity_type_generic_garbage,
	_simulation_entity_type_vehicle,
	_simulation_entity_type_projectile,
	_simulation_entity_type_weapon,
	_simulation_entity_type_device,

	k_simulation_entity_type_maximum_count = 32
};

enum e_simulation_entity_flag
{
	k_simulation_entity_flag_count = 2
};

struct s_simulation_entity
{
	dword entity_index;
	e_simulation_entity_type entity_type;
	bool exists_in_gameworld;
	byte event_reference_count;
	datum_index gamestate_index;
	qword pending_update_mask;
	qword force_update_mask;
	dword creation_data_size;
	void* creation_data;
	dword state_data_size;
	void* state_data;
};
static_assert(sizeof(s_simulation_entity) == 0x30);

long simulation_entity_create(e_simulation_entity_type simulation_entity_type, long object_index, datum_index gamestate_index);
e_simulation_entity_type simulation_entity_type_from_game_engine();
void simulation_entity_update(long entity_index, long unknown, c_flags<long, ulong64, 64>* update_mask);