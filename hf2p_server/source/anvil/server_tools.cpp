#include "server_tools.h"
#include <iostream>
#include <hf2p\hf2p_session.h>
#include <interface\user_interface_session.h>
#include <networking\session\network_session.h>
#include <text\unicode.h>
#include <hf2p\hq.h>
#include <memory\tls.h>
#include <game\game.h>
#include <networking\network_utilities.h>
#include <networking\session\network_managed_session.h>
#include <anvil\build_version.h>
#include <anvil\hooks.h>

void enable_memory_write(dword base)
{
    // Enable write to all executable memory
    size_t Offset, Total;
    Offset = Total = 0;
    MEMORY_BASIC_INFORMATION MemInfo;

    while (VirtualQuery((byte*)base + Offset, &MemInfo, sizeof(MEMORY_BASIC_INFORMATION)))
    {
        Offset += MemInfo.RegionSize;
        if (MemInfo.Protect == PAGE_EXECUTE_READ)
        {
            Total += MemInfo.RegionSize;
            VirtualProtect(MemInfo.BaseAddress, MemInfo.RegionSize, PAGE_EXECUTE_READWRITE, &MemInfo.Protect);
        }
    }
}

void anvil_initialize()
{
    printf("%s %s\n", anvil_get_project_name_string(), anvil_get_config_string());
    printf("%s\n", anvil_build_version_string());
    printf("Base address: %p\n\n", (void*)module_base);
    enable_memory_write(module_base);
    anvil_dedi_apply_patches();
    anvil_dedi_apply_hooks();
}

bool anvil_create_session()
{
    printf("Creating session...\n");
    if (!hf2p_setup_session())
    {
        printf("Session creation failed!\n");
        return false;
    }
    user_interface_set_desired_multiplayer_mode(_desired_multiplayer_mode_custom_games);
    return true;
}

