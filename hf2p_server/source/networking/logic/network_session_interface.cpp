#include "network_session_interface.h"
#include <networking\session\network_session.h>
#include <text\unicode.h>
#include <networking\network_time.h>
#include <iostream>
#include <networking\logic\network_life_cycle.h>
#include <anvil\server_tools.h>
#include <cache\cache_files.h>
#include <game\game_globals.h>
#include <game\multiplayer_definitions.h>
#include <main\main.h>
#include <game\game_engine_team.h>
#include <cseries\language.h>
#include <main\levels.h>
#include <networking\network_configuration.h>
#include <game\game.h>
#include <networking\network_globals.h>

REFERENCE_DECLARE(0x3EAE0C0, s_network_session_interface_globals, session_interface_globals);

void __fastcall network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer)
{
    ulong session_update_time = session_interface_globals.session_update_times[session->session_index()];
    ulong time_since_last_update = network_time_since(session_update_time);

    // wait for network_configuration peer properties update rate
    if (time_since_last_update < 0 || time_since_last_update >= get_network_configuration()->peer_properties_update_timeout)
    {
        c_network_observer* observer = nullptr;
        network_life_cycle_get_observer(&observer);

        s_network_session_peer_properties peer_properties = {};
        peer_properties.peer_name = session_interface_globals.machine_name;
        peer_properties.peer_session_name = session_interface_globals.session_name;
        peer_properties.language = get_current_language();
        network_session_calculate_peer_connectivity(session, &peer_properties.connectivity);
        observer->quality_statistics_get_ratings(&peer_properties.connectivity_badness_rating, &peer_properties.host_badness_rating, &peer_properties.client_badness_rating);
        peer_properties.peer_mp_map_mask = levels_get_available_map_mask();
        peer_properties.peer_map = session_interface_globals.map_id;
        peer_properties.game_start_error = session_interface_globals.game_start_error;
        peer_properties.peer_map_status = session_interface_globals.current_map;
        peer_properties.peer_map_progress_percentage = session_interface_globals.current_map_progress_percentage;
        peer_properties.peer_game_instance = session_interface_globals.game_instance;
        game_get_determinism_versions(&peer_properties.determinism_version, &peer_properties.determinism_compatible_version);
        peer_properties.flags = session_interface_globals.peer_status_flags;

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
                peer_properties.peer_name.get_string(),
                peer_properties.peer_session_name.get_string(),
                peer_properties.peer_map_status.get(),
                time_since_last_update);
            if (session->peer_request_properties_update(&secure_address, &peer_properties))
                session_interface_globals.session_update_times[session->session_index()] = network_time_get();
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
    assert(session != nullptr);
    assert(session->is_host());

    bool mulg_is_valid = false;
    if (global_game_globals)
    {
        long mulg_index = global_game_globals->multiplayer_globals.index;
        if (mulg_index != -1)
        {
            s_multiplayer_globals_definition* multiplayer_globals = (s_multiplayer_globals_definition*)tag_get(MULTIPLAYER_GLOBALS_TAG, mulg_index);
            if (multiplayer_globals)
                mulg_is_valid = multiplayer_globals->universal.count() > 0;
        }
    }

    if (!session->membership_is_locked() && !main_is_in_main_loop_pregame() && mulg_is_valid)
    {
        long maximum_team_count = 8;
        bool sve_teams = false;
        bool variant_has_teams = false;
        bool observer_allowed = true;
        bool update_required = false;

        c_network_session_parameters* parameters = session->get_session_parameters();
        c_game_variant* game_variant = nullptr;
        if (parameters->game_variant.get_allowed())
            game_variant = parameters->game_variant.get();
        long update_number = session->get_session_membership_update_number();
        long session_index = session->session_index();

        if (session_interface_globals.session_connection_update_numbers[session_index] != update_number)
        {
            c_network_session_membership* membership = session->get_session_membership_for_update();
            assert(membership != NULL);
            update_required = true;
            bool player_data_updated = false;

            // TODO: NAME VERIFICATION? we might not need it so long as the API sanitises everything

            // assign player loadouts
            for (long player_index = membership->get_first_player(); player_index != -1; player_index = membership->get_next_player(player_index))
            {
                s_network_session_player* player = membership->get_player(player_index);
                player_data_updated = anvil_assign_player_loadout(session, player_index, &player->configuration.host);
            }

            if (player_data_updated)
                membership->player_data_updated();
            session_interface_globals.session_connection_update_numbers[session_index] = session->get_session_membership_update_number();
        }

        if (session->session_type() == _network_session_type_squad)
        {
            if (game_variant != nullptr)
            {
                variant_has_teams = game_engine_variant_has_teams(game_variant);
                sve_teams = game_variant->get_active_variant()->get_social_options()->get_spartans_vs_elites_enabled();
                observer_allowed = game_engine_variant_is_observer_allowed(game_variant);
                if (session->get_session_parameters()->map.get_allowed())
                {
                    s_network_session_parameter_map* map = session->get_session_parameters()->map.get();
                    assert(map != nullptr);
                    maximum_team_count = game_engine_variant_get_maximum_team_count(game_variant, map->map_id);
                }
            }
            if ((maximum_team_count || !variant_has_teams) && (update_required ||
                session_interface_globals.session_variant_has_teams[session_index] != variant_has_teams ||
                session_interface_globals.session_variant_has_sve_teams[session_index] != sve_teams ||
                session_interface_globals.session_variant_observers_allowed[session_index] != observer_allowed ||
                session_interface_globals.session_variant_maximum_team_count[session_index] != maximum_team_count))
            {
                network_session_update_team_indices(session, variant_has_teams, session_interface_globals.session_variant_has_teams[session_index], observer_allowed, sve_teams, maximum_team_count);
                session_interface_globals.session_variant_has_teams[session_index] = variant_has_teams;
                session_interface_globals.session_variant_has_sve_teams[session_index] = sve_teams;
                session_interface_globals.session_variant_observers_allowed[session_index] = observer_allowed;
                session_interface_globals.session_variant_maximum_team_count[session_index] = maximum_team_count;
            }
        }
    }
}

