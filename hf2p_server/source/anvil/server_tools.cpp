#include "server_tools.h"
#include <cseries\cseries.h>
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
#include <hf2p\user.h>
#include <memory\tls.h>
#include <fstream>
#include <string>

const wchar_t k_anvil_machine_name[16] = L"ANVIL_DEDICATED";
const wchar_t k_anvil_session_name[32] = L"ANVIL_DEDICATED_SESSION";

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
    printf("%s %s\n", anvil_get_project_name_string(), anvil_get_config_string());
    printf("%s\n", anvil_build_version_string());
    printf("Base address: %p\n\n", (void*)module_base);
    enable_memory_write((void*)module_base);
    anvil_patches_apply();
    anvil_hooks_apply();
}

bool anvil_create_session()
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

void anvil_session_update()
{
    if (!life_cycle_globals.initialized) return;

    c_network_session* network_session = life_cycle_globals.state_manager.get_active_squad_session();

    static bool logged_connection_info = true;
    static bool key_held_home = false;
    static bool key_held_end = false;
    static bool key_held_insert = false;
    static bool key_held_pgup = false;
    static bool key_held_pgdown = false;
    //static bool key_held_delete = false; // used for podium taunts
    if (network_session->current_local_state() == _network_session_state_none)
    {
        anvil_create_session();
        logged_connection_info = false;
    }

    // log session connection info
    if (!logged_connection_info)
    {
        // wait for the managed session to create & for the session to establish
        c_managed_session* managed_session = &online_session_manager_globals.managed_sessions[network_session->managed_session_index()];
        if (managed_session->flags.test(_online_managed_session_created_bit) && network_session->established())
        {
            logged_connection_info = true;
            char address_str[0x100];
            s_transport_secure_identifier secure_identifier = {};
            s_transport_secure_address secure_address = {};
            if (transport_secure_identifier_retrieve(&transport_security_globals.address, _transport_platform_pc, &secure_identifier, &secure_address))
            {
                printf("\nSession ready to join!\nServer Address: %s\nSecure Address: %s\nSecure Identifier: %s\n\n",
                    transport_address_to_string(&transport_security_globals.address, nullptr, address_str, 0x100, true, false),
                    transport_secure_address_get_string(&secure_address),
                    transport_secure_identifier_get_string(&secure_identifier));
            }
            else
            {
                printf("\nSession failed to retrieve security info.\nServer Address: %s\n\n",
                    transport_address_to_string(&transport_security_globals.address, nullptr, address_str, 0x100, true, false));
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
        
        // debug server controls
        if (anvil_key_pressed(VK_NEXT, &key_held_pgdown)) // begin voting
        {
            printf("Starting vote...\n");
            anvil_session_start_voting(network_session);
        }
        else if (anvil_key_pressed(VK_HOME, &key_held_home))
        {
            printf("Launching session...\n");
            parameters->session_mode.set(_network_session_mode_setup);
        }
        else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
        {
            s_api_user_loadout* loadout = user_get_loadout_from_api(2, 0);

            s_s3d_player_loadout player_loadout;
            loadout->write_configuration(&player_loadout);

            printf("Test loadout retrieve...\n");
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
            parameters->session_mode.set(_network_session_mode_end_game);
        }
        else if (anvil_key_pressed(VK_INSERT, &key_held_insert))
        {
            printf("Setting test mode...\n");
            anvil_session_set_gamemode(network_session, _game_engine_type_slayer, 0);
            anvil_session_set_map(_riverworld);
        }

        // update voting
        if (parameters->dedicated_server_session_state.get_allowed()) // skip if data is not available
            anvil_session_update_voting(network_session);
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
    for (long i = 0; i < map_entries.get_count(); i++)
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
    assert(VALID_INDEX(map_entry_indices[0], NUMBEROF(s_anvil_session_vote_pool::map_entries)));
    assert(VALID_INDEX(map_entry_indices[1], NUMBEROF(s_anvil_session_vote_pool::map_entries)));
    long gamemode_entry_indices[2] =
    {
        rand_range(1, g_anvil_session_vote_pool.gamemode_entries.get_count() - 1), // CLAMP OUT CTF
        rand_range(1, g_anvil_session_vote_pool.gamemode_entries.get_count() - 1)  // CLAMP OUT CTF
    };
    assert(VALID_INDEX(gamemode_entry_indices[0], NUMBEROF(s_anvil_session_vote_pool::gamemode_entries)));
    assert(VALID_INDEX(gamemode_entry_indices[1], NUMBEROF(s_anvil_session_vote_pool::gamemode_entries)));

    c_network_session_parameters* parameters = session->get_session_parameters();
    s_network_session_parameter_lobby_vote_set lobby_vote_set = {};
    lobby_vote_set.vote_options[0].gamemode = g_anvil_session_vote_pool.gamemode_entries[gamemode_entry_indices[0]].gamemode_title_instance;
    lobby_vote_set.vote_options[0].map = g_anvil_session_vote_pool.map_entries[map_entry_indices[0]].map_title_instance;
    lobby_vote_set.vote_options[1].gamemode = g_anvil_session_vote_pool.gamemode_entries[gamemode_entry_indices[1]].gamemode_title_instance;
    lobby_vote_set.vote_options[1].map = g_anvil_session_vote_pool.map_entries[map_entry_indices[1]].map_title_instance;
    parameters->lobby_vote_set.set(&lobby_vote_set);
    e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
    parameters->dedicated_server_session_state.set(&session_state);
    parameters->countdown_timer.set(_network_game_countdown_delayed_reason_voting, 10);
}

void anvil_session_update_voting(c_network_session* session)
{
    c_network_session_membership* membership = session->get_session_membership();
    c_network_session_parameters* parameters = session->get_session_parameters();

    e_dedicated_server_session_state* dedicated_server_session_state = parameters->dedicated_server_session_state.get();
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
                    s_network_session_player* player = membership->get_player(i);
                    e_player_vote_selection vote_selection_index = player->configuration.client.vote_selection_index;
                    // if vote has changed and is valid
                    if (vote_selection_index > _player_vote_none && vote_selection_index < k_player_vote_selection_count && vote_selection_index != g_anvil_vote_selections[i])
                    {
                        s_network_session_parameter_lobby_vote_set lobby_vote_set;
                        parameters->lobby_vote_set.get(&lobby_vote_set);

                        // remove old vote if its valid
                        if (g_anvil_vote_selections[i] > _player_vote_none && g_anvil_vote_selections[i] < k_player_vote_selection_count)
                            lobby_vote_set.vote_options[g_anvil_vote_selections[i] - 1].number_of_votes--;
                        // update vote selection
                        g_anvil_vote_selections[i] = vote_selection_index;
                        // add new vote
                        lobby_vote_set.vote_options[vote_selection_index - 1].number_of_votes++;

                        // update vote set parameter
                        parameters->lobby_vote_set.set(&lobby_vote_set);
                    }
                }
            }
            // else if voting has ended
            else
            {
                // loop through each player and gather their vote selections
                char option_votes[2] = {};
                for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
                {
                    // if vote selection is valid
                    if (g_anvil_vote_selections[i] > _player_vote_none && g_anvil_vote_selections[i] < k_player_vote_selection_count)
                        option_votes[g_anvil_vote_selections[i] - 1]++;
                }
                char winning_index = (option_votes[0] > option_votes[1] ? 0 : 1);
                printf("option %d wins the vote!\n", winning_index);
                s_network_session_parameter_lobby_vote_set lobby_vote_set;
                parameters->lobby_vote_set.get(&lobby_vote_set);
                lobby_vote_set.winning_vote_index = winning_index;
                parameters->lobby_vote_set.set(&lobby_vote_set);

                // load winning map & mode
                char gamemode_index = lobby_vote_set.vote_options[winning_index].gamemode;
                e_game_engine_type engine_index = g_anvil_session_vote_pool.gamemode_entries[gamemode_index].engine_index.get();
                long variant_index = g_anvil_session_vote_pool.gamemode_entries[gamemode_index].variant_index;
                char map_index = lobby_vote_set.vote_options[winning_index].map;
                e_map_id map_id = g_anvil_session_vote_pool.map_entries[map_index].map_id;
                anvil_session_set_gamemode(session, engine_index, variant_index);
                anvil_session_set_map(map_id);

                e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
                parameters->dedicated_server_session_state.set(&session_state);
            }
        }
        // if dedi state is ready to start and the map has finished loading, start the countdown
        else if (*dedicated_server_session_state == _dedicated_server_session_state_game_start_countdown &&
            parameters->game_start_status.get()->game_start_status == _session_game_start_status_ready_leader &&
            parameters->game_start_status.get()->map_load_progress == 100)
        {
            parameters->countdown_timer.set(_network_game_countdown_delayed_reason_start, 5);
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
    for (long i = membership->get_first_player(); i != NONE; i = membership->get_next_player(i))
    {
        s_network_session_player* current_player = membership->get_player(i);
        s_player_configuration_from_host* host_configuration = &current_player->configuration.host;
        
        // display server-set loadouts in the client's UI
        //host_configuration->s3d_player_customization.override_api_data = true;
        //host_configuration->s3d_player_container.override_api_data = true;

        if (current_player->configuration.host.user_xuid == 2) // yokim
        {
            host_configuration->team_index = 0;
            host_configuration->user_selected_team_index = 0;
        }
        if (current_player->configuration.host.user_xuid == 3) // twister
        {
            host_configuration->team_index = 1;
            host_configuration->user_selected_team_index = 1;
        }

        //host_configuration->s3d_player_container.loadouts[0].primary_weapon = _none;
        //host_configuration->s3d_player_container.loadouts[0].secondary_weapon = _none;
    }
    // push update
    membership->increment_update();
}

bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration)
{
    assert(configuration != nullptr);
    assert(session != nullptr);
    bool player_data_updated = false;
    
    c_network_session_membership* membership = session->get_session_membership();
    s_network_session_player* player = membership->get_player(player_index);
    s_network_session_peer* peer = membership->get_peer(player->peer_index);
    
    /*
    // if the player's xuid is unassigned 
    // temp peer name check, we're currently relying on this to assign the right user id so we need to wait until the first peer properties update comes in and sets this
    if (configuration->user_xuid == 0 && peer->properties.peer_name.length() > 0)
    {
        // dedi host loadout
        if (player->peer_index == membership->host_peer_index() && game_is_dedicated_server())
        {
            //configuration->user_xuid = USER_SYSTEM;
            //player->controller_index = 0;
            configuration->s3d_player_customization.colors[_armor_color_primary] = 0x0F0F0F;
            configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x05286E;
            configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
            configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFF640A;
            configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFF640A;
            configuration->s3d_player_container.loadouts[0].armor_suit = _armor_pilot;
            configuration->s3d_player_container.loadouts[0].primary_weapon = _dmr_v2;
            configuration->s3d_player_container.loadouts[0].secondary_weapon = _magnum_v1;
            configuration->s3d_player_container.loadouts[0].tactical_packs[0] = _concussive_blast;
            configuration->s3d_player_container.loadouts[0].tactical_packs[1] = _invisibility;
            configuration->s3d_player_container.loadouts[0].tactical_packs[2] = _hologram;
            configuration->s3d_player_container.loadouts[0].tactical_packs[3] = _powerdrain;
            configuration->s3d_player_customization.override_api_data = true;
            configuration->s3d_player_container.override_api_data = true;
            wchar_t service_tag[5] = L"HOST";
            configuration->player_appearance.service_tag.set(service_tag);
            player_data_updated = true;
        }
        else
        {
            wchar_t player_list[16][16] = { L"zzVertigo", L"ilikemyname", L"Twister", L"Yokim" }; // player nicknames
            qword user_ids[16] = { 1724964179, 1724964187, 1724964203, 1724964208 };
            for (size_t i = 0; i < NUMBEROF(user_ids); i++)
            {
                // if a match is found
                if (peer->properties.peer_name.is_equal(player_list[i]))
                {
                    configuration->user_xuid = user_ids[i];
                    // set dev service tag
                    if (i == 0)
                    {
                        wchar_t service_tag[5] = L"DEV";
                        configuration->player_appearance.service_tag.set(service_tag);
                    }
                    player_data_updated = true;
                    break;
                }
            }
        }
    }
    */
    if (!configuration->s3d_player_customization.override_api_data && configuration->user_xuid != USER_SYSTEM && configuration->user_xuid > USER_INVALID)
    {
        // assign player name based on peer name - TODO: THIS IS TEMPORARY, RETRIEVE THIS FROM API W/ USER ID INSTEAD
        configuration->player_name.set(peer->properties.peer_name.get_string());

        s_api_user_customisation* customisation = user_get_customisation_from_api(configuration->user_xuid);
        if (customisation != nullptr)
        {
            customisation->write_colours(&configuration->s3d_player_customization);
        }
        else
        {
            printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: anvil_assign_player_loadout: failed to retrieve user customisation from API for user [%lld]!\n", configuration->user_xuid);
        }
        for (long i = 0; i < k_maximum_loadouts; i++)
        {
            s_api_user_loadout* loadout = user_get_loadout_from_api(configuration->user_xuid, i);
            if (loadout != nullptr)
            {
                loadout->write_configuration(&configuration->s3d_player_container.loadouts[i]);
            }
            else
            {
                printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: anvil_assign_player_loadout: failed to retrieve user loadout '%d' from API for user [%lld]!\n", i, configuration->user_xuid);
            }
        }
        // TODO: modifiers
        //configuration->s3d_player_container.modifiers[0].modifier_values[_enable_nemesis_mechanics] = true;
        //configuration->s3d_player_container.modifiers[0].modifier_values[_grenade_warning] = false;
        membership->increment_update();
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

// map_name is optional to retrieve a map id from a .map
void anvil_launch_scenario(const char* scenario_path, const wchar_t* map_name)
{
    assert(scenario_path != nullptr);
    g_tutorial_scenario_path.set(scenario_path);
    if (map_name != nullptr)
    {
        g_tutorial_map_name.set(map_name);
    }
    else
    {
        g_tutorial_map_name.set(L"no .map provided");
    }

    long scnr_path_address = (long)g_tutorial_scenario_path.get_string();
    memcpy((void*)BASE_ADDRESS(0x33AB0D), &scnr_path_address, 4);
    memcpy((void*)BASE_ADDRESS(0x33AB58), &scnr_path_address, 4);

    long map_name_address = (long)g_tutorial_map_name.get_string();
    memcpy((void*)BASE_ADDRESS(0xDD176), &map_name_address, 4);

    printf("anvil_launch_scenario(): launching %s (%ls)\n", g_tutorial_scenario_path.get_string(), g_tutorial_map_name.get_string());
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

void anvil_boot_peer(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
        return;

    // If we're in a game, display boot message for all associated players
    if (life_cycle_globals.state_manager.get_current_state() == _life_cycle_state_in_game)
    {
        s_network_session_peer* peer = session->get_session_membership()->get_peer(peer_index);
        for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
        {
            if (peer->player_mask.test(player_absolute_index))
            {
                datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
                if (player_index != NONE)
                {
                    s_game_engine_event_data event_data;
                    game_engine_initialize_event(_multiplayer_event_type_general, STRING_ID(game_engine, general_event_player_booted_player), &event_data);
                    game_engine_set_event_effect_player_and_team(player_index, &event_data);
                    game_engine_send_event(&event_data);
                }
            }
        }
    }
    session->host_boot_machine(peer_index, _network_session_boot_reason_banned);
}

void anvil_power_sister(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    if (!session->get_session_membership()->is_peer_valid(peer_index))
        return;

    s_network_session_peer* peer = session->get_session_membership()->get_peer(peer_index);
    for (short player_absolute_index = 0; player_absolute_index < k_maximum_players; player_absolute_index++)
    {
        if (peer->player_mask.test(player_absolute_index))
        {
            datum_index player_index = player_index_from_absolute_player_index(player_absolute_index);
            if (player_index != NONE)
            {
                s_network_session_player* player = session->get_session_membership()->get_player(player_absolute_index);
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
                session->get_session_membership()->increment_update();
            }
        }
    }
}