void anvil_session_update()
{
    static bool logged_connection_info = true;
    static bool key_held_home = false;
    static bool key_held_end = false;
    static bool key_held_insert = false;
    static bool key_held_pgup = false;
    static bool key_held_pgdown = false;
    //static bool key_held_delete = false; // used for podium taunts
    c_network_session* network_session = network_get_session_manager()->session[0];
    if (network_session->current_local_state() == _network_session_state_none)
    {
        anvil_create_session();
        logged_connection_info = false;
    }

    // log session connection info
    if (!logged_connection_info)
    {
        // wait for the managed session to create & for the session to establish
        auto managed_session = &online_session_manager_globals->managed_sessions[network_session->managed_session_index()];
        if (managed_session->flags.test(_online_managed_session_created_bit) && network_session->established())
        {
            logged_connection_info = true;
            char address_str[0x100];
            s_transport_secure_identifier secure_identifier = {};
            s_transport_secure_address secure_address = {};
            if (transport_secure_identifier_retrieve(&transport_security_globals->address, _transport_platform_pc, &secure_identifier, &secure_address))
            {
                printf("\nSession ready to join!\nServer Address: %s\nSecure Address: %s\nSecure Identifier: %s\n\n",
                    transport_address_to_string(&transport_security_globals->address, nullptr, address_str, 0x100, true, false),
                    transport_secure_address_get_string(&secure_address),
                    transport_secure_identifier_get_string(&secure_identifier));
            }
            else
            {
                printf("\nSession failed to retrieve security info.\nServer Address: %s\n\n",
                    transport_address_to_string(&transport_security_globals->address, nullptr, address_str, 0x100, true, false));
            }
            // set default dedicated server state
            if (game_is_dedicated_server())
            {
                e_dedicated_server_session_state session_state = _dedicated_server_session_state_waiting_for_players;
                network_session->get_session_parameters()->dedicated_server_session_state.set(&session_state);
            }
        }
    }

    // once session is setup
    if (network_session->established())
    {
        c_network_session_membership* membership = network_session->get_session_membership();
        c_network_session_parameters* parameters = network_session->get_session_parameters();
        static c_enum<e_player_vote_selection, byte, k_player_vote_selection_count> vote_selections[k_maximum_multiplayer_players];
        
        // debug server controls
        if (anvil_key_pressed(VK_NEXT, &key_held_pgdown)) // begin voting
        {
            printf("Starting vote...\n");
            s_network_session_parameter_lobby_vote_set lobby_vote_set = {};
            lobby_vote_set.vote_options[0].gamemode = 0;
            lobby_vote_set.vote_options[0].map = 0;
            lobby_vote_set.vote_options[1].gamemode = 0;
            lobby_vote_set.vote_options[1].map = 1;
            parameters->lobby_vote_set.set(&lobby_vote_set);
            e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
            parameters->dedicated_server_session_state.set(&session_state);
            parameters->countdown_timer.set(_countdown_type_voting, 10);
            memset(vote_selections, _player_vote_none, k_maximum_multiplayer_players);
        }
        else if (anvil_key_pressed(VK_HOME, &key_held_home))
        {
            printf("Launching session...\n");
            parameters->session_mode.set(_network_session_mode_setup);
        }
        else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
        {
            printf("Starting session countdown...\n");
            parameters->countdown_timer.set(_countdown_type_game_start, 5);
            e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
            parameters->dedicated_server_session_state.set(&session_state);
        }
        else if (anvil_key_pressed(VK_END, &key_held_end))
        {
            printf("Ending game...\n");
            parameters->session_mode.set(_network_session_mode_end_game);
        }
        else if (anvil_key_pressed(VK_INSERT, &key_held_insert))
        {
            printf("Setting test map...\n");
            anvil_session_set_gamemode(network_session, _engine_variant_slayer);
            anvil_session_set_map(_s3d_turf);
        }

        // update dedicated server state
        auto dedicated_server_session_state = parameters->dedicated_server_session_state.get();
        if (dedicated_server_session_state != nullptr && game_is_dedicated_server())
        {
            if (*dedicated_server_session_state == _dedicated_server_session_state_voting)
            {
                // if voting is active
                if (parameters->countdown_timer.get_countdown_timer() > 0)
                {
                    // update votes
                    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
                    {
                        auto player = membership->get_player(i);
                        auto vote_selection_index = player->configuration.client.vote_selection_index;
                        // if vote has changed and is valid
                        if (vote_selection_index > _player_vote_none && vote_selection_index < k_player_vote_selection_count && vote_selection_index != vote_selections[i])
                        {
                            s_network_session_parameter_lobby_vote_set lobby_vote_set;
                            parameters->lobby_vote_set.get(&lobby_vote_set);

                            // remove old vote if its valid
                            if (vote_selections[i] > _player_vote_none && vote_selections[i] < k_player_vote_selection_count)
                                lobby_vote_set.vote_options[vote_selections[i].get() - 1].number_of_votes--;
                            // update vote selection
                            vote_selections[i] = vote_selection_index.get();
                            // add new vote
                            lobby_vote_set.vote_options[vote_selection_index.get() - 1].number_of_votes++;

                            // update vote set parameter
                            parameters->lobby_vote_set.set(&lobby_vote_set);
                        }
                    }
                }
                // else if voting has ended
                else
                {
                    // loop through each player and gather their vote selections
                    byte option_votes[2] = {};
                    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
                    {
                        // if vote selection is valid
                        if (vote_selections[i] > _player_vote_none && vote_selections[i] < k_player_vote_selection_count)
                            option_votes[vote_selections[i].get() - 1]++;
                    }
                    long winning_index = (option_votes[0] > option_votes[1] ? 0 : 1);
                    printf("option %d wins the vote!\n", winning_index);

                    // TODO: retrieve voting options from title instances - pull map id and use with anvil_session_set_map
                    if (winning_index == 0)
                    {
                        anvil_session_set_gamemode(network_session, _engine_variant_slayer);
                        anvil_session_set_map(_s3d_edge);
                    }
                    else if (winning_index == 1)
                    {
                        anvil_session_set_gamemode(network_session, _engine_variant_slayer);
                        anvil_session_set_map(_s3d_turf);
                    }

                    e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
                    parameters->dedicated_server_session_state.set(&session_state);
                }
            }
            // if dedi state is ready to start and the map has finished loading, start the countdown
            else if (*dedicated_server_session_state == _dedicated_server_session_state_game_start_countdown &&
                parameters->game_start_status.get()->game_start_status == _session_game_start_status_ready_leader &&
                parameters->game_start_status.get()->map_load_progress == 100)
            {
                parameters->countdown_timer.set(_countdown_type_game_start, 5);
            }
            // set dedi state to in game once the start countdown has finished
            else if (parameters->countdown_timer.get_countdown_type() == _countdown_type_game_start && parameters->countdown_timer.get_countdown_timer() <= 0)
            {
                parameters->countdown_timer.set(_countdown_type_none, 0);
                e_dedicated_server_session_state session_state = _dedicated_server_session_state_in_game;
                parameters->dedicated_server_session_state.set(&session_state);
            }
            // TODO: dedi state is currently reset on game end in game_engine_detach_from_simulation_gracefully - FIND A BETTER WAY TO DO THIS
        }
    }
}

