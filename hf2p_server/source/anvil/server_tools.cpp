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
        static e_player_vote_selection vote_selections[k_maximum_multiplayer_players] {};
        
        // debug server controls
        if (anvil_key_pressed(VK_NEXT, &key_held_pgdown)) // begin voting
        {
            printf("Starting vote...\n");
            s_network_session_parameter_lobby_vote_set lobby_vote_set = {};
            lobby_vote_set.vote_options[0].gamemode = 1; // slayer small
            lobby_vote_set.vote_options[0].map = 3; // guardian
            lobby_vote_set.vote_options[1].gamemode = 1; // slayer small
            lobby_vote_set.vote_options[1].map = 4; // valhalla
            parameters->lobby_vote_set.set(&lobby_vote_set);
            e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
            parameters->dedicated_server_session_state.set(&session_state);
            parameters->countdown_timer.set(_network_game_countdown_delayed_reason_voting, 10);
        }
        else if (anvil_key_pressed(VK_HOME, &key_held_home))
        {
            printf("Launching session...\n");
            parameters->session_mode.set(_network_session_mode_setup);
        }
        else if (anvil_key_pressed(VK_PRIOR, &key_held_pgup))
        {
            printf("Running test command...\n");

            anvil_boot_peer(1);

            printf("Command finished!\n");

            //printf("Setting test player data...\n");
            //anvil_session_set_test_player_data(membership);
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
            anvil_session_set_gamemode(network_session, _game_engine_type_slayer, 1);
            anvil_session_set_map(_riverworld);
            //printf("Setting test player data...\n");
            //anvil_session_set_test_player_data(membership);
            
            //printf("TEST: loading campaign file...\n");
            //s_file_reference file_reference;
            //file_reference_create_from_path(&file_reference, L"maps\\halo3.campaign", false);
            //levels_add_campaign(&file_reference);
        }

        // update dedicated server state
        if (!parameters->dedicated_server_session_state.get_allowed()) return; // skip if data is not available

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
                        if (vote_selection_index > _player_vote_none && vote_selection_index < k_player_vote_selection_count && vote_selection_index != vote_selections[i])
                        {
                            s_network_session_parameter_lobby_vote_set lobby_vote_set;
                            parameters->lobby_vote_set.get(&lobby_vote_set);

                            // remove old vote if its valid
                            if (vote_selections[i] > _player_vote_none && vote_selections[i] < k_player_vote_selection_count)
                                lobby_vote_set.vote_options[vote_selections[i] - 1].number_of_votes--;
                            // update vote selection
                            vote_selections[i] = vote_selection_index;
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
                    byte option_votes[2] = {};
                    for (long i = membership->get_first_player(); i != -1; i = membership->get_next_player(i))
                    {
                        // if vote selection is valid
                        if (vote_selections[i] > _player_vote_none && vote_selections[i] < k_player_vote_selection_count)
                            option_votes[vote_selections[i] - 1]++;
                    }
                    byte winning_index = (option_votes[0] > option_votes[1] ? 0 : 1);
                    printf("option %d wins the vote!\n", winning_index);
                    s_network_session_parameter_lobby_vote_set lobby_vote_set;
                    parameters->lobby_vote_set.get(&lobby_vote_set);
                    lobby_vote_set.winning_vote_index = winning_index;
                    parameters->lobby_vote_set.set(&lobby_vote_set);

                    // TODO: retrieve voting options from title instances - pull map id and use with anvil_session_set_map
                    if (winning_index == 0)
                    {
                        anvil_session_set_gamemode(network_session, _game_engine_type_slayer, 0);
                        anvil_session_set_map(_guardian);
                    }
                    else if (winning_index == 1)
                    {
                        anvil_session_set_gamemode(network_session, _game_engine_type_slayer, 0);
                        anvil_session_set_map(_riverworld);
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
                parameters->countdown_timer.set(_network_game_countdown_delayed_reason_start, 5);
            }
            // dedi state is set to in game by c_life_cycle_state_handler_in_game::enter, and reset to waiting for players in c_life_cycle_state_handler_in_game::exit
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

bool anvil_session_set_gamemode(c_network_session* session, e_game_engine_type engine_index, long variant_index)
{
    c_game_variant game_variant = c_game_variant();
    if (!game_engine_tag_defined_variant_get_built_in_variant(engine_index, variant_index, &game_variant))
    {
        printf("Failed to get game variant!");
        return false;
    }

    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_round_limit(3);
    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_early_victory_win_count(1);
    //game_variant.get_slayer_variant_writeable()->set_score_to_win(1);
    //game_variant.get_active_variant_writeable()->get_miscellaneous_options_writeable()->set_round_time_limit_minutes(1);
    //game_variant.get_slayer_variant_writeable()->set_suicide_points(1);
    //game_variant.get_active_variant_writeable()->get_map_override_options_writeable()->get_base_player_traits_writeable()->get_appearance_traits_writeable()->set_active_camo_setting(_active_camo_setting_poor, false);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->get_respawn_player_traits_writeable()->get_appearance_traits_writeable()->set_active_camo_setting(_active_camo_setting_invisible, false);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_respawn_growth_seconds(15);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_respawn_player_traits_duration_seconds(60);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_team_lives_per_round(2);
    //game_variant.get_active_variant_writeable()->get_respawn_options_writeable()->set_lives_per_round(2);

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

// TODO: PULL THIS DATA FROM THE API RATHER THAN HARDCODING IT
bool anvil_assign_player_loadout(c_network_session* session, long player_index, s_player_configuration_from_host* configuration)
{
    assert(configuration != nullptr);
    assert(session != nullptr);
    bool player_data_updated = false;
    
    c_network_session_membership* membership = session->get_session_membership();
    s_network_session_player* player = membership->get_player(player_index);
    s_network_session_peer* peer = membership->get_peer(player->peer_index);
    // if the player's xuid is unassigned 
    // temp peer name check, we're currently relying on this to assign the right user id so we need to wait until the first peer properties update comes in and sets this
    if (configuration->user_xuid == 0 && peer->properties.peer_name.length() > 0)
    {        
        // assign player name based on peer name - TODO: THIS IS TEMPORARY, WE NEED A MORE RIGOROUS WAY OF VERIFYING USER CREDENTIALS
        configuration->player_name.set(peer->properties.peer_name.get_string());

        // assign temporary hardcoded loadout data
        configuration->s3d_player_customization.colors[_armor_color_primary] = 0xFF230A; // orange red
        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0xFFFFFF; // white
        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
        configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFF640A;
        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFF640A;
        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_air_assault;
        configuration->s3d_player_container.loadouts[0].primary_weapon = _dmr_v2;
        configuration->s3d_player_container.loadouts[0].secondary_weapon = _magnum_v1;
        configuration->s3d_player_container.loadouts[0].tactical_packs[0] = _concussive_blast;
        configuration->s3d_player_container.loadouts[0].tactical_packs[1] = _invisibility;
        configuration->s3d_player_container.loadouts[0].tactical_packs[2] = _hologram;
        configuration->s3d_player_container.loadouts[0].tactical_packs[3] = _powerdrain;
        configuration->s3d_player_container.modifiers[0].modifier_values[_revenge_shield_boost] = 1.0f;
        player_data_updated = true;

        // dedi host loadout
        if (player->peer_index == membership->host_peer_index() && game_is_dedicated_server())
        {
            configuration->user_xuid = 3; // -1 SYSTEM/invalid player id, 0 empty ID
            player->controller_index = 0;
            configuration->s3d_player_customization.colors[_armor_color_primary] = 0x0F0F0F;
            configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x05286E;
            configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
            configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFF640A;
            configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFF640A;
            configuration->s3d_player_container.loadouts[0].armor_suit = _armor_pilot;
            //configuration->s3d_player_container.loadouts[0].secondary_weapon = _gravity_hammer;
            //configuration->s3d_player_container.modifiers[0].modifier_values[_detonate_on_player_cdt] = 1.0f;
            //configuration->s3d_player_container.modifiers[0].modifier_values[_detonate_on_vehicle_cdt] = 1.0f;
            configuration->s3d_player_customization.override_api_data = true;
            configuration->s3d_player_container.override_api_data = true;
            wchar_t service_tag[5] = L"HOST";
            configuration->player_appearance.service_tag.set(service_tag);
            player_data_updated = true;
        }
        // standard player xuids here
        else
        {
            // make sure these appear in API user id order
            wchar_t player_list[16][16] = { L"zzVertigo", L"Yokim", L"Twister", L"Berth", L"", L"ilikemyname", L"", L"", L"", L"", L"", L"", L"", L"", L"", L""};
            for (size_t i = 0; i < 16; i++)
            {
                // if a match is found
                if (peer->properties.peer_name.is_equal(player_list[i]))
                {
                    configuration->user_xuid = i + 1;
                    // set dev service tag
                    if (i == 0 || i == 1 || i == 2)
                    {
                        wchar_t service_tag[5] = L"DEV";
                        configuration->player_appearance.service_tag.set(service_tag);
                    }
                    else if (i == 3)
                    {
                        wchar_t service_tag[5] = L"BFB";
                        configuration->player_appearance.service_tag.set(service_tag);
                    }

                    if (i == 0) // zz
                    {
                        configuration->s3d_player_customization.colors[_armor_color_primary] = 0x0F0F0F;
                        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x00AAF0;
                        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
                        configuration->s3d_player_customization.colors[_armor_color_lights] = 0x00B4FF;
                        configuration->s3d_player_customization.colors[_armor_color_holo] = 0x00B4FF;
                        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_ninja_rare;
                    }
                    else if (i == 1) // yokim
                    {
                        configuration->s3d_player_customization.colors[_armor_color_primary] = 0x0F0F0F;
                        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x05286E;
                        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
                        configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFF640A;
                        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFF640A;
                        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_pilot;
                    }
                    else if (i == 2) // twist
                    {
                        configuration->s3d_player_customization.colors[_armor_color_primary] = 0xFF230A;
                        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0xFFFFFF;
                        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xF5EB05;
                        configuration->s3d_player_customization.colors[_armor_color_lights] = 0xF5EB05;
                        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xF5EB05;
                        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_renegade_rare;
                    }
                    else if (i == 3)
                    {
                        configuration->s3d_player_customization.colors[_armor_color_primary] = 0xFFFFFFF;
                        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x0F0F0F;
                        configuration->s3d_player_customization.colors[_armor_color_visor] = 0x00B4FF;
                        configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFF640A;
                        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFF640A;
                        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_air_assault;
                    }
                    else if (i == 5)
                    {
                        configuration->s3d_player_customization.colors[_armor_color_primary] = 0xFFFFFF;
                        configuration->s3d_player_customization.colors[_armor_color_secondary] = 0x0F0F0F;
                        configuration->s3d_player_customization.colors[_armor_color_visor] = 0xFF640A;
                        configuration->s3d_player_customization.colors[_armor_color_lights] = 0xFFFFFFF;
                        configuration->s3d_player_customization.colors[_armor_color_holo] = 0xFFFFFFF;
                        configuration->s3d_player_container.loadouts[0].armor_suit = _armor_air_assault;
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

void anvil_boot_peer(long peer_index)
{
    c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
    s_network_session_peer* peer = session->get_session_membership()->get_peer(peer_index);

    // If we're in a game, display boot message for all associated players
    if (life_cycle_globals.state_manager.get_current_state() == _life_cycle_state_in_game)
    {
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