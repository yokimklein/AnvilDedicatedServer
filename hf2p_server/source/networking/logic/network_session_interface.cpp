#include "network_session_interface.h"
#include "..\session\network_session.h"
#include <iostream>

void network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer)
{
    ulong(__cdecl * system_default_ui_language_to_game_language)() = reinterpret_cast<decltype(system_default_ui_language_to_game_language)>(module_base + 0xB0C00);
    void(__fastcall * network_session_peer_connectivity_unknown)(c_network_session * session, s_network_session_peer_connectivity* peer_connectivity) = reinterpret_cast<decltype(network_session_peer_connectivity_unknown)>(module_base + 0x2E500);
    ulong(__cdecl * build_peer_mp_map_mask)() = reinterpret_cast<decltype(build_peer_mp_map_mask)>(module_base + 0xDD750);

    ulong unknown_time = ((ulong*)module_base + 0x3EB1498)[session->m_session_index];
    ulong current_time = timeGetTime();
    if (*(bool*)(module_base + 0x1038344)) // life cycle session m_time_exists?
        current_time = *(ulong*)(module_base + 0x1038348); // m_time?
    ulong network_time_since = current_time - unknown_time;
    if (network_time_since < 0 || network_time_since >= *(ulong*)(module_base + 0x1038760))
    {
        // network_life_cycle_get_observer
        c_network_observer* observer = nullptr;
        if (*(bool*)(module_base + 0x3EADFA8))
            observer = (c_network_observer*)(module_base + 0x3EADFD8);

        s_network_session_peer_properties* peer_properties = new s_network_session_peer_properties();
        memcpy(&peer_properties->peer_name, (wchar_t*)(module_base + 0x3EAE0C2), 32);
        memcpy(&peer_properties->peer_session_name, (wchar_t*)(module_base + 0x3EAE0E2), 64);
        peer_properties->language = system_default_ui_language_to_game_language();
        network_session_peer_connectivity_unknown(session, &peer_properties->connectivity); // get connectivity
        observer->quality_statistics_get_ratings(&peer_properties->connectivity_badness_rating, &peer_properties->host_badness_rating, &peer_properties->client_badness_rating);
        peer_properties->peer_mp_map_mask = build_peer_mp_map_mask();
        peer_properties->peer_map = *((ulong*)(module_base + 0x3EAE12C));
        peer_properties->game_start_error = *((ulong*)(module_base + 0x3EAE128));
        peer_properties->peer_map_status = *((e_peer_map_status*)(module_base + 0x3EAE130));
        peer_properties->peer_map_progress_percentage = *((ulong*)(module_base + 0x3EAE134));
        peer_properties->peer_game_instance = *((long64*)(module_base + 0x3EB10D8));
        peer_properties->determinism_version = *((ulong*)(module_base + 0x1039AC8));
        peer_properties->determinism_compatible_version = *((ulong*)(module_base + 0x1039ACC));
        peer_properties->flags = *((ulong*)(module_base + 0x3EAE124));

        c_network_session_membership* membership = session->get_session_membership_unsafe();
        s_transport_secure_address* secure_address = new s_transport_secure_address();
        if (!transport_secure_address_get(secure_address))
        {
            const s_transport_unique_identifier* machine_identifier = transport_unique_identifier_get();
            transport_secure_address_build_from_identifier(machine_identifier, secure_address);
        }
        s_transport_secure_address* peer_secure_address;
        if (!membership || membership->update_number() == -1)
            peer_secure_address = secure_address;
        else
            peer_secure_address = membership->get_peer_address(membership->local_peer_index());
        if (peer->connection_state == _network_session_peer_state_joined
            || memcmp(secure_address, peer_secure_address, sizeof(s_transport_secure_address)) // equal
            || (memcmp(&peer->properties, peer_properties, sizeof(s_network_session_peer_properties))) != 0) // not equal
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: requesting peer-properties update (machine:%ls session-name:%ls map:%d), after %dms\n",
                peer_properties->peer_name,
                peer_properties->peer_session_name,
                peer_properties->peer_map_status,
                network_time_since);
            if (session->peer_request_properties_update(secure_address, peer_properties))
            {
                if (*(bool*)(module_base + 0x1038344))
                    ((ulong*)module_base + 0x3EB1498)[session->session_index()] = *(ulong*)(module_base + 0x1038348);
                else
                    ((ulong*)module_base + 0x3EB1498)[session->session_index()] = timeGetTime();
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: unable to send peer-properties update\n");
            }
        }

        delete peer_properties;
        delete secure_address;
    }
}

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