bool anvil_session_set_map(e_map_id map_id)
{
    c_map_variant* map_variant = new c_map_variant();
    map_variant->create_default(map_id);
    bool success = user_interface_squad_set_multiplayer_map(map_variant);
    if (success == false)
        printf("Failed to set map variant!\n");
    delete map_variant;
    return success;
}

bool anvil_session_set_gamemode(c_network_session* session, e_engine_variant engine_variant)
{
    c_game_variant game_variant = c_game_variant();
    build_default_game_variant(&game_variant, engine_variant);
    game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_number_of_rounds = 1;
    //game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_round_time_limit = 1;
    game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_early_victory_win_count = 1;
    game_variant.m_storage.m_slayer_variant.m_score_to_win = 10;
    //game_variant.m_storage.m_slayer_variant.m_suicide_points = 1;
    //game_variant.m_storage.m_base_variant.m_miscellaneous_options.m_flags.set(_game_engine_miscellaneous_option_teams_enabled, true);

    //game_variant.m_storage.m_base_variant.m_respawn_options.m_flags.set(_game_engine_respawn_options_auto_respawn_disabled, false);
    //wchar_t variant_name[16] = L"RESPAWN TEST";
    //ustrnzcpy(game_variant.m_storage.m_base_variant.m_metadata.m_name, variant_name, 16);

    if (!session->get_session_parameters()->ui_game_mode.request_change(_gui_game_mode_multiplayer))
    {
        printf("Failed to set UI game mode!\n");
        return false;
    }
    if (!user_interface_squad_set_game_variant(&game_variant))
    {
        printf("Failed to set game variant!\n");
        return false;
    }
    return true;
}

void anvil_session_set_test_player_data(c_network_session_membership* membership)
{
    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
    {
        auto current_player = membership->get_player(i);
        auto host_configuration = &current_player->configuration.host;
        
        // display server-set loadouts in the client's UI
        //host_configuration->s3d_player_customization.override_api_data = true;
        //host_configuration->s3d_player_container.override_api_data = true;

        // host player data
        if (current_player->peer_index == membership->host_peer_index())
        {
            
        }
    }
    // push update
    membership->increment_update();
}