void network_session_update_team_indices(c_network_session* session, bool variant_has_teams, bool previous_update_variant_had_teams, bool observer_allowed, bool sve_teams, long maximum_team_count)
{
    c_network_session_membership* membership = session->get_session_membership_for_update();
    bool teams_updated = false;
    if (variant_has_teams)
    {
        // In ms23 there's a dedicated server check here which would override team sorting
        // in the client build we have, the function used to override is likely stubbed as it only returns 0
        // I'm guessing saber had some sort of rank or skill based team sorting system? we're probably not gonna go for that right now

        // spartans vs elites team sorting
        if (sve_teams)
        {
            for (long player_index = membership->get_first_player(); player_index != -1; player_index = membership->get_next_player(player_index))
            {
                s_network_session_player* player = membership->get_player(player_index);
                player->configuration.host.user_selected_team_index = player->configuration.host.player_appearance.player_model_choice != 0;
            }
        }
        else
        {
            // TODO: proper team sorting
            for (long player_index = membership->get_first_player(); player_index != -1; player_index = membership->get_next_player(player_index))
            {
                s_network_session_player* player = membership->get_player(player_index);
                player->configuration.host.user_selected_team_index = player_index % 2;
            }
        }
        // assign user selected team to team_index
        for (long player_index = membership->get_first_player(); player_index != -1; player_index = membership->get_next_player(player_index))
        {
            s_network_session_player* player = membership->get_player(player_index);
            if (player->configuration.host.team_index != player->configuration.host.user_selected_team_index)
            {
                player->configuration.host.team_index = player->configuration.host.user_selected_team_index;
                teams_updated = true;
            }
        }
    }
    else
    {
        // FFA team sorting
        for (long player_index = membership->get_first_player(); player_index != -1; player_index = membership->get_next_player(player_index))
        {
            s_network_session_player* player = membership->get_player(player_index);
            if (player_index != player->configuration.host.team_index.get())
            {
                teams_updated = true;
                player->configuration.host.team_index = player_index;
            }
        }
    }
    if (teams_updated)
        membership->player_data_updated();
}

void __fastcall network_session_interface_update_session(c_network_session* session)
{
    INVOKE(0x2F410, network_session_interface_update_session, session);
}

bool __fastcall network_session_interface_get_local_user_identifier(s_player_identifier* player_identifier)
{
    return INVOKE(0x3D50, network_session_interface_get_local_user_identifier, player_identifier);
}

void __fastcall network_session_calculate_peer_connectivity(c_network_session* session, s_network_session_peer_connectivity* peer_connectivity)
{
    INVOKE(0x2E500, network_session_calculate_peer_connectivity, session, peer_connectivity);
}

void network_session_interface_set_local_name(wchar_t const* machine_name, wchar_t const* session_name)
{
    session_interface_globals.machine_name.set(machine_name);
    session_interface_globals.session_name.set(session_name);
}

bool network_session_interface_get_squad_session(c_network_session** out_session)
{
    if (network_life_cycle_in_squad_session(out_session))
    {
        return true;
    }
    return false;
}

void __cdecl network_session_interface_update()
{
    INVOKE(0x2DC50, network_session_interface_update);
}