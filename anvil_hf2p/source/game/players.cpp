#include "players.h"
#include <game\game.h>
#include <game\game_engine.h>
#include <game\game_engine_util.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <scenario\scenario.h>
#include <game\multiplayer_definitions.h>
#include <anvil\backend\cache.h>
#include <game\game_time.h>
#include <game\player_mapping.h>
#include <stdio.h>

bool player_identifier_is_valid(s_player_identifier const* identifier)
{
	s_player_identifier invalid_identifier;
	csmemset(&invalid_identifier, 0, sizeof(invalid_identifier));

	if (*identifier != invalid_identifier)
	{
		return true;
	}

	return false;
}

const char* player_identifier_get_string(s_player_identifier const* identifier)
{
	static char identifier_str[0x18]{};
	for (long i = 0; i < sizeof(s_player_identifier); i++)
	{
		sprintf_s(identifier_str + (i * 3), 4, "%02x%c", *((byte*)identifier + i), i == (sizeof(s_player_identifier) - 1) ? 0 : ':');
	}
	return identifier_str;
}

long player_mapping_get_input_user(datum_index player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_mapping_globals);
	if (player_index == NONE)
	{
		return NONE;
	}
	else
	{
		return player_mapping_globals->player_input_user_mapping[player_index];
	}
}

void __fastcall player_set_facing(datum_index player_index, real_vector3d* forward)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player_data = (player_datum*)datum_get(players, player_index);
	if (game_is_authoritative())
	{
		if (player_data->unit_index != NONE)
		{
			TLS_DATA_GET_VALUE_REFERENCE(object_headers);
			unit_datum* unit = (unit_datum*)object_get_and_verify_type(player_data->unit_index, _object_mask_unit);
			unit->unit.facing_vector = *forward;
			unit->unit.aiming_vector = *forward;
			unit->unit.looking_vector = *forward;
			simulation_action_object_update(player_data->unit_index, _simulation_unit_update_desired_aiming_vector);
		}
	}
	long input_user_index = player_mapping_get_input_user((word)player_index);
	if (input_user_index != NONE)
	{
		player_control_set_facing(input_user_index, forward);
	}
}

void __fastcall player_control_set_facing(long input_user_index, real_vector3d* forward)
{
	INVOKE(0x106780, player_control_set_facing, input_user_index, forward);
}

long player_index_from_absolute_player_index(short absolute_player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	return datum_absolute_index_to_index(players, absolute_player_index);
}

void __fastcall player_increment_control_context(datum_index player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(players, player_index);
	datum_index unit_index = player->unit_index;
	if (unit_index != NONE)
	{
		//TLS_DATA_GET_VALUE_REFERENCE(object_headers);
		unit_datum* unit = (unit_datum*)object_get_and_verify_type(unit_index, _object_mask_unit);
		unit->unit.next_spawn_control_context = player->next_spawn_control_context;
		simulation_action_object_update(unit_index, _simulation_unit_update_control_context);
	}
	player->next_spawn_control_context++;
	SET_MASK(player->next_spawn_control_context, MASK(get_player_action_control_context_identifier_bits()), false);
}

bool __fastcall player_is_local(datum_index player_index)
{
	return INVOKE(0xC1480, player_is_local, player_index);
}

void __fastcall player_clear_assassination_state(datum_index player_index)
{
	INVOKE(0xBA0F0, player_clear_assassination_state, player_index);
}

long get_player_action_control_context_identifier_bits()
{
	return 4;

	// reach & h4
	//if (game_engine_is_sandbox())
	//	return 4;
	//return 2;
}

s_machine_identifier* players_get_machine_identifier(long machine_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	ASSERT(machine_index >= 0 && machine_index < k_maximum_machines);
	ASSERT(TEST_BIT(players_globals->machine_valid_mask, machine_index));
	return &players_globals->machine_identifiers[machine_index];
}

void c_player_in_game_iterator::begin()
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	m_iterator.begin(players);
}

bool c_player_in_game_iterator::next()
{
	for (m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.m_iterator);
		m_iterator.m_datum && m_iterator.m_datum->flags.test(_player_left_game_bit);
		m_iterator.m_datum = (player_datum*)data_iterator_next(&m_iterator.m_iterator))
	{
	}

	return m_iterator.m_datum != NULL;
}

player_datum* c_player_in_game_iterator::get_datum()
{
	return m_iterator.m_datum;
}

datum_index c_player_in_game_iterator::get_index() const
{
	return m_iterator.m_iterator.index;
}

short c_player_in_game_iterator::get_absolute_index() const
{
	return m_iterator.get_absolute_index();
}

