#include "network_session_interface.h"
#include "..\session\network_session.h"
#include <iostream>

void network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer)
{
    uint32_t(__cdecl * system_default_ui_language_to_game_language)() = reinterpret_cast<decltype(system_default_ui_language_to_game_language)>(module_base + 0xB0C00); // TODO TEST
    void(__fastcall * network_session_peer_connectivity_unknown)(c_network_session * session, uint16_t * peer_connectivity_mask) = reinterpret_cast<decltype(network_session_peer_connectivity_unknown)>(module_base + 0x2E500); // TODO TEST
    uint32_t(__cdecl * build_peer_mp_map_mask)() = reinterpret_cast<decltype(build_peer_mp_map_mask)>(module_base + 0xDD750); // TODO TEST

    uint32_t unknown_time = ((uint32_t*)module_base + 0x3EB1498)[session->m_session_index];
    uint32_t current_time = timeGetTime();
    if (*(bool*)(module_base + 0x1038344)) // life cycle session m_time_exists?
        current_time = *(uint32_t*)(module_base + 0x1038348); // m_time?
    uint32_t network_time_since = current_time - unknown_time;
    if (network_time_since < 0 || network_time_since >= *(uint32_t*)(module_base + 0x1038760))
    {
        // network_life_cycle_get_observer
        c_network_observer* observer = nullptr;
        if (*(bool*)(module_base + 0x3EADFA8))
            observer = (c_network_observer*)(module_base + 0x3EADFD8);

        s_network_session_peer_properties peer_properties;
        memset(&peer_properties, 0, sizeof(s_network_session_peer_properties));
        memcpy(&peer_properties.peer_name, (wchar_t*)(module_base + 0x3EAE0C2), 32);
        memcpy(&peer_properties.peer_session_name, (wchar_t*)(module_base + 0x3EAE0E2), 64);
        peer_properties.language = system_default_ui_language_to_game_language();
        network_session_peer_connectivity_unknown(session, &peer_properties.peer_connectivity_mask);
        observer->quality_statistics_get_ratings(&peer_properties.connectivity_badness_rating, &peer_properties.host_badness_rating, &peer_properties.client_badness_rating);
        peer_properties.peer_mp_map_mask = build_peer_mp_map_mask();
        peer_properties.peer_map = *((uint32_t*)(module_base + 0x3EAE12C));
        peer_properties.game_start_error = *((uint32_t*)(module_base + 0x3EAE128));
        peer_properties.peer_map_status = *((e_peer_map_status*)(module_base + 0x3EAE130));
        peer_properties.peer_map_progress_percentage = *((uint32_t*)(module_base + 0x3EAE134));
        peer_properties.peer_game_instance = *((int64_t*)(module_base + 0x3EB10D8));
        peer_properties.determinism_version = *((uint32_t*)(module_base + 0x1039AC8));
        peer_properties.determinism_compatible_version = *((uint32_t*)(module_base + 0x1039ACC));
        peer_properties.flags = *((uint32_t*)(module_base + 0x3EAE124));

        c_network_session_membership* membership = session->get_session_membership_unsafe();
        s_transport_secure_address secure_address;
        if (!transport_secure_address_get(&secure_address))
        {
            const s_transport_unique_identifier* machine_identifier = transport_unique_identifier_get();
            transport_secure_address_build_from_identifier(machine_identifier, &secure_address);
        }
        s_transport_secure_address* peer_secure_address;
        if (!membership || membership->m_baseline.update_number == -1)
            peer_secure_address = &secure_address;
        else
            peer_secure_address = &membership->m_baseline.peers[membership->m_local_peer_index].secure_address;
        if (peer->connection_state == _network_session_peer_state_joined
            || memcmp(&secure_address, peer_secure_address, sizeof(s_transport_secure_address))
            || (memcmp(&peer->properties, &peer_properties, sizeof(s_network_session_peer_properties))) != 0)
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: requesting peer-properties update (machine:%ls session-name:%ls map:%d), after %dms\n",
                peer_properties.peer_name,
                peer_properties.peer_session_name,
                peer_properties.peer_map_status,
                network_time_since);
            if (session->peer_request_properties_update(&secure_address, &peer_properties))
            {
                if (*(bool*)(module_base + 0x1038344))
                    ((uint32_t*)module_base + 0x3EB1498)[session->m_session_index] = *(uint32_t*)(module_base + 0x1038348);
                else
                    ((uint32_t*)module_base + 0x3EB1498)[session->m_session_index] = timeGetTime();
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: network_session_update_peer_properties: unable to send peer-properties update\n");
            }
        }
    }
}