// TODO: PULL THIS DATA FROM THE API RATHER THAN HARDCODING IT
bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration)
{
    assert(configuration != nullptr);
    assert(session != nullptr);
    bool player_data_updated = false;
    
    auto membership = session->get_session_membership();
    auto player = membership->get_player(player_index);
    auto peer = membership->get_peer(player->peer_index);
    // if the player's xuid is unassigned 
    // temp peer name check, we're currently relying on this to assign the right user id so we need to wait until the first peer properties update comes in and sets this
    if (configuration->player_xuid.data == 0 && ustrlen(peer->properties.peer_name) > 0)
    {        
        // assign player name based on peer name - TODO: THIS IS TEMPORARY, WE NEED A MORE RIGOROUS WAY OF VERIFYING USER CREDENTIALS
        memcpy(&configuration->player_name, peer->properties.peer_name, 32);

        // assign temporary hardcoded loadout data
        configuration->s3d_player_customization.colors[_primary] = 0xFF230A; // orange red
        configuration->s3d_player_customization.colors[_secondary] = 0xFFFFFF; // white
        configuration->s3d_player_customization.colors[_visor] = 0xFF640A;
        configuration->s3d_player_customization.colors[_lights] = 0xFF640A;
        configuration->s3d_player_customization.colors[_holo] = 0xFF640A;
        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_air_assault;
        configuration->s3d_player_container.loadouts[0].primary_weapon = _assault_rifle;
        configuration->s3d_player_container.loadouts[0].secondary_weapon = _magnum_v1;
        configuration->s3d_player_container.loadouts[0].tactical_packs[0] = _adrenaline;
        configuration->s3d_player_container.loadouts[0].tactical_packs[1] = _deployable_cover;
        configuration->s3d_player_container.loadouts[0].tactical_packs[2] = _hologram;
        configuration->s3d_player_container.loadouts[0].tactical_packs[3] = _jammer;
        //configuration->s3d_player_customization.override_api_data = true;
        //configuration->s3d_player_container.override_api_data = true;
        player_data_updated = true;

        // dedi host loadout
        if (player->peer_index == membership->host_peer_index() && game_is_dedicated_server())
        {
            configuration->player_xuid.data = -1; // SYSTEM/invalid player id
            player->controller_index = 0;
            configuration->s3d_player_container.loadouts[0].armor_suit = _armor_scanner;
            configuration->s3d_player_customization.colors[_primary] = 0x0F0F0F;
            configuration->s3d_player_customization.colors[_secondary] = 0x003750;
            player_data_updated = true;
        }
        // standard player xuids here
        else
        {
            // make sure these appear in API user id order
            wchar_t player_list[16][16] = { L"zzVertigo", L"Yokim", L"Twister", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L"", L""};
            for (size_t i = 0; i < 16; i++)
            {
                // if a match is found
                if (ustrncmp(peer->properties.peer_name, player_list[i], 16) == 0)
                {
                    configuration->player_xuid.data = i + 1;
                    // set dev service tag
                    if (i == 0 || i == 1 || i == 2)
                    {
                        wchar_t service_tag[5] = L"DEV";
                        memcpy(&configuration->player_appearance.service_tag, service_tag, 10);
                    }
                    player_data_updated = true;
                    break;
                }
            }
        }
    }
    return player_data_updated;
}

void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status)
{
    if (start_status->game_start_status == _session_game_start_status_error)
        printf("start status updated: error (%s) affected player mask [%08X]\n", multiplayer_game_start_error_to_string(start_status->game_start_error), start_status->player_error_mask);
    else
        printf("start status updated: %s (map load progress: %d)\n", multiplayer_game_start_status_to_string(start_status->game_start_status), start_status->map_load_progress);
}

bool anvil_key_pressed(long vkey, bool* key_held)
{
    bool key_pressed = false;

    if ((GetKeyState(vkey) & 0x8000))
    {
        if (*key_held == false)
            key_pressed = true;
        else
            key_pressed = false;
        *key_held = true;
    }
    else
    {
        *key_held = false;
        key_pressed = false;
    }
    return key_pressed;
}

void anvil_launch_scenario(const char* scenario_path)
{
    //csstrnzcpy(g_tutorial_scenario_path, scenario_path, 40);
    //csmemcpy(g_tutorial_scenario_path, scenario_path, strlen_debug(scenario_path));
    hq_start_tutorial_level();
}

// TODO: use this to retrieve a secure address from the API matchmaking service
void anvil_get_dedicated_secure_address(s_transport_secure_address* secure_address)
{
    // cdbeb834-c1a4-9a4d-8dda-f540f378d644
    byte temp_address[16] = { 0x34, 0xb8, 0xbe, 0xcd, 0xa4, 0xc1, 0x4d, 0x9a, 0x8d, 0xda, 0xf5, 0x40, 0xf3, 0x78, 0xd6, 0x44 };
    memcpy(secure_address, temp_address, sizeof(s_transport_secure_address));
}

// TODO: above, but retrieve the lobby/party ID
void anvil_get_dedicated_secure_identifier(s_transport_secure_identifier* secure_identifier)
{
    // 379bd202-9787-bd4a-ad96-a89bae9c7e4a
    byte temp_identifier[16] = { 0x02, 0xd2, 0x9b, 0x37, 0x87, 0x97, 0x4a, 0xbd, 0xad, 0x96, 0xa8, 0x9b, 0xae, 0x9c, 0x7e, 0x4a };
    memcpy(secure_identifier, temp_identifier, sizeof(s_transport_secure_identifier));
}