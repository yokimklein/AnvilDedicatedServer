#include "network_session_interface.h"
#include <networking\session\network_session.h>
#include <text\unicode.h>
#include <networking\network_time.h>
#include <iostream>
#include <networking\logic\network_life_cycle.h>
#include <anvil\server_tools.h>

void network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer)
{
    ulong(__cdecl* game_language_get_default)() = reinterpret_cast<decltype(game_language_get_default)>(base_address(0xB0C00));
    void(__fastcall* network_session_get_connectivity)(c_network_session* session, s_network_session_peer_connectivity* peer_connectivity) = reinterpret_cast<decltype(network_session_get_connectivity)>(base_address(0x2E500));
    ulong(__cdecl* build_peer_mp_map_mask)() = reinterpret_cast<decltype(build_peer_mp_map_mask)>(base_address(0xDD750));

    ulong peer_properties_update_timestamp = ((ulong*)base_address(0x3EB1498))[session->session_index()];
    ulong time_since_last_update = network_time_since(peer_properties_update_timestamp);

    // wait for network_configuration peer properties update rate
    if (time_since_last_update < 0 || time_since_last_update >= *(ulong*)base_address(0x1038760))
    {
        // network_life_cycle_get_observer
        c_network_observer* observer = nullptr;
        network_life_cycle_get_observer(&observer);
        s_network_session_peer_properties peer_properties = {};
        ustrnzcpy(peer_properties.peer_name, k_anvil_machine_name, 16); // (wchar_t*)base_address(0x3EAE0C4)
        ustrnzcpy(peer_properties.peer_session_name, k_anvil_session_name, 32); // (wchar_t*)base_address(0x3EAE0E4)
        peer_properties.language = game_language_get_default();
        network_session_get_connectivity(session, &peer_properties.connectivity);
        observer->quality_statistics_get_ratings(&peer_properties.connectivity_badness_rating, &peer_properties.host_badness_rating, &peer_properties.client_badness_rating);
        peer_properties.peer_mp_map_mask = build_peer_mp_map_mask();
        peer_properties.peer_map = *((ulong*)base_address(0x3EAE12C));
        peer_properties.game_start_error = *((ulong*)base_address(0x3EAE128));
        peer_properties.peer_map_status = *((e_peer_map_status*)base_address(0x3EAE130));
        peer_properties.peer_map_progress_percentage = *((ulong*)base_address(0x3EAE134));
        peer_properties.peer_game_instance = *((qword*)base_address(0x3EB10D8));
        peer_properties.determinism_version = *((ulong*)base_address(0x1039AC8));
        peer_properties.determinism_compatible_version = *((ulong*)base_address(0x1039ACC));
        peer_properties.flags = *((ulong*)base_address(0x3EAE124));

        s_transport_secure_address secure_address = {};
        s_transport_secure_address* peer_secure_address;
        c_network_session_membership* membership = session->get_session_membership_unsafe();
        if (!transport_secure_address_get(&secure_address))
            transport_secure_address_build_from_identifier(transport_unique_identifier_get(), &secure_address);
        if (membership != nullptr && membership->has_membership())
            peer_secure_address = membership->get_peer_address(membership->local_peer_index());
        else
            peer_secure_address = &secure_address;

        if (peer->connection_state == _network_session_peer_state_joined
            || csmemcmp(&secure_address, peer_secure_address, sizeof(s_transport_secure_address)) // not equal
            || (csmemcmp(&peer->properties, &peer_properties, sizeof(s_network_session_peer_properties))) != 0) // equal
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: requesting peer-properties update (machine:%ls session-name:%ls map:%d), after %dms\n",
                peer_properties.peer_name,
                peer_properties.peer_session_name,
                peer_properties.peer_map_status,
                time_since_last_update);
            if (session->peer_request_properties_update(&secure_address, &peer_properties))
                ((ulong*)base_address(0x3EB1498))[session->session_index()] = network_time_get();
            else
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: unable to send peer-properties update\n");
        }
    }
}

// TODO: set player name, loadouts, teams and vote selection here
// if dedicated_server_session_state == voting, and new selection index != old, increment vote count for new choice and decrement for old (so long as old isn't 0)
// make sure to sanitise vote inputs, if players set to an invalid index/enum, update their player data to set it back to 0 and remove their vote
void network_session_check_properties(c_network_session* session)
{
    /* TODO - once mulg definition is implemented & tag interface
    bool mulg_is_valid = false;
    if (g_globals_definition)
    {
        tag_index mulg_index = g_globals_definition->multiplayer_globals.tag_index;
        if (mulg_index != -1)
        {
            s_multiplayer_globals_definition* multiplayer_globals = (s_multiplayer_globals_definition*)tag_get('mulg', mulg_index);
            if (multiplayer_globals)
                mulg_is_valid = multiplayer_globals->universal.count > 0;
        }
    }
    */
    /*
    bool mulg_is_valid = true;

    if (!session->membership_is_locked() && mulg_is_valid)
    {
        c_network_session_parameters* parameters = session->get_session_parameters();
        long session_index = session->session_index();
        c_game_variant* game_variant = nullptr;
        if (parameters->game_variant.get_allowed())
            game_variant = parameters->game_variant.get();
        long update_number = session->get_session_membership_update_number();
    }
    */
}