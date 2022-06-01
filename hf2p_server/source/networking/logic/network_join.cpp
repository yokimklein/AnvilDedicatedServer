#include "network_join.h"
#include "..\messages\network_message_type_collection.h"
#include "..\session\network_session.h"
#include <iostream>
#include "..\network_utilities.h"

void network_join_add_join_to_queue(c_network_session* session, s_transport_address const* address, s_network_session_join_request const* join_request)
{
    // init
    s_networking_join_data* data = g_network_join_data;
    e_network_join_mode join_status = data->join_queue_mode;
    e_network_join_refuse_reason closure_reason = session->get_closure_reason();

    // make sure join isn't closed
    if (closure_reason != _network_join_refuse_reason_none)
    {
        session->acknowledge_join_request(address, closure_reason);
        return;
    }

    // check join status
    switch (join_status)
    {
    case _network_join_closed_to_all_joins:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: attempted to join while in _network_join_closed_to_all_joins\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_not_joinable);
        return;
    case _network_join_open_to_join_squad:
        if (session->m_session_type != _network_session_type_group)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a group session while joining is in _network_join_open_to_join_squad\n");
            session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
            return;
        }
        break;
    case _network_join_queue_joins_to_group:
    case _network_join_process_queued_group_joins:
        if (session->m_session_type != _network_session_type_squad)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join a squad session while in matchmaking\n");
            session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
            return;
        }
        break;
    case _network_join_queue_closed_while_in_match:
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: Warning. Attempted to join while in matchmaking\n");
        session->acknowledge_join_request(address, _network_join_refuse_reason_in_matchmaking);
        return;
    }

    // ensure join request isn't already a session member
    long peer_index = session->m_session_membership.get_first_peer();
    while (peer_index != -1)
    {
        if (join_request->join_nonce == session->m_session_membership.m_baseline.peers[peer_index].join_nonce)
        {
            printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it already exists in the join queue\n", transport_address_get_string(address));
            return;
        }
        peer_index = session->m_session_membership.get_next_peer(peer_index);
    }

    // make sure the request entry isn't already in the queue (if the queue has entries)
    if (g_network_join_data->join_queue_entry_count != 0)
    {
        for (long i = 0; i < g_network_join_data->join_queue_entry_count; i++)
        {
            if (join_request->join_nonce == g_network_join_data->join_queue[i].join_nonce)
            {
                printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was ignored because it is already being processed\n", transport_address_get_string(address));
                return;
            }
        }
    }

    // add entry to the queue
    auto queue_entry = &g_network_join_data->join_queue[g_network_join_data->join_queue_entry_count];
    memset(queue_entry, 0, sizeof(s_join_queue_entry)); // clear entry
    queue_entry->address = *address;
    queue_entry->join_nonce = join_request->join_nonce;
    queue_entry->join_request = *join_request;
    queue_entry->times[0] = network_get_time();
    queue_entry->times[1] = -1;
    printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was added to the join queue\n", transport_address_get_string(address));
    g_network_join_data->join_queue_entry_count++;

    // if the queue is now full (max 32), remove the entry that entered first to free up space
    if (g_network_join_data->join_queue_entry_count == 32)
    {
        // i'm ignoring the latency field here (time[2]) as HO no longer updates it, so it'll always be 0 and unaccounted for
        auto entry_to_delete = &g_network_join_data->join_queue[0];
        auto previous_entry_enter_time = g_network_join_data->join_queue[0].times[0];
        for (size_t i = 1; i < 32; i++)
        {
            // if the current queue entry was added first, mark it for deletion
            if (previous_entry_enter_time > g_network_join_data->join_queue[i].times[0])
                entry_to_delete = &g_network_join_data->join_queue[i];
        }
        printf("MP/NET/JOIN,CTRL: network_join_add_join_to_queue: %s was removed from the join queue because the queue was full\n", transport_address_get_string(address));
        session->acknowledge_join_request(address, _network_join_refuse_reason_rejected_by_host);

        // replace entry to delete w/ last entry, and decrement queue entry count
        memcpy(entry_to_delete, &g_network_join_data->join_queue[--g_network_join_data->join_queue_entry_count], sizeof(s_join_queue_entry));
    }
}

// WIP - needs testing
// this still exists in ms29, but we need to hook this function to add back the missing c_network_session::join_accept call
bool network_join_process_joins_from_queue() // runs every tick the session is open & has queue entries
{
    bool is_success = false;
    c_network_session* session = nullptr;
    bool life_cycle_initialised = *(bool*)(module_base + 0x3EADFA8);
    bool bandwidth_is_stable = false;
    void* life_cycle_unknown = (void*)(module_base + 0x3EADFD8);

    if (g_network_join_data->join_queue_mode == _network_join_open_to_join_squad && life_cycle_initialised) // c_network_life_cycle::get_session()
        session = (c_network_session*)(module_base + 0x3970168/*0x3EADFD0*/); // life_cycle_session
    if (life_cycle_initialised && life_cycle_unknown)
        bandwidth_is_stable = *(bool*)((long)life_cycle_unknown + 0x23748) == 0; // network_life_cycle_get_squad_session_for_join

    if (session == nullptr)
        return false;
    if (session->m_local_state <= _network_session_state_peer_join_abort
        || session->m_local_state != _network_session_state_host_established
        && session->m_local_state != _network_session_state_host_disband)
    {
        if (g_network_join_data->join_queue_entry_count)
            network_join_flush_join_queue();
        return false;
    }
    if (session->m_local_state == _network_session_state_host_disband)
        return false;

    bool is_peer_joining = session->is_peer_joining_this_session();
    is_success = is_peer_joining;
    if (is_peer_joining && !bandwidth_is_stable)
    {
        if (g_network_join_data->join_queue_entry_count && !is_peer_joining)
            printf("MP/NET/JOIN,CTRL: network_join_process_joins_from_queue: Warning. unabled to process join because our bandwidth is not stable (stable=%d)\n", bandwidth_is_stable);
        return is_success;
    }

    if (g_network_join_data->join_queue_entry_count != 0)
    {
        s_join_queue_entry* queue_entry = &g_network_join_data->join_queue[0];
        session->join_accept(&queue_entry->join_request, &queue_entry->address);
        memcpy(queue_entry, &g_network_join_data->join_queue[--g_network_join_data->join_queue_entry_count], sizeof(s_join_queue_entry));
        return true;
    }
    return is_success;
}

void network_join_flush_join_queue()
{
    typedef void(__cdecl* network_join_flush_join_queue_ptr)();
    auto network_join_flush_join_queue_call = reinterpret_cast<network_join_flush_join_queue_ptr>(module_base + 0x2A640);

    network_join_flush_join_queue_call();
}

void network_join_remove_join_from_queue(uint64_t join_nonce)
{
    long* entry_count = &g_network_join_data->join_queue_entry_count;
    for (long i = 0; i < *entry_count; i++)
    {
        auto queue_entry = &g_network_join_data->join_queue[i];
        if (queue_entry->join_nonce == join_nonce)
        {
            printf("MP/NET/JOIN,CTRL: network_join_remove_join_from_queue: %s was removed from the join queue by request\n",
                transport_address_get_string(&queue_entry->address));
            // overwrite queue entry with last entry & decrease entry count
            memcpy(queue_entry, &g_network_join_data->join_queue[--*entry_count], sizeof(s_join_queue_entry));
            // do not increment counter
            i--;
        }
    }
}