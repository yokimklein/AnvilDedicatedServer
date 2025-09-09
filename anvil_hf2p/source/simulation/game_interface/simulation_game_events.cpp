#include "simulation_game_events.h"
#include <game\game.h>
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <simulation\simulation_world.h>
#include <networking\network_configuration.h>
#include <game\game_engine_event_definitions.h>
#include <game\game_engine_simulation.h>
#include <stdio.h>

void simulation_event_generate_for_remote_peers(e_simulation_event_type event_type, long entity_reference_count, datum_index* object_reference_indices, long ignore_player_index, long event_payload_size, void const* event_payload)
{
	static void* simulation_event_generate_for_remote_peers_call = base_address<void*>(0x7E490);
	__asm
	{
		push event_payload
		push event_payload_size
		push ignore_player_index
		push object_reference_indices
		mov edx, entity_reference_count
		mov ecx, event_type
		call simulation_event_generate_for_remote_peers_call
		add esp, 16
	}
}

void simulation_event_generate_for_clients(e_simulation_event_type event_type, long entity_reference_count, datum_index* object_reference_indices, long ignore_player_index, long event_payload_size, void const* event_payload)
{
	ASSERT(game_is_authoritative());
	simulation_event_generate_for_remote_peers(event_type, entity_reference_count, object_reference_indices, ignore_player_index, event_payload_size, event_payload);
}

void simulation_event_generate_for_client_player_list(e_simulation_event_type event_type, long entity_reference_count, datum_index* object_reference_indices, datum_index const* player_indices, long player_count, long event_payload_size, void const* event_payload)
{
	c_simulation_world* world = simulation_get_world();
	if (!world->is_distributed())
	{
		return;
	}

	c_simulation_event_handler* event_handler = world->get_event_handler();
	c_flags<long, ulong, k_maximum_players> player_mask;
	if (!player_indices || player_count < 1 || player_count > k_maximum_players)
	{
		event(_event_warning, "networking:simulation:event: not sending exclusive list event %d/%s for invalid player list [count %d]",
			event_type,
			event_handler->get_event_type_name(event_type), player_count);
	}
	else
	{
		for (long i = 0; i < player_count; i++)
		{
			if (player_indices[i] == NONE)
			{
				event(_event_warning, "networking:simulation:event: exclusive player list has bad player index %d/%s",
					event_type,
					event_handler->get_event_type_name(event_type));
			}
			else
			{
				player_mask.set(DATUM_INDEX_TO_ABSOLUTE_INDEX(player_indices[i]), true);
			}
		}
		simulation_event_generate_for_client_player_mask(event_type, entity_reference_count, object_reference_indices, player_mask, event_payload_size, event_payload);
	}
}

void simulation_event_generate_for_client_player_mask(e_simulation_event_type event_type, long entity_reference_count, datum_index* object_reference_indices, c_flags<long, ulong, k_maximum_multiplayer_players> player_mask, long event_payload_size, void const* event_payload)
{
	c_simulation_world* world = simulation_get_world();
	if (!world->is_distributed())
	{
		return;
	}

	c_simulation_event_handler* event_handler = world->get_event_handler();
	event(_event_verbose, "networking:simulation:event: generating exclusive list event %d/%s", event_type, event_handler->get_event_type_name(event_type));
	if (!world->is_authority())
	{
		return;
	}

	TLS_DATA_GET_VALUE_REFERENCE(players);
	c_flags<long, ulong, k_maximum_machines> machine_mask;
	for (long player_index = 0; player_index < k_maximum_players; player_index++)
	{
		if (player_mask.test(player_index))
		{
			player_datum* player = (player_datum*)datum_try_and_get_absolute(players, player_index);
			if (!player)
			{
				event(_event_warning, "networking:simulation:event: Failed to get a player for index %d for event %d/%s",
					player_index,
					event_type,
					event_handler->get_event_type_name(event_type));
			}
			else
			{
				short machine_index = player->machine_index;
				if (machine_index != NONE)
				{
					s_machine_identifier* machine_identifier = players_get_machine_identifier(machine_index);
					long machine_index = world->get_machine_index_by_identifier(machine_identifier);
					if (machine_index != NONE)
					{
						machine_mask.set(machine_index, true);
					}
				}
			}
		}
	}

	if (!machine_mask.is_empty())
	{
		event_handler_send_event(machine_mask, event_type, entity_reference_count, object_reference_indices, event_payload_size, event_payload);
	}
	else
	{
		event(_event_warning,"networking:simulation:event: exclusive player list for event %d/%s generated an invalid send machine mask",
			event_type,
			event_handler->get_event_type_name(event_type));
	}
}

void event_handler_send_event(c_flags<long, ulong, k_maximum_machines> machine_mask, e_simulation_event_type event_type, long entity_reference_count, datum_index const* object_reference_indices, long event_payload_size, void const* event_payload)
{
	c_simulation_world* world = simulation_get_world();
	c_simulation_event_handler* event_handler = world->get_event_handler();
	long entity_reference_indices[2];
	simulation_event_build_entity_reference_indices(entity_reference_count, object_reference_indices, entity_reference_indices);
	if (entity_reference_count <= 0)
	{
		entity_reference_indices[0] = 0;
	}
	long cancel_timer = get_network_configuration()->simulation_configuration.simulation_event_configurations[event_type].cancel_timer_milliseconds;
	world->m_distributed_world->m_entity_database.m_event_handler.send_event(event_type, entity_reference_count, entity_reference_indices, machine_mask, event_payload_size, event_payload, cancel_timer);
}

// Had to disable RTC here otherwise it'll throw an exception before add esp 4 corrects the stack
#pragma runtime_checks("", off)
void __fastcall simulation_event_build_entity_reference_indices(long entity_reference_count, datum_index const* object_reference_indices, long const* entity_reference_indices)
{
	INVOKE(0x7E610, simulation_event_build_entity_reference_indices, entity_reference_count, object_reference_indices, entity_reference_indices);
	__asm add esp, 4; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

void simulation_action_multiplayer_event(s_game_engine_event_data* game_engine_event)
{
	if (!game_is_distributed() || !game_is_server() || game_is_playback())
	{
		return;
	}

	s_game_engine_event_data event_data = *game_engine_event;
	if (event_data.audience_player_index != NONE)
	{
		event_data.audience_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(event_data.audience_player_index);
	}
	if (event_data.cause_player_index != NONE)
	{
		event_data.cause_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(event_data.cause_player_index);
	}
	if (event_data.effect_player_index != NONE)
	{
		event_data.effect_player_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(event_data.effect_player_index);
	}
	long player_response_list_count = 0;
	datum_index player_response_list[k_maximum_multiplayer_players];
	game_engine_event_build_player_response_list(game_engine_event, player_response_list, NUMBEROF(player_response_list), &player_response_list_count);
	if (player_response_list_count > 0)
	{
		simulation_event_generate_for_client_player_list(_simulation_event_type_game_engine, 0, nullptr, player_response_list, player_response_list_count, sizeof(event_data), &event_data);
	}
}