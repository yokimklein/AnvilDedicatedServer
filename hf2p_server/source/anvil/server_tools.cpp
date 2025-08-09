#include "server_tools.h"
#include <cseries\cseries.h>
#include <anvil\backend\private_service.h>
#include <anvil\build_version.h>
#include <anvil\hooks\hooks.h>
#include <game\game.h>
#include <game\game_engine_simulation.h>
#include <hf2p\hq.h>
#include <interface\user_interface_networking.h>
#include <interface\user_interface_session.h>
#include <iostream>
#include <networking\logic\network_life_cycle.h>
#include <networking\logic\network_session_interface.h>
#include <networking\session\network_managed_session.h>
#include <networking\session\network_session.h>
#include <game\game_engine_event_definitions.h>
#include <game\game_engine_util.h>
#include <tag_files\string_ids.h>
#include <hf2p\loadouts.h>
#include <anvil\backend\user.h>
#include <anvil\backend\lobby.h>
#include <memory\tls.h>
#include <fstream>
#include <string>
#include <combaseapi.h>
#include <networking\network_time.h>

constexpr wchar_t k_anvil_machine_name[16] = L"ANVIL_DEDICATED";
constexpr wchar_t k_anvil_session_name[32] = L"ANVIL_DEDICATED_SESSION";

void enable_memory_write(void* base)
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
    printf("%s\n", anvil_get_build_name_string());
    printf("%s\n", anvil_build_version_string());
    printf("Base address: %p\n\n", base_address<void*>());
    enable_memory_write(base_address<void*>());
    anvil_patches_apply();
    anvil_hooks_apply();
}

bool anvil_session_create()
{
    printf("Creating session...\n");
    network_life_cycle_end();
    if (!network_life_cycle_create_local_squad(_network_session_class_online))
    {
        printf("Session creation failed!\n");
        return false;
    }
    user_interface_set_desired_multiplayer_mode(_desired_multiplayer_mode_custom_games);
    network_session_interface_set_local_name(k_anvil_machine_name, k_anvil_session_name);
    return true;
}

