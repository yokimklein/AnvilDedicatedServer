#include "network_join.h"
#include <networking\network_utilities.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_life_cycle.h>
#include <iostream>
#include <networking\network_time.h>
#include <anvil\backend\user.h>
#include <anvil\backend\services\private_service.h>
#include <anvil\backend\cache.h>
#include <game\game.h>

REFERENCE_DECLARE(0x1039AF8, s_networking_join_data, g_network_join_data);

void network_join_add_join_to_queue(c_network_session* session, transport_address const* address, s_network_session_join_request const* join_request)
{
	long entry_count = g_network_join_data.join_queue_entry_count;

	// make sure join isn't closed
	e_network_join_refuse_reason closure_reason = session->can_accept_any_join_request();
	if (closure_reason != _network_join_refuse_reason_none)
	{
		session->acknowledge_join_request(address, closure_reason);
		return;
	}

	// check join status
	switch (g_network_join_data.join_queue_mode)
	{
		case _network_join_closed_to_all_joins:
		{
			printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: attempted to join while in _network_join_closed_to_all_joins\n");
			session->acknowledge_join_request(address, _network_join_refuse_reason_not_joinable);
			return;
		}
		case _network_join_open_to_join_squad:
		{
			if (session->session_type() == _network_session_type_squad)
			{
				break;
			}
			printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a group session while joining is in _network_join_open_to_join_squad\n");
			session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
			return;
		}
		// removed since ms23
		//case _network_join_queue_joins_to_group:
		//case _network_join_process_queued_group_joins:
		//{
		//    if (session->session_type() == _network_session_type_group)
		//        break;
		//    printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a squad session while in matchmaking\n");
		//    session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
		//    return;
		//}
		case _network_join_queue_closed_while_in_match:
		{
			printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join while in matchmaking\n");
			session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
			return;
		}
	}

	// ensure join request isn't already a session member
	for (long peer_index = session->get_session_membership()->get_first_peer(); peer_index != NONE; peer_index = session->get_session_membership()->get_next_peer(peer_index))
	{
		if (join_request->join_nonce == session->get_session_membership()->get_join_nonce(peer_index))
		{
			printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it already exists in the join queue\n", transport_address_get_string(address));
			return;
		}
	}

	// make sure the request entry isn't already in the queue (if the queue has entries)
	if (g_network_join_data.join_queue_entry_count != 0)
	{
		for (long queue_index = 0; queue_index < g_network_join_data.join_queue_entry_count; queue_index++)
		{
			if (join_request->join_nonce == g_network_join_data.join_queue[queue_index].join_nonce)
			{
				printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it is already being processed\n", transport_address_get_string(address));
				return;
			}
		}
	}

	// add entry to the queue
	s_networking_join_queue_entry& queue_entry = g_network_join_data.join_queue[g_network_join_data.join_queue_entry_count++];
	memset(&queue_entry, 0, sizeof(s_networking_join_queue_entry)); // clear entry
	queue_entry.address = *address;
	queue_entry.join_nonce = join_request->join_nonce;
	queue_entry.join_request = *join_request;
	queue_entry.time_inserted_into_the_queue = network_time_get();
	queue_entry.time_client_was_last_notified = -1;
	printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was added to the join queue\n", transport_address_get_string(address));

	// if the queue is now full (max 32), remove the entry that entered first to free up space
	ASSERT(entry_count < NUMBEROF(g_network_join_data.join_queue));
	if (entry_count == NUMBEROF(g_network_join_data.join_queue))
	{
		// i'm ignoring the desirability as HO no longer updates it, so it'll always be 0 and unaccounted for
		s_networking_join_queue_entry* entry_to_delete = &g_network_join_data.join_queue[0];
		ulong previous_entry_enter_time = entry_to_delete->time_inserted_into_the_queue;
		for (long queue_index = 1; queue_index < NUMBEROF(g_network_join_data.join_queue); queue_index++)
		{
			s_networking_join_queue_entry& queue_entry = g_network_join_data.join_queue[queue_index];
			// if the current queue entry was added first, mark it for deletion
			if (previous_entry_enter_time > queue_entry.time_inserted_into_the_queue)
			{
				entry_to_delete = &queue_entry;
			}
		}
		printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was removed from the join queue because the queue was full\n", transport_address_get_string(address));
		session->acknowledge_join_request(address, _network_join_refuse_reason_rejected_by_host);

		// replace entry to delete w/ last entry, and decrement queue entry count
		memcpy(entry_to_delete, &g_network_join_data.join_queue[--g_network_join_data.join_queue_entry_count], sizeof(s_networking_join_queue_entry));
	}
}

