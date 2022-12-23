#include "network_managed_session.h"
#include "..\..\cseries\cseries.h"
#include "network_session.h"
#include <iostream>

// TEST THIS - function exists in ms29 but I couldn't get it to call correctly
// ms23's function has an extra bool & code, which seems to be related to matchmaking
bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];

	if (managed_session_index == -1 || (managed_session->flags & 0x10) == 0)
		return false;
	if (out_secure_host_description != nullptr)
		memcpy(out_secure_host_description, &managed_session->actual_online_session_state.description, sizeof(s_transport_session_description));
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
	if ((managed_session->flags & _online_managed_session_created_bit) == 0)
		return false;
	if (secure_id != nullptr)
		memcpy(secure_id, &managed_session->actual_online_session_state.description.session_id, sizeof(s_transport_secure_identifier));
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
		managed_session->pending_operation_flags |= 0x80u;
		managed_session->flags &= 0xFFFFFFFB;
		managed_session->creation_time = 0;
	}
	if (friends_only)
	{
		if ((managed_session->flags & 2) == 0 && (managed_session->flags & 0x40000) == 0)
			managed_session->flags = managed_session->flags | 0x20000;
	}
}

short* managed_session_get_status(short* managed_session_status, long managed_session_index)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	auto flags = managed_session->flags;
	short status = 0;
	if ((flags & 0x10) != 0)
		status = 2;
	if ((flags & 0x20) != 0)
		status |= 4u;
	if ((managed_session->current_operation_flags & 4) != 0 || (managed_session->pending_operation_flags & 4) != 0)
		status |= 1u;
	if ((flags & 0x40) != 0)
		status |= 8u;
	if ((flags & 0x80) != 0)
		status |= 0x10u;
	if ((managed_session->current_operation_flags & 0x10) != 0 || (managed_session->pending_operation_flags & 0x10) != 0)
		status |= 8u;
	if ((flags & 0x800) != 0)
		status |= 0x40u;
	if ((flags & 0x1000) != 0)
		status |= 0x80u;
	if ((flags & 0x400) != 0)
		status |= 0x20u;
	if ((managed_session->current_operation_flags & 0x400) != 0 || (managed_session->pending_operation_flags & 0x400) != 0)
		status |= 0x100u;
	if ((flags & 0x2000) != 0)
		status |= 0x200u;
	if ((flags & 0x4000) != 0)
		status |= 0x400u;
	if ((managed_session->current_operation_flags & 0x200) != 0 || (managed_session->pending_operation_flags & 0x200) != 0)
		status |= 0x800u;
	if ((flags & 0x8000) != 0)
		status |= 0x1000u;
	if ((flags & 0x10000) != 0)
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
		auto flags = managed_session->flags;
		return (flags & 2) != 0;
	}
}

void managed_session_reset_session(long managed_session_index, bool use_session_time)
{
	c_network_session* life_cycle_session = (c_network_session*)(module_base + 0x3EADFD0); // life cycle session 0 - desired state session?

	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: managed_session_reset_session: [%s]:%08X reset session\n",
		transport_session_description_get_string(&managed_session->desired_online_session_state.description),
		managed_session_index);
	managed_session->pending_operation_flags |= 1u;
	managed_session->desired_online_session_state.public_slots_flags |= 1u;
	managed_session->creation_contexts[0].id = _online_context_id_game_type;
	managed_session->pending_operation_flags = managed_session->pending_operation_flags & 0xFFEF | 0x184;
	if (use_session_time)
	{
		managed_session->flags |= (1 << 2);
		if (life_cycle_session->m_time_exists)
			managed_session->creation_time = life_cycle_session->m_time;
		else
			managed_session->creation_time = timeGetTime();
	}
	else
	{
		managed_session->flags &= ~(1 << 2);
		managed_session->creation_time = 0;
	}
}

void managed_session_remove_players(long managed_session_index, qword* xuids, long xuid_count)
{
	//void(__fastcall* remove_from_player_list)(s_online_session_player* players, long player_count, qword* xuids, long xuid_count) = reinterpret_cast<decltype(remove_from_player_list)>(module_base + 0x290E0);
	
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	remove_from_player_list(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, xuids, xuid_count);
	managed_session->pending_operation_flags |= 0xC0u;
	managed_session->flags &= 0xFFFFFFFB;
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
	managed_session->flags &= 0xFFFFE7FF;
}

void managed_session_add_players(long managed_session_index, qword* player_xuids, bool* player_bools, long xuid_count)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	managed_session_add_players_internal(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, player_xuids, player_bools, xuid_count); // non-original function name
	managed_session->flags |= 0x400u;
	managed_session->pending_operation_flags |= 0x180u;
	managed_session->flags &= 0xFFFFFFFB;
	managed_session->creation_time = 0;
}

// TODO VERIFY THIS!!!!!!!
void managed_session_add_players_internal(s_online_session_player* players, long player_count, qword* player_xuids, bool* player_bools, long xuid_count)
{
	for (long xuid_index = 0; xuid_index < xuid_count; xuid_index++)
	{
		long player_index = -1;
		if (player_xuids[xuid_index])
		{
			long player_index2 = 0;
			for (player_index2 = 0; player_index2 < player_count; player_index2++)
			{
				if (((players[player_index2].flags & 1) != 0 || player_index2 != -1) && player_xuids[xuid_index] == players[player_index2].xuid)
					break;
				else
					player_index = player_index2;
			}
			if (player_index2 == player_count)
			{
				players[player_index].xuid = player_xuids[xuid_index];
				players[player_index].flags |= 1u;
				if (player_bools[xuid_index])
					players[player_index].flags |= 2u;
			}
		}
	}
}

bool managed_session_compare_id(long managed_session_index, s_transport_secure_identifier const* secure_id)
{
	bool(__fastcall * managed_session_compare_id_call)(long managed_session_index, s_transport_secure_identifier const* secure_id) = reinterpret_cast<decltype(managed_session_compare_id_call)>(module_base + 0x28B40);
	return managed_session_compare_id_call(managed_session_index, secure_id);
}