// #TODO: move to anvil\session_control.h
void anvil_session_update()
{
    if (!life_cycle_globals.initialized)
    {
        return;
    }

    // $TODO: Move this to an anvil_backend_initialize() function called from the game executable
    if (!g_backend_private_service)
    {
        c_backend_private_service::initialise();
    }

    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();

    static bool logged_connection_info = true;
    static bool key_held_home = false;
    static bool key_held_end = false;
    static bool key_held_insert = false;
    static bool key_held_pgup = false;
    static bool key_held_pgdown = false;
    //static bool key_held_delete = false; // used for podium taunts

    // $TODO: handle this for when we're not running in dedicated server mode
    // register game server with the API prior to creating the session
    switch (g_lobby_info.status)
    {
        case _request_status_none:
        {
            if (!g_lobby_info.valid)
            {
                s_request_register_game_server register_request;
                s_transport_secure_address server_identifier;

                anvil_get_server_identifier(&server_identifier);
                register_request.secureAddr = transport_secure_address_get_string(&server_identifier);

                g_backend_private_service->request_register_game_server(register_request);
            }
            break;
        }
        case _request_status_received:
        {
            if (session->current_local_state() == _network_session_state_none)
            {
                g_lobby_info.status = _request_status_none;
                anvil_session_create();
                logged_connection_info = false;
            }
            break;
        }
        // on failure, wait 5 seconds before requesting again
        case _request_status_failed:
        {
            g_lobby_info.failure_time = network_time_get();
            g_lobby_info.status = _request_status_timeout;
            break;
        }
        case _request_status_timeout:
        {
            if (network_time_since(g_lobby_info.failure_time) >= PRIVATE_SERVICE_REQUEST_TIMEOUT)
            {
                g_lobby_info.failure_time = NONE;
                g_lobby_info.status = _request_status_none;
            }
            break;
        }
        case _request_status_waiting:
        default:
        {
            return;
        }
    }

    // log session connection info
    if (!logged_connection_info)
    {
        // wait for the managed session to create & for the session to establish
        s_online_managed_session* managed_session = &online_session_manager_globals.managed_sessions[session->managed_session_index()];
        if (managed_session->flags.test(_online_managed_session_created_bit) && session->established())
        {
            logged_connection_info = true;
            char address_str[0x100];
            s_transport_secure_identifier secure_identifier = {};
            s_transport_secure_address secure_address = {};
            if (transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_windows, &secure_identifier, &secure_address))
            {
                printf("\nSession ready to join!\nServer Address: %s\nServerID: %s\nLobbyID: %s\n\n",
                    transport_address_to_string(&transport_security_globals.address, NULL, address_str, 0x100, true, false),
                    transport_secure_address_get_string(&secure_address),
                    transport_secure_identifier_get_string(&secure_identifier));
            }
            else
            {
                printf("\nSession failed to retrieve security info.\nServer Address: %s\n\n",
                    transport_address_to_string(&transport_security_globals.address, NULL, address_str, 0x100, true, false));
            }
            if (game_is_dedicated_server())
            {
                // update server info on API
                s_request_update_game_server update_request;

                s_transport_secure_address server_identifier;
                anvil_get_server_identifier(&server_identifier);
                update_request.secureAddr = transport_secure_address_get_string(&server_identifier);
                // $TODO: pull this IP from somewhere
                update_request.serverAddr = "192.168.0.181"; //transport_address_to_string(&transport_security_globals.address, NULL, address_str, 0x100, false, false);
                update_request.serverPort = transport_security_globals.address.port;
                // $TODO: pull this from a config?
                update_request.playlistId = "playlist_team_slayer_small";

                g_backend_private_service->request_update_game_server(update_request);

                // set default dedicated server state
                e_dedicated_server_session_state session_state = _dedicated_server_session_state_waiting_for_players;
                session->get_session_parameters()->m_parameters.dedicated_server_session_state.set(&session_state);
            }
        }
    }

    // run anvil session loop once the session is setup
    if (!session->established())
    {
        return;
    }

    c_network_session_membership* membership = session->get_session_membership_for_update();
    c_network_session_parameters* parameters = session->get_session_parameters();
    
    // debug server controls
    if (anvil_key_pressed(VK_NEXT, &key_held_pgdown)) // begin voting
    {
        printf("Starting vote...\n");
        anvil_session_start_voting(session);
    }
    else if (anvil_key_pressed(VK_HOME, &key_held_home))
    {
        printf("Launching session...\n");
        parameters->m_parameters.session_mode.set(_network_session_mode_setup);
    }
    else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
    {
        printf("Disconnecting session...\n");
        session->disconnect();
        
        //anvil_session_set_test_player_data(membership);

        /*
        // load new string from text file
        std::ifstream file("map_load.txt");
        std::string scenario_path_str;
        std::string map_name_str;
        if (std::getline(file, scenario_path_str))
        {
            if (!std::getline(file, map_name_str))
            {
                printf("no .map name provided, using -1 map id\n");
                anvil_launch_scenario(scenario_path_str.c_str(), nullptr);
            }
            else
            {
                wchar_t map_name[128];
                ascii_string_to_wchar_string(map_name_str.c_str(), map_name, map_name_str.length(), nullptr);
                anvil_launch_scenario(scenario_path_str.c_str(), map_name);
            }
        }
        else
        {
            printf("map_load.txt mising scenario path line!\n");
        }
        */
        //TLS_DATA_GET_VALUE_REFERENCE(director_globals);
        //director_globals->infos[0].camera_mode;
        //director_globals->infos[0].director_mode;
        //director_globals->infos[0].director_perspective;
        //printf("Finished.\n");

        //printf("Starting session countdown...\n");
        //parameters->countdown_timer.set(_network_game_countdown_delayed_reason_none, 5);
        //e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
        //parameters->dedicated_server_session_state.set(&session_state);
    }
    else if (anvil_key_pressed(VK_END, &key_held_end))
    {
        printf("Ending game...\n");
        parameters->m_parameters.session_mode.set(_network_session_mode_end_game);
    }
    else if (anvil_key_pressed(VK_INSERT, &key_held_insert))
    {
        printf("Setting test mode...\n");
        anvil_session_set_gamemode(session, _game_engine_type_slayer, 0);
        anvil_session_set_map(_riverworld);
    }

    // update voting
    if (parameters->m_parameters.dedicated_server_session_state.get_allowed()) // skip if data is not available
    {
        anvil_session_update_voting(session);
    }
}

