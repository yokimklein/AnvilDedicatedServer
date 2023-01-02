#include "network_managed_session.h"
#include "..\..\cseries\cseries.h"
#include "network_session.h"
#include <iostream>
#include <assert.h>
#include "..\network_time.h"

bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];

	if (managed_session_index == -1 || !managed_session->flags.test(_online_managed_session_created_bit))
		return false;
	if (out_secure_host_description != nullptr)
	{
		out_secure_host_description->session_id = managed_session->actual_online_session_state.description.session_id;
		out_secure_host_description->host_address = managed_session->actual_online_session_state.description.host_address;
		out_secure_host_description->key = managed_session->actual_online_session_state.description.key;
	}
	if (out_transport_platform != nullptr)
		*out_transport_platform = managed_session->platform;
	return true;
}

const char* managed_session_get_id_string(long managed_session_index)
{
	if (managed_session_index == -1)
		return "00:00:00:00:00:00:00:00";
	else
		return transport_secure_identifier_get_string(&online_session_manager_globals->managed_sessions[managed_session_index].actual_online_session_state.description.session_id);
}

// this exists in ms29, I rewrote it thinking there was an issue with it but there wasn't, oh well lol
bool managed_session_get_id(long managed_session_index, s_transport_secure_identifier* secure_id)
{
	if (secure_id != nullptr)
		memset(secure_id, 0, sizeof(s_transport_secure_identifier));
	if (managed_session_index == -1)
		return false;
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	if (!managed_session->flags.test(_online_managed_session_created_bit))
		return false;
	if (secure_id != nullptr)
		*secure_id = managed_session->actual_online_session_state.description.session_id;
	return true;
}

void managed_session_modify_slot_counts(long managed_session_index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	if (managed_session->desired_online_session_state.private_slot_count != private_slot_count
		|| managed_session->desired_online_session_state.public_slot_count != public_slot_count
		|| ((managed_session->desired_online_session_state.public_slots_flags & 0x200) != 0))
	{
		managed_session->desired_online_session_state.private_slot_count = private_slot_count;
		managed_session->desired_online_session_state.public_slot_count = public_slot_count;
		if (public_slot_count)
			managed_session->desired_online_session_state.public_slots_flags |= 0x200u;
		else
			managed_session->desired_online_session_state.public_slots_flags &= 0xFDFFu;
		managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
		managed_session->flags.set(_online_managed_session_recreating_bit, false);
		managed_session->creation_time = 0;
	}
	if (friends_only)
	{
		if (!managed_session->flags.test(_online_managed_session_master_session_bit) && !managed_session->flags.test(_online_managed_session_session_is_locked_bit))
			managed_session->flags.set(_online_managed_session_lock_session_bit, true);
	}
}

short* managed_session_get_status(short* managed_session_status, long managed_session_index)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	short status = 0; // TODO: e_managed_session_status_flags
	if (managed_session->flags.test(_online_managed_session_created_bit))
		status = 2;
	if (managed_session->flags.test(_online_managed_session_unknown_flag5_bit))
		status |= 4u;
	if (managed_session->current_operation_flags.test(_online_managed_session_create_bit) || managed_session->pending_operation_flags.test(_online_managed_session_create_bit))
		status |= 1u;
	if (managed_session->flags.test(_online_managed_session_host_migration_in_process_bit))
		status |= 8u;
	if (managed_session->flags.test(_online_managed_session_host_migration_session_created_bit))
		status |= 0x10u;
	if (managed_session->current_operation_flags.test(_online_managed_session_create_host_migration_bit) || managed_session->pending_operation_flags.test(_online_managed_session_create_host_migration_bit))
		status |= 8u;
	if (managed_session->flags.test(_online_managed_session_unknown_flag11_bit))
		status |= 0x40u;
	if (managed_session->flags.test(_online_managed_session_unknown_flag12_bit))
		status |= 0x80u;
	if (managed_session->flags.test(_online_managed_session_players_add_pending_bit))
		status |= 0x20u;
	if (managed_session->current_operation_flags.test(_online_managed_session_game_start_bit) || managed_session->pending_operation_flags.test(_online_managed_session_game_start_bit))
		status |= 0x100u;
	if (managed_session->flags.test(_online_managed_session_game_start_succeeded_bit))
		status |= 0x200u;
	if (managed_session->flags.test(_online_managed_session_game_start_failed_bit))
		status |= 0x400u;
	if (managed_session->current_operation_flags.test(_online_managed_session_game_end_bit) || managed_session->pending_operation_flags.test(_online_managed_session_game_end_bit))
		status |= 0x800u;
	if (managed_session->flags.test(_online_managed_session_game_end_succeeded_bit))
		status |= 0x1000u;
	if (managed_session->flags.test(_online_managed_session_game_end_failed_bit))
		status |= 0x2000u;
	*managed_session_status = status;
	return managed_session_status;
}

