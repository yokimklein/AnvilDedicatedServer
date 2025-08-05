#include "network_managed_session.h"
#include <cseries\cseries.h>
#include <networking\session\network_session.h>
#include <iostream>
#include <networking\network_time.h>

REFERENCE_DECLARE(0x3EAB120, s_online_session_manager_globals, online_session_manager_globals);

bool managed_session_get_security_information(long index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);

	if (index == NONE || !managed_session->flags.test(_online_managed_session_created_bit))
	{
		return false;
	}

	if (out_secure_host_description)
	{
		out_secure_host_description->session_id = managed_session->actual_online_session_state.description.session_id;
		out_secure_host_description->host_address = managed_session->actual_online_session_state.description.host_address;
		out_secure_host_description->key = managed_session->actual_online_session_state.description.key;
	}
	if (out_transport_platform)
	{
		*out_transport_platform = managed_session->platform;
	}
	return true;
}

const char* managed_session_get_id_string(long index)
{
	if (index == NONE)
	{
		return "00:00:00:00:00:00:00:00";
	}

	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	return transport_secure_identifier_get_string(&managed_session->actual_online_session_state.description.session_id);
}

bool managed_session_get_id(long index, s_transport_secure_identifier* out_secure_identifier)
{
	if (out_secure_identifier)
	{
		csmemset(out_secure_identifier, 0, sizeof(s_transport_secure_identifier));
	}

	if (index == NONE)
	{
		return false;
	}

	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	if (!managed_session->flags.test(_online_managed_session_created_bit))
	{
		return false;
	}
	if (out_secure_identifier)
	{
		*out_secure_identifier = managed_session->actual_online_session_state.description.session_id;
	}
	return true;
}

void managed_session_modify_slot_counts(long index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	s_online_session& online_session = managed_session->desired_online_session_state;

	if (online_session.private_slot_count != private_slot_count || online_session.public_slot_count != public_slot_count || TEST_BIT(online_session.public_slots_flags, 9) != friends_only)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_modify_slot_counts: [%s]:%08X modify slot count from %d/%d/fo=%d to %d/%d/fo=%d\n",
			transport_session_description_get_string(&online_session.description),
			index,
			online_session.private_slot_count,
			online_session.public_slot_count,
			TEST_BIT(online_session.public_slots_flags, 9),
			private_slot_count,
			public_slot_count,
			friends_only);

		online_session.private_slot_count = private_slot_count;
		online_session.public_slot_count = public_slot_count;

		if (friends_only)
		{
			SET_BIT(online_session.public_slots_flags, 9, true);
		}
		else
		{
			SET_BIT(online_session.public_slots_flags, 9, false);
		}

		managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
		managed_session->flags.set(_online_managed_session_pending_retry_bit, false);
		managed_session->time_of_last_failure = 0;
	}
	if (peer_count != 0)
	{
		if (!managed_session->flags.test(_online_managed_session_master_bit) && !managed_session->flags.test(_online_managed_session_flags_locked_bit))
		{
			managed_session->flags.set(_online_managed_session_locking_flags_bit, true);
		}
	}
}

c_managed_session_status_flags managed_session_get_status(long index)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	c_managed_session_status_flags status = {};
	if (managed_session->flags.test(_online_managed_session_created_bit))
	{
		status.set(_managed_session_creation_succeeded_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_creation_failed_bit))
	{
		status.set(_managed_session_creation_failed_bit, true);
	}
	if (managed_session->current_operation_flags.test(_online_managed_session_create_bit) ||
		managed_session->pending_operation_flags.test(_online_managed_session_create_bit))
	{
		status.set(_managed_session_creation_in_progress_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_host_migration_in_process_bit))
	{
		status.set(_managed_session_host_migration_in_progress_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_host_migration_session_created_bit))
	{
		status.set(_managed_session_host_migration_session_created_bit, true);
	}
	if (managed_session->current_operation_flags.test(_online_managed_session_create_host_migration_bit) ||
		managed_session->pending_operation_flags.test(_online_managed_session_create_host_migration_bit))
	{
		status.set(_managed_session_host_migration_in_progress_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_players_add_succeeded_bit))
	{
		status.set(_managed_session_players_add_succeeded_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_players_add_failed_bit))
	{
		status.set(_managed_session_players_add_failed_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_players_add_pending_bit))
	{
		status.set(_managed_session_players_add_in_progress_bit, true);
	}
	if (managed_session->current_operation_flags.test(_online_managed_session_game_start_bit) ||
		managed_session->pending_operation_flags.test(_online_managed_session_game_start_bit))
	{
		status.set(_managed_session_game_start_in_progress_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_game_started_bit))
	{
		status.set(_managed_session_game_started_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_game_start_failed_bit))
	{
		status.set(_managed_session_game_start_failed_bit, true);
	}
	if (managed_session->current_operation_flags.test(_online_managed_session_game_end_bit) ||
		managed_session->pending_operation_flags.test(_online_managed_session_game_end_bit))
	{
		status.set(_managed_session_game_end_in_progress_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_game_ended_bit))
	{
		status.set(_managed_session_game_ended_bit, true);
	}
	if (managed_session->flags.test(_online_managed_session_game_end_failed_bit))
	{
		status.set(_managed_session_game_end_failed_bit, true);
	}
	return status;
}