s_anvil_session_vote_pool::s_anvil_session_vote_pool()
{
    // Map ids in order
    e_map_id map_ids[] =
    {
        _zanzibar,
        _s3d_turf,
        _deadlock,
        _guardian,
        _riverworld,
        _chill,
        _cyberdyne,
        _bunkerworld,
        _s3d_reactor,
        _s3d_edge,
        _s3d_avalanche
    };
    for (char i = 0; i < map_entries.get_count(); i++)
    {
        map_entries[i].map_id = map_ids[i];
        map_entries[i].map_title_instance = i; // TIs appear in map id order so this should be fine
    }
    gamemode_entries[0].engine_index = _game_engine_type_ctf;
    gamemode_entries[0].gamemode_title_instance = 0;
    gamemode_entries[0].variant_index = 0;
    gamemode_entries[1].engine_index = _game_engine_type_slayer;
    gamemode_entries[1].gamemode_title_instance = 1;
    gamemode_entries[1].variant_index = 0;
    gamemode_entries[2].engine_index = _game_engine_type_slayer;
    gamemode_entries[2].gamemode_title_instance = 2;
    gamemode_entries[2].variant_index = 1;
}

s_anvil_session_vote_pool g_anvil_session_vote_pool;
e_player_vote_selection g_anvil_vote_selections[k_maximum_multiplayer_players]{};

// TEMP FUNCTION: TODO blam _random_range
long rand_range(long min, long max)
{
    long range = max - min + 1;
    long num = rand() % range + min;
    return num;
};
void anvil_session_start_voting(c_network_session* session)
{
    g_anvil_vote_selections[0] = _player_vote_none;
    g_anvil_vote_selections[1] = _player_vote_none;
    long map_entry_indices[2] = 
    {
        rand_range(0, g_anvil_session_vote_pool.map_entries.get_count() - 1),
        rand_range(0, g_anvil_session_vote_pool.map_entries.get_count() - 1)
    };
    ASSERT(VALID_INDEX(map_entry_indices[0], NUMBEROF(s_anvil_session_vote_pool::map_entries)));
    ASSERT(VALID_INDEX(map_entry_indices[1], NUMBEROF(s_anvil_session_vote_pool::map_entries)));
    long gamemode_entry_indices[2] =
    {
        rand_range(1, g_anvil_session_vote_pool.gamemode_entries.get_count() - 1), // CLAMP OUT CTF
        rand_range(1, g_anvil_session_vote_pool.gamemode_entries.get_count() - 1)  // CLAMP OUT CTF
    };
    ASSERT(VALID_INDEX(gamemode_entry_indices[0], NUMBEROF(s_anvil_session_vote_pool::gamemode_entries)));
    ASSERT(VALID_INDEX(gamemode_entry_indices[1], NUMBEROF(s_anvil_session_vote_pool::gamemode_entries)));

    c_network_session_parameters* parameters = session->get_session_parameters();
    s_network_session_parameter_lobby_vote_set lobby_vote_set = {};
    lobby_vote_set.vote_options[0].gamemode = g_anvil_session_vote_pool.gamemode_entries[gamemode_entry_indices[0]].gamemode_title_instance;
    lobby_vote_set.vote_options[0].map = g_anvil_session_vote_pool.map_entries[map_entry_indices[0]].map_title_instance;
    lobby_vote_set.vote_options[1].gamemode = g_anvil_session_vote_pool.gamemode_entries[gamemode_entry_indices[1]].gamemode_title_instance;
    lobby_vote_set.vote_options[1].map = g_anvil_session_vote_pool.map_entries[map_entry_indices[1]].map_title_instance;
    parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);
    e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
    parameters->m_parameters.dedicated_server_session_state.set(&session_state);
    parameters->m_parameters.countdown_timer.set(_network_game_countdown_delayed_reason_voting, 10);
}