// This function still exists in ms29, but was hooked and rewritten to add back the missing c_network_session::join_accept call
bool network_join_process_joins_from_queue()
{
	c_network_session* session = nullptr;
	switch (g_network_join_data.join_queue_mode)
	{
		case _network_join_open_to_join_squad:
		{
			session = network_life_cycle_get_squad_session_for_join();
			break;
		}
		// Matchmaking groups removed since h3/ms23
		//case _network_join_process_queued_group_joins:
		//{
		//    session = network_life_cycle_get_squad_session_for_join();
		//    break;
		//}
		default:
			break;
	}

	if (!session)
	{
		return false;
	}

	bool bandwidth_is_stable = false;
	c_network_observer* observer = nullptr;
	network_life_cycle_get_observer(&observer);
	if (observer)
	{
		bandwidth_is_stable = observer->is_bandwidth_stable();
	}

	if (!session->established() || !session->is_host() || session->membership_is_locked())
	{
		if (g_network_join_data.join_queue_entry_count > 0)
		{
			network_join_flush_join_queue();
		}
		return false;
	}

	bool is_peer_joining = session->is_peer_joining_this_session();
	if (is_peer_joining || !bandwidth_is_stable)
	{
		if (g_network_join_data.join_queue_entry_count > 0 && !is_peer_joining)
		{
			printf("MP/NET/JOIN,CTRL: network_join_process_joins_from_queue: Warning. unabled to process join because our bandwidth is not stable (stable=%d)\n", bandwidth_is_stable);
		}
		return is_peer_joining;
	}

	if (g_network_join_data.join_queue_entry_count > 0)
	{
		s_networking_join_queue_entry& queue_entry = g_network_join_data.join_queue[0];

		if (game_is_dedicated_server())
		{
			// check if sessionIDs have been requested, if not request them
			// joining peers will be held in a queue until this data is received
			// if no data is received after 60 seconds the peer will be timed out

			// returning true updates network_join_queue_update, allowing peers to be held in the queue
			// returning false resets the join state

			if (c_backend::private_service::retrieve_lobby_members::m_status.status == _request_status_none)
			{
				if (!user_sessions_request_for_lobby())
				{
					// reset the join state if request fails
					return false;
				}
				// hold in queue until we have valid data from the request
				printf("MP/NET/JOIN,CTRL: network_join_process_joins_from_queue: requesting user sessions, holding join [%s] in queue\n",
					transport_secure_nonce_get_string(queue_entry.join_nonce));
				return true;
			}
			else if (c_backend::private_service::retrieve_lobby_members::m_status.status == _request_status_received)
			{
				c_backend::private_service::retrieve_lobby_members::m_status.status = _request_status_none;

				// reset join if session data returned invalid, otherwise continue to join accept
				if (!g_backend_data_cache.lobby_session.valid)
				{
					printf("MP/NET/JOIN,CTRL: network_join_process_joins_from_queue: session data returned invalid!\n");
					return false;
				}
			}
			else
			{
				// continue to hold in queue
				return true;
			}
		}

		// Accept first entry in the queue
		session->join_accept(&queue_entry.join_request, &queue_entry.address);
		// Move queue up
		memcpy(&queue_entry, &g_network_join_data.join_queue[--g_network_join_data.join_queue_entry_count], sizeof(s_networking_join_queue_entry));
		return true;
	}
	return is_peer_joining;
}

void __cdecl network_join_flush_join_queue()
{
	INVOKE(0x2A640, network_join_flush_join_queue);
}

void network_join_remove_join_from_queue(qword join_nonce)
{
	for (long queue_index = 0; queue_index < g_network_join_data.join_queue_entry_count;)
	{
		s_networking_join_queue_entry& queue_entry = g_network_join_data.join_queue[queue_index];
		if (queue_entry.join_nonce != join_nonce)
		{
			queue_index++;
			continue;
		}

		printf("MP/NET/JOIN,CTRL: network_join_remove_join_from_queue: %s was removed from the join queue by request\n", transport_address_get_string(&queue_entry.address));
		// overwrite queue entry with last entry & decrease entry count
		memcpy(&queue_entry, &g_network_join_data.join_queue[--g_network_join_data.join_queue_entry_count], sizeof(s_networking_join_queue_entry));
		// do not increment counter after removal
	}
}