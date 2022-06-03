#include "network_managed_session.h"
#include "network_session.h"
#include <iostream>

// TEST THIS - function exists in ms29 but I couldn't get it to call correctly
bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform)
{
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];

	if (managed_session_index == -1 || (managed_session->flags & 0x10) == 0)
		return false;
	if (out_secure_host_description)
		*out_secure_host_description = managed_session->actual_online_session_state.description;
	if (out_transport_platform)
		*out_transport_platform = managed_session->platform;
	return true;
}

// FUNC TODO
const char* managed_session_get_id_string(long managed_session_index)
{
	const char* managed_session_id = "00:00:00:00:00:00:00:00";
	return managed_session_id;
}

// this exists in ms29, I rewrote it thinking there was an issue with it but there wasn't, oh well lol
bool managed_session_get_id(long index, s_transport_secure_identifier* secure_id)
{
	if (secure_id != nullptr)
		memset(secure_id, 0, sizeof(s_transport_secure_identifier));
	if (index == -1)
		return false;
	c_managed_session* managed_session = &online_session_manager_globals->managed_sessions[index];
	if ((managed_session->flags & 0x10) == 0) // test _online_managed_session_created_bit
		return false;
	if (secure_id != nullptr)
		memcpy(secure_id->data, managed_session->actual_online_session_state.description.session_id.data, sizeof(s_transport_secure_identifier));
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

void managed_session_remove_players(long managed_session_index, uint64_t* xuids, long xuid_count)
{
	void(__fastcall * remove_from_player_list)(s_online_session_player* players, long player_count, uint64_t* xuids, long xuid_count) = reinterpret_cast<decltype(remove_from_player_list)>(module_base + 0x290E0);
	
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	remove_from_player_list(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, xuids, xuid_count);
	managed_session->pending_operation_flags |= 0xC0u;
	managed_session->flags &= 0xFFFFFFFB;
	managed_session->creation_time = 0;
}

void managed_session_reset_players_add_status(long managed_session_index)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	managed_session->flags &= 0xFFFFE7FF;
}

void managed_session_add_players(long managed_session_index, uint64_t* player_xuids, bool* player_bools, long xuid_count)
{
	auto managed_session = &online_session_manager_globals->managed_sessions[managed_session_index];
	managed_session_add_players_internal(managed_session->desired_online_session_state.players, k_network_maximum_players_per_session, player_xuids, player_bools, xuid_count); // non-original function name
	managed_session->flags |= 0x400u;
	managed_session->pending_operation_flags |= 0x180u;
	managed_session->flags &= 0xFFFFFFFB;
	managed_session->creation_time = 0;
}

void managed_session_add_players_internal(s_online_session_player* players, long player_count, uint64_t* player_xuids, bool* player_bools, long xuid_count)
{
	for (long i = 0; i < player_count; i++)
	{
		long player_index = -1;
		for (player_index = 0; player_index < player_count; player_index++)
		{
			if (players[player_index].flags & 1 && player_xuids[i] == players[player_index].xuid)
				break;
		}
		if (player_index == player_count)
		{
			players[player_index].xuid = player_xuids[i];
			players[player_index].flags |= 1u;
			if (player_bools[i])
				players[player_index].flags |= 2u;
		}
	}
}