void __fastcall player_notify_vehicle_ejection_finished(datum_index player_index)
{
	if (!game_is_predicted())
	{
		TLS_DATA_GET_VALUE_REFERENCE(players);
		player_datum* player = (player_datum*)datum_get(players, player_index);
		player->flags.set(_player_vehicle_entrance_ban_bit, true);
		player->vehicle_entrance_ban_ticks = (word)game_seconds_to_ticks_round(2.0f);
		simulation_action_game_engine_player_update(player_index, _simulation_player_update_vehicle_entrance_ban);
	}
}

void player_navpoint_data_set_action(s_player_navpoint_data* waypoint, e_navpoint_action action)
{
	if (action == _navpoint_action_none)
	{
		waypoint->next_navpoint_action = _navpoint_action_none;
		waypoint->current_navpoint_action = _navpoint_action_none;
		waypoint->current_navpoint_action_timer = 0;
	}
	else if (waypoint->current_navpoint_action == _navpoint_action_none)
	{
		waypoint->current_navpoint_action = action;
		waypoint->current_navpoint_action_timer = (byte)game_seconds_to_ticks_round(0.5f);
	}
	else if (action == waypoint->current_navpoint_action.get())
	{
		if (action == _navpoint_action_player_damaged || waypoint->next_navpoint_action != _navpoint_action_player_damaged)
		{
			waypoint->current_navpoint_action_timer = (byte)game_seconds_to_ticks_round(0.5f);
		}
	}
	else if (action != waypoint->next_navpoint_action.get())
	{
		if (!waypoint->next_navpoint_action || waypoint->current_navpoint_action == _navpoint_action_player_damaged)
		{
			waypoint->next_navpoint_action = action;
		}
		else
		{
			waypoint->current_navpoint_action = waypoint->next_navpoint_action;
			waypoint->next_navpoint_action = action;
			waypoint->current_navpoint_action_timer = (byte)game_seconds_to_ticks_round(0.5f);
		}
	}
}

void __fastcall player_swap(long player_absolute_index, long swap_player_absolute_index)
{
	INVOKE(0xB5340, player_swap, player_absolute_index, swap_player_absolute_index);
}

void __fastcall player_delete(datum_index player_index)
{
	INVOKE(0xB5670, player_delete, player_index);
}

s_s3d_player_loadout* player_get_loadout(player_datum* player)
{
	ASSERT(player);
	return &player->configuration.host.s3d_player_container.loadouts[player->active_loadout_index];
}

const s_s3d_player_loadout* player_get_loadout(const player_datum* player)
{
	ASSERT(player);
	return &player->configuration.host.s3d_player_container.loadouts[player->active_loadout_index];
}

bool player_has_consumable(const player_datum* player, long consumable_slot)
{
	ASSERT(player);
	ASSERT(VALID_INDEX(consumable_slot, k_consumable_slots));
	return player_get_loadout(player)->consumables[consumable_slot] != _consumable_none;
}

long _fastcall player_get_consumable_cost(const player_datum* player, long consumable_slot)
{
	ASSERT(player);
	ASSERT(VALID_INDEX(consumable_slot, k_consumable_slots));
	ulong consumable_index = player_get_loadout(player)->consumables[consumable_slot].get();

	// pull data from our backend cache instead of saber's
	if (!VALID_INDEX(consumable_index, g_backend_data_cache.m_consumable_costs.size()))
	{
		return 0;
	}
	return g_backend_data_cache.m_consumable_costs[consumable_index].energy_cost;
}

bool _fastcall player_can_use_consumable(const player_datum* player, long consumable_slot)
{
	ASSERT(player);
	ASSERT(VALID_INDEX(consumable_slot, k_consumable_slots));
	if (player->unit_index == NONE)
	{
		return false;
	}
	if (player->equipment_cooldown_ticks > 0)
	{
		return false;
	}
	if (!player_has_consumable(player, consumable_slot))
	{
		return false;
	}
	unit_datum* unit = (unit_datum*)object_get_and_verify_type(player->unit_index, _object_mask_unit);
	if (player_get_consumable_cost(player, consumable_slot) > unit->unit.consumable_energy_level)
	{
		return false;
	}
	return true;
}

long player_get_energy_regeneration_duration(const player_datum* player, long energy_level)
{
	ASSERT(player);

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal)
	{
		return NONE;
	}
	if (!VALID_INDEX(energy_level, universal->energy_regeneration.count()))
	{
		return NONE;
	}
	if (energy_level >= player_get_energy_regeneration_count(player))
	{
		return NONE;
	}
	s_multiplayer_energy_regeneration& energy_regeneration = universal->energy_regeneration[energy_level];
	real energy_regeneration_rate = player_get_modifier_value_real(player, _energy_regeneration_rate, 0.0f);
	real regenerated_energy_level = energy_regeneration_rate + 1.0f * static_cast<real>(energy_regeneration.energy_level);
	FLOOR(regenerated_energy_level, k_test_real_epsilon);
	return static_cast<long>(game_seconds_to_ticks_real(static_cast<real>(energy_regeneration.duration) / regenerated_energy_level));
}