void anvil_session_update_voting(c_network_session* session)
{
    const c_network_session_membership* membership = session->get_session_membership();
    c_network_session_parameters* parameters = session->get_session_parameters();

    const e_dedicated_server_session_state* dedicated_server_session_state = parameters->m_parameters.dedicated_server_session_state.get();
    if (!dedicated_server_session_state || !game_is_dedicated_server())
    {
        return;
    }

    if (*dedicated_server_session_state == _dedicated_server_session_state_voting)
    {
        // if voting has ended
        if (parameters->m_parameters.countdown_timer.get_countdown_timer() <= 0)
        {
            // loop through each player and gather their vote selections
            char option_votes[2] = {};
            for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
            {
                // if vote selection is valid
                if (g_anvil_vote_selections[i] > _player_vote_none && g_anvil_vote_selections[i] < k_player_vote_selection_count)
                {
                    option_votes[g_anvil_vote_selections[i] - 1]++;
                }
            }
            char winning_index = (option_votes[0] > option_votes[1] ? 0 : 1);
            printf("option %d wins the vote!\n", winning_index);
            s_network_session_parameter_lobby_vote_set lobby_vote_set;
            parameters->m_parameters.lobby_vote_set.get(&lobby_vote_set);
            lobby_vote_set.winning_vote_index = winning_index;
            parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);

            // load winning map & mode
            char gamemode_index = lobby_vote_set.vote_options[winning_index].gamemode;
            e_game_engine_type engine_index = g_anvil_session_vote_pool.gamemode_entries[gamemode_index].engine_index.get();
            long variant_index = g_anvil_session_vote_pool.gamemode_entries[gamemode_index].variant_index;
            char map_index = lobby_vote_set.vote_options[winning_index].map;
            e_map_id map_id = g_anvil_session_vote_pool.map_entries[map_index].map_id;
            anvil_session_set_gamemode(session, engine_index, variant_index);
            anvil_session_set_map(map_id);

            e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
            parameters->m_parameters.dedicated_server_session_state.set(&session_state);

            return;
        }

        // if voting is active
        // update votes
        for (long i = membership->get_first_player(); i != NONE; i = membership->get_next_player(i))
        {
            const s_network_session_player* player = membership->get_player(i);
            e_player_vote_selection vote_selection_index = player->configuration.client.vote_selection_index;
            // if vote hasn't changed or is invalid
            if (vote_selection_index <= _player_vote_none || vote_selection_index >= k_player_vote_selection_count || vote_selection_index == g_anvil_vote_selections[i])
            {
                continue;
            }

            s_network_session_parameter_lobby_vote_set lobby_vote_set;
            parameters->m_parameters.lobby_vote_set.get(&lobby_vote_set);

            // remove old vote if its valid
            if (g_anvil_vote_selections[i] > _player_vote_none && g_anvil_vote_selections[i] < k_player_vote_selection_count)
            {
                lobby_vote_set.vote_options[g_anvil_vote_selections[i] - 1].number_of_votes--;
            }

            // update vote selection
            g_anvil_vote_selections[i] = vote_selection_index;
            // add new vote
            lobby_vote_set.vote_options[vote_selection_index - 1].number_of_votes++;

            // update vote set parameter
            parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);
        }
    }
    // if dedi state is ready to start and the map has finished loading, start the countdown
    else if (*dedicated_server_session_state == _dedicated_server_session_state_game_start_countdown &&
        parameters->m_parameters.game_start_status.get()->game_start_status == _session_game_start_status_ready_leader &&
        parameters->m_parameters.game_start_status.get()->map_load_progress == 100)
    {
        parameters->m_parameters.countdown_timer.set(_network_game_countdown_delayed_reason_start, 5);
    }
    // dedi state is set to in game by c_life_cycle_state_handler_in_game::enter, and reset to waiting for players in c_life_cycle_state_handler_in_game::exit

    // when enough players are found JOINING SESSION
    // once connected, WAITING FOR PLAYERS until all lobby players have connected
    // VOTING ENDS IN for 10 seconds during vote phase
    // GAME STARTS 5 second countdown timer
    // wait 30 seconds once loaded back to mainmenu after game with 'PREPARE FOR BATTLE' text, kick all players from session back to matchmake
    // then return to MATCHMAKING QUEUE until enough players are found

    // TODO: what displays the 'PREPARE FOR BATTLE' text?
}

bool anvil_session_set_map(e_map_id map_id)
{
    // create variant on the heap to avoid a large stack frame
    c_map_variant* map_variant = new c_map_variant();
    map_variant->create_default(map_id);
    bool success = user_interface_squad_set_multiplayer_map(map_variant);
    if (success == false)
    {
        printf("Failed to set map variant!\n");
    }
    delete map_variant;
    return success;
}

