#include "players.h"
#include <game\game.h>
#include <game\game_engine.h>
#include <game\game_engine_util.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_units.h>
#include <stdio.h>
#include <simulation\game_interface\simulation_game_engine_player.h>

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
	player_datum* player_data = (player_datum*)datum_get(*players, player_index);
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
	return datum_absolute_index_to_index(*players, absolute_player_index);
}

void __fastcall player_increment_control_context(datum_index player_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(players);
	player_datum* player = (player_datum*)datum_get(*players, player_index);
	datum_index unit_index = player->unit_index;
	if (unit_index != NONE)
	{
		TLS_DATA_GET_VALUE_REFERENCE(object_headers);
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
	m_iterator.begin(*players);
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
		player_datum* player = (player_datum*)datum_get(*players, player_index);
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