#include "simulation_game_action.h"
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <simulation\simulation_gamestate_entities.h>
#include <simulation\simulation_entity_database.h>
#include <simulation\simulation_world.h>
#include <simulation\game_interface\simulation_game_objects.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <stdio.h>

c_simulation_object_update_flags::c_simulation_object_update_flags() : c_flags<long, ulong64, 64>()
{
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index object_index, e_simulation_object_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(object_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index weapon_index, e_simulation_weapon_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(weapon_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index generic_index, e_simulation_generic_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(generic_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index item_index, e_simulation_item_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(item_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index projectile_index, e_simulation_projectile_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(projectile_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index vehicle_index, e_simulation_vehicle_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(vehicle_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index device_index, e_simulation_device_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(device_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index unit_index, e_simulation_unit_update_flag flag) : c_flags<long, ulong64, 64>()
{
	set_flag(unit_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(datum_index player_index, e_simulation_player_update_flag flag)
{
	set_flag(player_index, flag);
}
c_simulation_object_update_flags::c_simulation_object_update_flags(e_simulation_statborg_update_flag flag)
{
	set_flag(flag);
}

void c_simulation_object_update_flags::set_flag(datum_index object_index, e_simulation_object_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(object_index))
	//{
	//	ASSERT(simulation_entity_type_is_object(get_simulation_entity_type(object_index)));
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index weapon_index, e_simulation_weapon_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(weapon_index))
	//{
	//	ASSERT(get_simulation_entity_type(weapon_index) == _simulation_entity_type_weapon);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index generic_index, e_simulation_generic_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(generic_index))
	//{
	//	ASSERT(get_simulation_entity_type(generic_index) == _simulation_entity_type_generic || get_simulation_entity_type(generic_index) == _simulation_entity_type_generic_garbage);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index item_index, e_simulation_item_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(item_index))
	//{
	//	ASSERT(get_simulation_entity_type(item_index) == _simulation_entity_type_item || get_simulation_entity_type(item_index) == _simulation_entity_type_weapon);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index projectile_index, e_simulation_projectile_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(projectile_index))
	//{
	//	ASSERT(get_simulation_entity_type(projectile_index) == _simulation_entity_type_projectile);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index vehicle_index, e_simulation_vehicle_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(vehicle_index))
	//{
	//	ASSERT(get_simulation_entity_type(vehicle_index) == _simulation_entity_type_vehicle);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index device_index, e_simulation_device_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(device_index))
	//{
	//	ASSERT(get_simulation_entity_type(device_index) == _simulation_entity_type_device);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index unit_index, e_simulation_unit_update_flag flag)
{
	//if (simulation_object_is_attached_to_distributed_networking(unit_index))
	//{
	//	ASSERT(get_simulation_entity_type(unit_index) == _simulation_entity_type_unit);
	//}

	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(datum_index player_index, e_simulation_player_update_flag flag)
{
	set(flag, true);
}
void c_simulation_object_update_flags::set_flag(e_simulation_statborg_update_flag flag)
{
	set(flag, true);
}

template<typename t_flag_enum>
__declspec(noinline) void __fastcall simulation_action_object_update(datum_index object_index, t_flag_enum flag)
{
	throw "no available 'simulation_action_object_update' templated function";
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_object_update_flag>(datum_index object_index, e_simulation_object_update_flag flag)
{
	c_simulation_object_update_flags flags(object_index, flag);
	simulation_action_object_update_internal(object_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_weapon_update_flag>(datum_index weapon_index, e_simulation_weapon_update_flag flag)
{
	c_simulation_object_update_flags flags(weapon_index, flag);
	simulation_action_object_update_internal(weapon_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_generic_update_flag>(datum_index generic_index, e_simulation_generic_update_flag flag)
{
	c_simulation_object_update_flags flags(generic_index, flag);
	simulation_action_object_update_internal(generic_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_item_update_flag>(datum_index item_index, e_simulation_item_update_flag flag)
{
	c_simulation_object_update_flags flags(item_index, flag);
	simulation_action_object_update_internal(item_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_projectile_update_flag>(datum_index projectile_index, e_simulation_projectile_update_flag flag)
{
	c_simulation_object_update_flags flags(projectile_index, flag);
	simulation_action_object_update_internal(projectile_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_vehicle_update_flag>(datum_index vehicle_index, e_simulation_vehicle_update_flag flag)
{
	c_simulation_object_update_flags flags(vehicle_index, flag);
	simulation_action_object_update_internal(vehicle_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_device_update_flag>(datum_index device_index, e_simulation_device_update_flag flag)
{
	c_simulation_object_update_flags flags(device_index, flag);
	simulation_action_object_update_internal(device_index, flags);
}
template<>
__declspec(noinline) void __fastcall simulation_action_object_update<e_simulation_unit_update_flag>(datum_index unit_index, e_simulation_unit_update_flag flag)
{
	c_simulation_object_update_flags flags(unit_index, flag);
	simulation_action_object_update_internal(unit_index, flags);
}

void simulation_action_game_engine_globals_create()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	ASSERT(game_engine_globals_get_gamestate_index() == NONE);
	datum_index gamestate_index = simulation_gamestate_entity_create();
	game_engine_globals_set_gamestate_index(gamestate_index);
	simulation_gamestate_entity_set_object_index(gamestate_index, NONE);

	if (game_is_playback())
	{
		return;
	}

	e_simulation_entity_type entity_type = simulation_entity_type_from_game_engine();
	if (entity_type == NONE)
	{
		return;
	}

	long entity_index = simulation_entity_create(entity_type, NONE, gamestate_index);
	if (entity_index == NONE)
	{
		return;
	}

	c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
	s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
	ASSERT(entity->gamestate_index != NONE);
	simulation_entity_database->entity_capture_creation_data(entity_index);
}

void simulation_action_game_engine_globals_delete()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	datum_index gamestate_index = game_engine_globals_get_gamestate_index();
	if (gamestate_index == NONE)
	{
		event(_event_warning, "networking:simulation:action game engine globals has invalid gamestate index, can't delete");
	}
	else
	{
		if (!game_is_playback())
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == NONE)
			{
				event(_event_warning, "networking:simulation:action game engine globals [gamestate index 0x%08X] has no entity to delete", gamestate_index);
			}
			else
			{
				simulation_entity_delete(entity_index, NONE, gamestate_index);
			}
		}
	}

	game_engine_globals_set_gamestate_index(NONE);
}

void simulation_action_game_statborg_create()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	ASSERT(game_engine_globals_get_statborg_gamestate_index() == NONE);
	datum_index gamestate_index = simulation_gamestate_entity_create();
	game_engine_globals_set_statborg_gamestate_index(gamestate_index);
	simulation_gamestate_entity_set_object_index(gamestate_index, NONE);

	if (game_is_playback())
	{
		return;
	}

	long entity_index = simulation_entity_create(_simulation_entity_type_game_engine_statborg, NONE, gamestate_index);
	if (entity_index == NONE)
	{
		return;
	}

	c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
	s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
	ASSERT(entity->gamestate_index != NONE);
	simulation_entity_database->entity_capture_creation_data(entity_index);
}

void simulation_action_game_statborg_delete()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	datum_index gamestate_index = game_engine_globals_get_statborg_gamestate_index();
	if (gamestate_index == NONE)
	{
		event(_event_warning, "networking:simulation:action: statborg has invalid gamestate index, cannot delete");
	}
	else
	{
		if (!game_is_playback())
		{
			long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
			if (entity_index == NONE)
			{
				event(_event_warning, "networking:simulation:action: statborg gamestate index 0x%08X not attached to entity", gamestate_index);
			}
			else
			{
				simulation_entity_delete(entity_index, NONE, gamestate_index);
			}
		}
		simulation_gamestate_entity_delete(gamestate_index);
	}
	game_engine_globals_set_statborg_gamestate_index(NONE);
}

void simulation_action_game_ai_create()
{
	// this function is empty in every build i've seen
	// is this for syncing ai over distributed/asynchronous games?
	// seems like it was planned at one point but unfinished & stubbed
}

void simulation_action_game_ai_delete()
{
	// ditto above
}

// only called for the sandbox engine (forge)
// TODO: test this
void simulation_action_game_map_variant_create_all()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	for (long i = 0; i < k_number_of_map_variant_simulation_entities; i++)
	{
		simulation_action_game_map_variant_create_for_chunk(i);
	}
}

void simulation_action_game_map_variant_create_for_chunk(long chunk_index)
{
	c_map_variant* map_variant = game_engine_get_runtime_map_variant();
	ASSERT(map_variant->get_chunk_gamestate_index(chunk_index) == NONE);
	datum_index gamestate_index = simulation_gamestate_entity_create();
	map_variant->simulation_gamestate_indices[chunk_index] = gamestate_index;
	simulation_gamestate_entity_set_object_index(gamestate_index, chunk_index);

	if (game_is_playback())
	{
		return;
	}

	long entity_index = simulation_entity_create(_simulation_entity_type_map_variant, chunk_index, gamestate_index);
	if (entity_index == NONE)
	{
		return;
	}

	c_simulation_entity_database* simulation_entity_database = simulation_get_world()->get_entity_database();
	s_simulation_entity* entity = simulation_entity_database->entity_get(entity_index);
	ASSERT(entity->gamestate_index != NONE);
	simulation_entity_database->entity_capture_creation_data(entity_index);
}

void simulation_action_game_map_variant_delete()
{
	if (!game_is_server() || !game_is_distributed())
	{
		return;
	}

	for (long i = 0; i < k_number_of_map_variant_simulation_entities; i++)
	{
		c_map_variant* map_variant = game_engine_get_runtime_map_variant();
		datum_index gamestate_index = map_variant->simulation_gamestate_indices[i];
		if (gamestate_index == NONE)
		{
			event(_event_warning, "networking:simulation:action map variant has invalid gamestate index, can't delete");
		}
		else
		{
			if (!game_is_playback())
			{
				long entity_index = simulation_gamestate_entity_get_simulation_entity_index(gamestate_index);
				if (entity_index == NONE)
				{
					event(_event_warning, "networking:simulation:action map variant [gamestate index 0x%08X] has no entity to delete", gamestate_index);
				}
				else
				{
					simulation_entity_delete(entity_index, i, gamestate_index);
				}
			}
			simulation_gamestate_entity_delete(gamestate_index);
		}
		map_variant->simulation_gamestate_indices[i] = NONE;
	}
}

void simulation_action_breakable_surfaces_create()
{
	// there is code for this in ms23, but it doesn't seem like it would function?
	// TODO: revisit this
}

//void simulation_action_actor_create(datum_index unit_index)
//{
//	if (!game_is_server() || !game_is_distributed())
//	{
//		return
//	}
// 
//	unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
//	ASSERT(unit->object.gamestate_index == NONE);
//	simulation_action_object_create(unit_index);
//	ASSERT(unit->unit.simulation_actor_index == NONE);
//	unit->unit.simulation_actor_index = simulation_get_world()->create_actor(NONE, unit_index); // TODO
//	unit_set_actively_controlled(unit_index, true);
//	simulation_action_object_update(unit_index, _simulation_unit_update_control);
//}