bool anvil_session_set_gamemode(c_network_session* session, e_game_engine_type engine_index, long variant_index)
{
    c_game_variant game_variant = c_game_variant();
    if (!game_engine_tag_defined_variant_get_built_in_variant(engine_index, variant_index, &game_variant))
    {
        printf("Failed to get game variant!");
        return false;
    }

    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_teams_enabled(false);
    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_round_limit(3);
    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_early_victory_win_count(3);
    //game_variant.get_slayer_variant_writeable()->set_score_to_win(1);
    //game_variant.get_slayer_variant_writeable()->set_suicide_points(1);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_team_lives_per_round(1);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_lives_per_round(2);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_appearance_traits_writeable()->set_active_camo_setting(_active_camo_setting_good, false);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_movement_traits_writeable()->set_gravity_setting(_player_gravity_setting_50_percent, false);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_sensor_traits_writeable()->set_motion_tracker_setting(_motion_tracker_setting_off, false);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_shield_vitality_traits_writeable()->set_shield_multiplier_setting(_shield_multiplier_setting_4x, false);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_weapons_traits_writeable()->set_infinite_ammo_setting(_infinite_ammo_setting_enabled, false);

    if (!session->get_session_parameters()->m_parameters.ui_game_mode.request_change(_gui_game_mode_multiplayer))
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
    for (long player_index = membership->get_first_player(); player_index != NONE; player_index = membership->get_next_player(player_index))
    {
        s_network_session_player* current_player = membership->get_player(player_index);
        s_player_configuration_from_host* host_configuration = &current_player->configuration.host;
        
        // display server-set loadouts in the client's UI
        //host_configuration->s3d_player_customization.override_api_data = true;
        //host_configuration->s3d_player_container.override_api_data = true;

        current_player->controller_index = _controller_index0;

        if (current_player->configuration.host.user_xuid == 2) // yokim
        {
            host_configuration->team_index = _multiplayer_team_red;
            host_configuration->assigned_team_index = _multiplayer_team_red;
        }
        if (current_player->configuration.host.user_xuid == 3) // twister
        {
            host_configuration->team_index = _multiplayer_team_blue;
            host_configuration->assigned_team_index = _multiplayer_team_blue;
        }

        //host_configuration->s3d_player_container.loadouts[0].primary_weapon = _none;
        //host_configuration->s3d_player_container.loadouts[0].secondary_weapon = _none;
    }
    // push update
    membership->increment_update();
}

bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration)
{
    ASSERT(configuration != nullptr);
    ASSERT(session != nullptr);
    bool player_data_updated = false;
    
    c_network_session_membership* membership = session->get_session_membership_for_update();
    const s_network_session_player* player = membership->get_player(player_index);
    const s_network_session_peer* peer = membership->get_peer(player->peer_index);

    // $TODO: the peer should say when it needs a configuration update, only call this function when that update number has changed
    if (!configuration->s3d_player_customization.override_api_data && configuration->user_xuid != USER_SYSTEM && configuration->user_xuid > USER_INVALID)
    {
        // assign player name based on peer name - $TODO: THIS IS TEMPORARY, RETRIEVE THIS FROM API W/ USER ID INSTEAD
        if (!configuration->player_name.is_equal(peer->properties.peer_name.get_string()))
        {
            configuration->player_name.set(peer->properties.peer_name.get_string());
            player_data_updated = true;
        }

        s_api_user_customisation* customisation = user_get_customisation_from_api(configuration->user_xuid);
        if (customisation)
        {
            s_s3d_player_customization player_customisation;
            customisation->write_colours(&player_customisation);
            if (configuration->s3d_player_customization != player_customisation)
            {
                configuration->s3d_player_customization = player_customisation;
                player_data_updated = true;
            }
        }
        else
        {
            //printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: anvil_assign_player_loadout: failed to retrieve user customisation from API for user [%lld]!\n", configuration->user_xuid);
        }
        for (long i = 0; i < k_maximum_loadouts; i++)
        {
            s_api_user_loadout* loadout = user_get_loadout_from_api(configuration->user_xuid, i);
            if (loadout)
            {
                s_s3d_player_loadout player_loadout;
                loadout->write_configuration(&player_loadout);

                if (configuration->s3d_player_container.loadouts[i] != player_loadout)
                {
                    configuration->s3d_player_container.loadouts[i] = player_loadout;
                    player_data_updated = true;
                }
            }
            else
            {
                //printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: anvil_assign_player_loadout: failed to retrieve user loadout '%d' from API for user [%lld]!\n", i, configuration->user_xuid);
            }
        }
        // TODO: modifiers
        //configuration->s3d_player_container.modifiers[0].modifier_values[_enable_nemesis_mechanics] = true;
        //configuration->s3d_player_container.modifiers[0].modifier_values[_grenade_warning] = false;
    }

    if (player_data_updated)
    {
        membership->increment_update();
    }

    return player_data_updated;
}