long player_get_consumable_energy_restored_game_time(const player_datum* player, long energy_level, real energy_regeneration_rate)
{
	ASSERT(player);

	long energy_regeneration_duration = player_get_energy_regeneration_duration(player, energy_level);
	if (energy_regeneration_duration == NONE)
	{
		return NONE;
	}

	return static_cast<long>(static_cast<real>(energy_regeneration_duration) * energy_regeneration_rate) + game_time_get();
}

const real* player_get_loadout_modifiers(const player_datum* player)
{
	ASSERT(player);
	return player->configuration.host.s3d_player_container.modifiers[player->active_loadout_index].modifier_values;
}

long player_get_modifier_value_integer(const player_datum* player, e_modifiers modifier_index, long default_value)
{
	ASSERT(player);
	if (!player)
	{
		return default_value;
	}
	// this check wasn't in original, but it should be
	if (!VALID_INDEX(modifier_index, k_modifiers_count))
	{
		return default_value;
	}
	real modifier_value = player_get_loadout_modifiers(player)[modifier_index];
	if (modifier_value == 0.0f)
	{
		return default_value;
	}
	return static_cast<long>(modifier_value);
}

real player_get_modifier_value_real(const player_datum* player, e_modifiers modifier_index, real default_value)
{
	ASSERT(player);
	if (!player)
	{
		return default_value;
	}
	// this check wasn't in original, but it should be
	if (!VALID_INDEX(modifier_index, k_modifiers_count))
	{
		return default_value;
	}
	real modifier_value = player_get_loadout_modifiers(player)[modifier_index];
	if (modifier_value == 0.0f)
	{
		return default_value;
	}
	return modifier_value;
}

real player_get_consumable_cooldown_init(const player_datum* player, long consumable_slot)
{
	ASSERT(player);
	ASSERT(VALID_INDEX(consumable_slot, k_consumable_slots));
	ulong consumable_index = player_get_loadout(player)->consumables[consumable_slot].get();
	
	// pull data from our backend cache instead of saber's
	if (!VALID_INDEX(consumable_index, g_backend_data_cache.m_consumable_costs.size()))
	{
		return 0.0f;
	}
	return g_backend_data_cache.m_consumable_costs[consumable_index].cooldown_init;
}

real player_get_consumable_cooldown(const player_datum* player, long consumable_slot)
{
	ASSERT(player);
	ASSERT(VALID_INDEX(consumable_slot, k_consumable_slots));
	ulong consumable_index = player_get_loadout(player)->consumables[consumable_slot].get();

	// pull data from our backend cache instead of saber's
	if (!VALID_INDEX(consumable_index, g_backend_data_cache.m_consumable_costs.size()))
	{
		return 0.0f;
	}
	return g_backend_data_cache.m_consumable_costs[consumable_index].cooldown;
}

void player_set_consumable_cooldown(player_datum* player, real cooldown, real cooldown_init)
{
	ASSERT(player);
	long cooldown_ticks = game_seconds_to_ticks_round(cooldown);
	player->cooldown_reset_unknown42 = static_cast<word>(cooldown_ticks);
	player->equipment_cooldown_ticks = static_cast<word>(cooldown_ticks);
	player->cooldown_reset_unknown44 = static_cast<word>(game_seconds_to_ticks_round(cooldown_init));
}

long player_get_energy_regeneration_count(const player_datum* player)
{
	ASSERT(player);

	s_multiplayer_universal_globals_definition* universal = scenario_multiplayer_globals_try_and_get_universal_data();
	if (!universal)
	{
		return 0;
	}

	long energy_increase_max = player_get_modifier_value_integer(player, _energy_increase_max, 0) + 3;
	energy_increase_max = MAX(energy_increase_max, 0);

	long energy_regeneration_count = universal->energy_regeneration.count();
	if (energy_increase_max < energy_regeneration_count)
	{
		return energy_increase_max;
	}
	return energy_regeneration_count;
}

long players_get_local_machine_index()
{
	TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	return players_globals->local_machine_index;
}

void players_get_machines(ulong* machine_valid_mask, s_machine_identifier machine_identifiers[k_maximum_machines])
{
	ASSERT(machine_valid_mask);
	ASSERT(machine_identifiers);
	TLS_DATA_GET_VALUE_REFERENCE(players_globals);
	*machine_valid_mask = players_globals->machine_valid_mask;
	csmemcpy(machine_identifiers, players_globals->machine_identifiers.get_elements(), sizeof(players_globals->machine_identifiers));
}
