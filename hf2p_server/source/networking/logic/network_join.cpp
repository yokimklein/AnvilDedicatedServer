#include "network_join.h"
#include <iostream>
#include <networking\network_utilities.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\session\network_session.h>
#include <networking\logic\network_life_cycle.h>

REFERENCE_DECLARE(0x1039AFC, s_networking_join_data, g_network_join_data);

void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request)
{
    s_networking_join_data* data = &g_network_join_data;
    long entry_count = g_network_join_data.join_queue_entry_count;

    // make sure join isn't closed
    e_network_join_refuse_reason closure_reason = session->get_closure_reason();
    if (closure_reason != _network_join_refuse_reason_none)
    {
        session->acknowledge_join_request(address, closure_reason);
        return;
    }

    // check join status
    switch (data->join_queue_mode.get())
    {
    case _network_join_closed_to_all_joins:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: attempted to join while in _network_join_closed_to_all_joins\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_not_joinable);
        return;
    case _network_join_open_to_join_squad:
        if (session->session_type() != _network_session_type_squad)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a group session while joining is in _network_join_open_to_join_squad\n");
            session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
            return;
        }
        break;
    // Removed since ms23
    //case _network_join_queue_joins_to_group:
    //case _network_join_process_queued_group_joins:
    //    if (session->session_type() != _network_session_type_group)
    //    {
    //        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a squad session while in matchmaking\n");
    //        session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
    //        return;
    //    }
    //    break;
    case _network_join_queue_closed_while_in_match:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join while in matchmaking\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
        return;
    }

    // ensure join request isn't already a session member
    for (long i = session->get_session_membership()->get_first_peer(); i != -1; i = session->get_session_membership()->get_next_peer(i))
    {
        if (join_request->join_nonce == session->get_session_membership()->get_join_nonce(i))
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it already exists in the join queue\n", transport_address_get_string(address));
            return;
        }
    }

    // make sure the request entry isn't already in the queue (if the queue has entries)
    if (data->join_queue_entry_count != 0)
    {
        for (long i = 0; i < data->join_queue_entry_count; i++)
        {
            if (join_request->join_nonce == data->join_queue[i].join_nonce)
            {
                printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it is already being processed\n", transport_address_get_string(address));
                return;
            }
        }
    }

    // add entry to the queue
    s_join_queue_entry& queue_entry = data->join_queue[data->join_queue_entry_count++];
    memset(&queue_entry, 0, sizeof(s_join_queue_entry)); // clear entry
    queue_entry.address = *address;
    queue_entry.join_nonce = join_request->join_nonce;
    queue_entry.join_request = *join_request;
    queue_entry.times[0] = network_get_time();
    queue_entry.times[1] = -1;
    printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was added to the join queue\n", transport_address_get_string(address));

    // if the queue is now full (max 32), remove the entry that entered first to free up space
    assert(entry_count < NUMBEROF(data->join_queue));
    if (entry_count == NUMBEROF(data->join_queue))
    {
        // i'm ignoring the latency field here (time[2]) as HO no longer updates it, so it'll always be 0 and unaccounted for
        s_join_queue_entry* entry_to_delete = &data->join_queue[0];
        ulong previous_entry_enter_time = entry_to_delete->times[0];
        for (size_t i = 1; i < NUMBEROF(data->join_queue); i++)
        {
            // if the current queue entry was added first, mark it for deletion
            if (previous_entry_enter_time > data->join_queue[i].times[0])
                entry_to_delete = &data->join_queue[i];
        }
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was removed from the join queue because the queue was full\n", transport_address_get_string(address));
        session->acknowledge_join_request(address, _network_join_refuse_reason_rejected_by_host);

        // replace entry to delete w/ last entry, and decrement queue entry count
        memcpy(entry_to_delete, &data->join_queue[--data->join_queue_entry_count], sizeof(s_join_queue_entry));
    }
}

// This function still exists in ms29, but was hooked and rewritten to add back the missing c_network_session::join_accept call
bool network_join_process_joins_from_queue()
{
    c_network_session* session = nullptr;
    switch (g_network_join_data.join_queue_mode.get())
    {
        case _network_join_open_to_join_squad:
            session = network_life_cycle_get_squad_session_for_join();
            break;
        // Matchmaking groups removed since h3/ms23
        //case _network_join_process_queued_group_joins:
        //    session = network_life_cycle_get_squad_session_for_join();
        //    break;
        default:
            break;
    }
    if (session == nullptr)
        return false;

    bool bandwidth_is_stable = false;
    c_network_observer* observer = nullptr;
    network_life_cycle_get_observer(&observer);
    if (observer != nullptr)
        bandwidth_is_stable = observer->is_bandwidth_stable();

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
            printf("MP/NET/JOIN,CTRL: network_join_process_joins_from_queue: Warning. unabled to process join because our bandwidth is not stable (stable=%d)\n", bandwidth_is_stable);
        return is_peer_joining;
    }

    if (g_network_join_data.join_queue_entry_count > 0)
    {
        // Accept first entry in the queue
        s_join_queue_entry* queue_entry = &g_network_join_data.join_queue[0];
        session->join_accept(&queue_entry->join_request, &queue_entry->address);
        // Move queue up
        memcpy(queue_entry, &g_network_join_data.join_queue[--g_network_join_data.join_queue_entry_count], sizeof(s_join_queue_entry));
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
    for (long i = 0; i < g_network_join_data.join_queue_entry_count; i++)
    {
        s_join_queue_entry* queue_entry = &g_network_join_data.join_queue[i];
        if (queue_entry->join_nonce == join_nonce)
        {
            printf("MP/NET/JOIN,CTRL: network_join_remove_join_from_queue: %s was removed from the join queue by request\n",
                transport_address_get_string(&queue_entry->address));
            // overwrite queue entry with last entry & decrease entry count
            memcpy(queue_entry, &g_network_join_data.join_queue[--g_network_join_data.join_queue_entry_count], sizeof(s_join_queue_entry));
            // do not increment counter
            i--;
        }
    }
}