void anvil_log_game_start_status(s_network_session_parameter_game_start_status* start_status)
{
    if (start_status->game_start_status == _session_game_start_status_error)
    {
        printf("start status updated: error (%s) affected player mask [%08X]\n", multiplayer_game_start_error_to_string(start_status->game_start_error), start_status->player_error_mask);
    }
    else
    {
        printf("start status updated: %s (map load progress: %d)\n", multiplayer_game_start_status_to_string(start_status->game_start_status), start_status->map_load_progress);
    }
}

bool anvil_key_pressed(long vkey, bool* key_held)
{
    bool key_pressed = false;

    if ((GetKeyState(vkey) & 0x8000))
    {
        if (*key_held == false)
        {
            key_pressed = true;
        }
        else
        {
            key_pressed = false;
        }
        *key_held = true;
    }
    else
    {
        *key_held = false;
        key_pressed = false;
    }
    return key_pressed;
}

// map_name is optional to retrieve a map id from a .map
void anvil_launch_scenario(const char* scenario_path, const wchar_t* map_name)
{
    ASSERT(scenario_path);
    g_tutorial_scenario_path.set(scenario_path);
    if (map_name)
    {
        g_tutorial_map_name.set(map_name);
    }
    else
    {
        g_tutorial_map_name.set(L"no .map provided");
    }

    long scnr_path_address = (long)g_tutorial_scenario_path.get_string();
    memcpy(base_address<void*>(0x33AB0D), &scnr_path_address, 4);
    memcpy(base_address<void*>(0x33AB58), &scnr_path_address, 4);

    long map_name_address = (long)g_tutorial_map_name.get_string();
    memcpy(base_address<void*>(0xDD176), &map_name_address, 4);

    printf("anvil_launch_scenario(): launching %s (%ls)\n", g_tutorial_scenario_path.get_string(), g_tutorial_map_name.get_string());
    hq_start_tutorial_level();
}

void anvil_boot_peer(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
    {
        return;
    }

    // If we're in a game, display boot message for all associated players
    if (life_cycle_globals.state_manager.get_current_state() == _life_cycle_state_in_game)
    {
        const s_network_session_peer* peer = session->get_session_membership()->get_peer(peer_index);
        for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
        {
            if (!peer->player_mask.test(player_absolute_index))
            {
                continue;
            }

            datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
            if (player_index == NONE)
            {
                continue;
            }

            s_game_engine_event_data event_data;
            game_engine_initialize_event(_multiplayer_event_type_general, STRING_ID(game_engine, general_event_player_booted_player), &event_data);
            game_engine_set_event_effect_player_and_team(player_index, &event_data);
            game_engine_send_event(&event_data);
        }
    }

    session->host_boot_machine(peer_index, _network_session_boot_from_ui);
}

void anvil_power_sister(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
    {
        return;
    }

    c_network_session_membership* membership = session->get_session_membership_for_update();
    const s_network_session_peer* peer = membership->get_peer(peer_index);
    for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
    {
        if (!peer->player_mask.test(player_absolute_index))
        {
            continue;
        }
        datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
        if (player_index == NONE)
        {
            continue;
        }

        s_network_session_player* player = membership->get_player(player_absolute_index);
        player->configuration.host.player_name.set(L"tickle tipson");
        player->configuration.host.player_appearance.flags = 1;
        player->configuration.host.player_appearance.service_tag.set(L"SIS");
        player->configuration.host.s3d_player_container.override_api_data = true;
        player->configuration.host.s3d_player_container.loadouts[0].armor_suit = _armor_prophet;
        player->configuration.host.s3d_player_container.loadouts[0].gender = _gender_female;
        player->configuration.host.s3d_player_customization.active_loadout_index = 0;
        player->configuration.host.s3d_player_customization.colors[_armor_color_primary] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_secondary] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_lights] = 0xFF69B4;
        player->configuration.host.s3d_player_customization.colors[_armor_color_holo] = 0xFF69B4;
        membership->increment_update();
    }
}