bool managed_session_is_master_session(long index)
{
	if (index == NONE)
	{
		return false;
	}

	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	return managed_session->flags.test(_online_managed_session_master_bit);
}

void managed_session_reset_session(long index, bool recreating_session)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_reset_session: [%s]:%08X reset session\n",
		transport_session_description_get_string(&managed_session->desired_online_session_state.description),
		index);

	managed_session->pending_operation_flags.set(_online_managed_session_delete_bit, true);
	SET_BIT(managed_session->desired_online_session_state.public_slots_flags, 0, true);
	//managed_session->we_are_the_new_host = true;
	managed_session->pending_operation_flags.set(_online_managed_session_create_host_migration_bit, false);
	managed_session->pending_operation_flags.set(_online_managed_session_create_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_players_add_bit, true);

	if (recreating_session)
	{
		managed_session->flags.set(_online_managed_session_pending_retry_bit, true);
		managed_session->time_of_last_failure = network_time_get();
	}
	else
	{
		managed_session->flags.set(_online_managed_session_pending_retry_bit, false);
		managed_session->time_of_last_failure = 0;
	}
}

void managed_session_remove_players(long index, qword const* xuids, long xuid_count)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	remove_from_player_list(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, xuids, xuid_count);
	
	for (long player_index = 0; player_index < xuid_count; player_index++)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_remove_players: [0x%I64x] from session 0x%8X\n", xuids[player_index], index);
	}

	managed_session->pending_operation_flags.set(_online_managed_session_players_remove_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->flags.set(_online_managed_session_pending_retry_bit, false);
	managed_session->time_of_last_failure = 0;
}

void remove_from_player_list(s_online_session_player* players, long player_count, qword const* xuids, long xuid_count)
{
	ASSERT(players);
	ASSERT(player_count > 0);
	ASSERT(xuids);

	for (long xuid_index = 0; xuid_index < xuid_count; xuid_index++)
	{
		long player_index = 0;
		for (player_index = 0; player_index < player_count; player_index++)
		{
			if (players[player_index].flags.test(_online_session_player_valid_bit) && players[player_index].xuid == xuids[xuid_index])
			{
				players[player_index].flags.clear();
				players[player_index].xuid = 0;
				csmemset(&players[player_index], 0, sizeof(s_online_session_player));
				break;
			}
		}
		if (player_index == player_count)
		{
			// previously used 0x%Lx, but this caused compiler warnings
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: remove_from_player_list: unable to find player [0x%llx] in the list to remove from the session\n", xuids[player_index]);
		}
	}
}

void managed_session_reset_players_add_status(long index)
{
	ASSERT(index < k_managed_sessions_max_count);

	s_online_managed_session* managed_session = managed_session_get(index);
	managed_session->flags.set(_online_managed_session_players_add_succeeded_bit, false);
	managed_session->flags.set(_online_managed_session_players_add_failed_bit, false);
}

void managed_session_add_players(long index, qword const* xuids, bool const* xuids_left_game, long xuid_count)
{
	ASSERT(index < k_managed_sessions_max_count);
	s_online_managed_session* managed_session = managed_session_get(index);
	ASSERT(!managed_session->flags.test(_online_managed_session_players_add_pending_bit));

	add_to_player_list(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, xuids, xuids_left_game, xuid_count);
	managed_session->flags.set(_online_managed_session_players_add_pending_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_modify_session_bit, true);
	managed_session->pending_operation_flags.set(_online_managed_session_players_add_bit, true);
	managed_session->flags.set(_online_managed_session_pending_retry_bit, false);
	managed_session->time_of_last_failure = 0;
}

void add_to_player_list(s_online_session_player* players, long player_count, qword const* xuids, bool const* xuids_left_game, long xuid_count)
{
	ASSERT(players);
	ASSERT(player_count > 0);
	ASSERT(xuids);

	for (long xuid_index = 0; xuid_index < xuid_count; xuid_index++)
	{
		long empty_index = NONE;
		if (xuids[xuid_index])
		{
			long player_index = 0;
			for (player_index = 0; player_index < player_count; player_index++)
			{
				if (players[player_index].flags.test(_online_session_player_valid_bit) || empty_index != NONE)
				{
					if (!csmemcmp(&xuids[xuid_index], &players[player_index].xuid, sizeof(qword)))
					{
						break;
					}
				}
				else
				{
					empty_index = player_index;
				}
			}
			if (player_index == player_count)
			{
				ASSERT(empty_index != NONE);
				players[empty_index].xuid = xuids[xuid_index];
				players[empty_index].flags.set(_online_session_player_valid_bit, true);
				if (xuids_left_game[xuid_index])
				{
					players[empty_index].flags.set(_online_session_player_occupies_private_slot_bit, true);
				}
			}
		}
	}
}

bool __fastcall managed_session_compare_id(long index, s_transport_secure_identifier const* secure_id)
{
	if (index == NONE)
	{
		return false;
	}

	ASSERT(index < k_managed_sessions_max_count);

	return INVOKE(0x28B40, managed_session_compare_id, index, secure_id);
}

void __fastcall managed_session_delete_session_internal(long index, s_online_managed_session* managed_session)
{
	INVOKE(0x28C30, managed_session_delete_session_internal, index, managed_session);
}

s_online_managed_session* managed_session_get(long index)
{
	ASSERT(index < k_managed_sessions_max_count);
	return &online_session_manager_globals.managed_sessions[index];
}