bool managed_session_is_master_session(long managed_session_index)
{
	if (managed_session_index == -1)
		return false;
	else
	{
		auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
		return managed_session->flags.test(_online_managed_session_master_session_bit);
	}
}

void managed_session_reset_session(long managed_session_index, bool recreating_session)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_reset_session: [%s]:%08X reset session\n",
		transport_session_description_get_string(&managed_session->desired_online_session_state.description),
		managed_session_index);
	managed_session->pending_operation_flags.set(_online_managed_session_delete_bit, true);
	managed_session->desired_online_session_state.public_slots_flags |= 1u;
	//managed_session->unknown1 = true;
	managed_session->pending_operation_flags.set(_online_managed_session_create_host_migration_bit, false);
	managed_session->pending_operation_flags.set(_online_managed_session_create_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_players_add_bit, true);
	if (recreating_session)
	{
		managed_session->flags.set(_online_managed_session_recreating_bit, true);
		managed_session->creation_time = network_time_get();
	}
	else
	{
		managed_session->flags.set(_online_managed_session_recreating_bit, false);
		managed_session->creation_time = 0;
	}
}

void managed_session_remove_players(long managed_session_index, qword* xuids, long xuid_count)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	remove_from_player_list(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, xuids, xuid_count);
	managed_session->pending_operation_flags.set(_online_managed_session_players_remove_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->flags.set(_online_managed_session_recreating_bit, false);
	managed_session->creation_time = 0;
}

void remove_from_player_list(s_online_session_player* players, long player_count, qword* xuids, long xuid_count)
{
	for (long xuid_index = 0; xuid_index < xuid_count; xuid_index++)
	{
		long player_index = 0;
		for (player_index = 0; player_index < player_count; player_index++)
		{
			if (players[player_index].flags & 1 && players[player_index].xuid == xuids[xuid_index])
			{
				players[player_index].flags = 0;
				players[player_index].xuid = 0;
				memset(&players[player_index], 0, sizeof(s_online_session_player));
				break;
			}
		}
		if (player_index == player_count)
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%I64x] in the list to remove from the session\n", xuids[player_index]); // uses 0x%Lx in ms23
		}
	}
}

void managed_session_reset_players_add_status(long managed_session_index)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	managed_session->flags.set(_online_managed_session_unknown_flag11_bit, false);
	managed_session->flags.set(_online_managed_session_unknown_flag8_bit, false);
}

void managed_session_add_players(long managed_session_index, qword* player_xuids, bool* player_bools, long xuid_count)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	managed_session_add_players_internal(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, player_xuids, player_bools, xuid_count); // non-original function name
	managed_session->flags.set(_online_managed_session_players_add_pending_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_players_add_bit, true);
	managed_session->flags.set(_online_managed_session_recreating_bit, false);
	managed_session->creation_time = 0;
}

void managed_session_add_players_internal(s_online_session_player* players, long player_count, qword* xuids, bool* player_bools, long xuid_count)
{
	assert(players);
	assert(player_count > 0);
	assert(xuids);

	for (long i = 0; i < xuid_count; i++)
	{
		long empty_index = -1;
		if (xuids[i])
		{
			long j = 0;
			for (j = 0; j < player_count; j++)
			{
				if ((players[j].flags & 1) != 0 || empty_index != -1)
				{
					if (!csmemcmp(&xuids[i], &players[j].xuid, 8))
						break;
				}
				else
				{
					empty_index = j;
				}
			}
			if (j == player_count)
			{
				assert(empty_index != NONE);
				players[empty_index].xuid = xuids[i];
				players[empty_index].flags |= 1u;
				if (player_bools[i])
					players[empty_index].flags |= 2u;
			}
		}
	}
}

bool managed_session_compare_id(long managed_session_index, s_transport_secure_identifier const* secure_id)
{
	bool(__fastcall* managed_session_compare_id_call)(long managed_session_index, s_transport_secure_identifier const* secure_id) = reinterpret_cast<decltype(managed_session_compare_id_call)>(module_base + 0x28B40);
	return managed_session_compare_id_call(managed_session_index, secure_id);
}