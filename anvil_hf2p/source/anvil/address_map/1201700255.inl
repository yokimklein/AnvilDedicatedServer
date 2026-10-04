#pragma once

// Engine address map for 12.1.700255 cert_ms30_oct19

#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)


// ai\path.cpp
#define ADDRESS_AI_SCRATCH_ALLOCATE                                      0x6D4170 // ai_scratch_allocate
#define ADDRESS_AI_SCRATCH_FREE                                          0x6D4220 // ai_scratch_free
#define ADDRESS_AI_POINT3D_NEW_TARGET                                    0x6EE1B0 // ai_point3d_new
#define ADDRESS_PATH_STATE_NEW_TARGET                                    0x75CF90 // path_state_new
#define ADDRESS_PATH_STATE_DESTINATION                                   0x75D160 // path_state_destination
#define ADDRESS_PATH_STATE_FIND                                          0x75D490 // path_state_find
#define ADDRESS_PATH_STATE_BUILD_PATH_TARGET                             0x75C2E0 // path_state_build_path
#define ADDRESS_COLLISION_SURFACE_GET_SECTOR                             0x701980 // collision_surface_get_sector
#define ADDRESS_OBJECT_GET_PATHFINDING_LOCATION_TARGET                   0x739090 // object_get_pathfinding_location

// algorithms\binary_search.cpp
#define ADDRESS_BINARY_SEARCH_ELEMENTS                                   0x1669B0 // binary_search_elements

// anvil\hooks\effects\hooks_effect_system.cpp
#define ADDRESS_WRITE_PARTICLE_STATE                                     0x5BF0A0 // write_particle_state
#define ADDRESS_G_PARTICLE_STATE_WRITE_BUFFER                            0x2484AC8
#define ADDRESS_G_PARTICLE_STATE_WRITE_BUFFER_STRIDE                     0x2512AD0
#define ADDRESS_WRITE_PARTICLE_STATE_CALL                                0x5C005B
#define ADDRESS_WRITE_PARTICLE_STATE_CALL_2                              0x5C0168

// anvil\hooks\hooks.cpp
#define ADDRESS_CACHE_FILE_HEADER_VERIFY_PATCH                           0x083934 // cache_file_header_verify+0x104
#define ADDRESS_TAG_LOAD_CHECKSUM_NOP                                    0x083CC1
#define ADDRESS_SCENARIO_TAGS_LOAD_NOP                                   0x0847A9 // scenario_tags_load+0x2fc
#define ADDRESS_ENGLISH_LANGUAGE_PATCH                                   0x2C73DE
#define ADDRESS_SCENARIO_TAGS_LOAD_RESET_HOOK                            0x0844D1 // scenario_tags_load+0x151, mov byte_3010B8D, 0 in the tag table reset branch
#define ADDRESS_SCENARIO_TAGS_LOAD_RESET_HOOK_RETURN                     0x0844D8 // scenario_tags_load+0x158
#define ADDRESS_CACHE_FILE_RESIDENT_TAGS_LOADED                          0x2C10B8C // byte_3010B8C, set once the tags.dat global tags (tag 0 closure) are resident

// anvil\hooks\hooks_debug.cpp
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_HOOK                       0x1712C0 // font_cache_retrieve_character
#define ADDRESS_MAIN_GAME_RESET_MAP_HOOK                                 0x0AAF73 // main_game_reset_map+0x1b3
#define ADDRESS_MAIN_GAME_RESET_MAP_HOOK_RETURN                          0x0AAF78 // main_game_reset_map+0x1b8
#define ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK                          0x0AB263 // main_game_change_immediate+0xd3
#define ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK_RETURN                   0x0AB268 // main_game_change_immediate+0xd8
#define ADDRESS_SHELL_INITIALIZED_HOOK                                   0x001045 // shell_initialize+0x35
#define ADDRESS_SHELL_INITIALIZED_HOOK_RETURN                            0x00104D // shell_initialize+0x3d
#define ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK                          0x27E9C9 // c_player_view::render+0x818
#define ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK_RETURN                   0x27E9CE // c_player_view::render+0x81d
#define ADDRESS_RENDER_INITIALIZE_HOOK                                   0x275C9B // vision_mode_initialize+0xeb
#define ADDRESS_RENDER_INITIALIZE_HOOK_RETURN                            0x275CA0 // vision_mode_initialize+0xf0
#define ADDRESS_MAIN_LOOP_BODY_HOOK2                                     0x09692E // main_loop_body+0x1e
#define ADDRESS_MAIN_LOOP_BODY_HOOK2_RETURN                              0x096933 // main_loop_body+0x23
#define ADDRESS_GAME_TICK_HOOK1                                          0x0B369B // game_tick+0x6b
#define ADDRESS_GAME_TICK_HOOK1_RETURN                                   0x0B36A2 // game_tick+0x72
#define ADDRESS_GAME_TICK_HOOK2                                          0x0B38D3 // game_tick+0x2a3
#define ADDRESS_GAME_TICK_HOOK2_RETURN                                   0x0B38DB // game_tick+0x2ab
#define ADDRESS_MAIN_LOOP_BODY_HOOK1                                     0x096999 // main_loop_body+0xb3
#define ADDRESS_MAIN_LOOP_BODY_HOOK1_RETURN                              0x09699E // main_loop_body+0xb8
#define ADDRESS_MAIN_LOOP_ENTER_HOOK1                                    0x0963A8 // main_loop_enter+0x78
#define ADDRESS_MAIN_LOOP_ENTER_HOOK1_RETURN                             0x0963AD // main_loop_enter+0x7d
#define ADDRESS_MAIN_LOOP_ENTER_HOOK2                                    0x096446 // main_loop_enter+0x116
#define ADDRESS_MAIN_LOOP_ENTER_HOOK2_RETURN                             0x09644B // main_loop_enter+0x11b
#define ADDRESS_MAIN_LOOP_EXIT_HOOK                                      0x096F21 // main_loop_exit+0x71
#define ADDRESS_MAIN_LOOP_EXIT_HOOK_RETURN                               0x096F2B // main_loop_exit+0x7b
#define ADDRESS_RENDER_DEBUG_FRAME_RENDER_CALL                           0x169ED8 // main_render_game+0x5e1
#define ADDRESS_PRINT_HS_PRINT_1_EVALUATE_SLOT                           0xD86EA8
#define ADDRESS_LOG_PRINT_HS_LOG_PRINT_1_EVALUATE_SLOT                   0xD88C60
#define ADDRESS_EVENTS_SUPPRESS_DISPLAY_EVENTS_SUPPRESS_OUTPUT_1_EVALUATE_SLOT 0xD892E8
#define ADDRESS_MAIN_LOOP_BODY_HOOK3                                     0x096A13 // main_loop_body+0x12d
#define ADDRESS_MAIN_LOOP_BODY_HOOK3_RETURN                              0x096A19 // main_loop_body+0x133
#define ADDRESS_MAIN_LOOP_BODY_HOOK4                                     0x096BE2 // main_loop_body+0x2cd
#define ADDRESS_MAIN_LOOP_BODY_HOOK4_RETURN                              0x096BE9 // main_loop_body+0x2d4
#define ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK1                        0x096AA4 // main_loop_body+0x194, call input_update (game path)
#define ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK1_RETURN                 0x096AA9 // main_loop_body+0x199
#define ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK2                        0x096C1F // main_loop_body+0x30f, call input_update (pregame path)
#define ADDRESS_MAIN_LOOP_BODY_INPUT_UPDATE_HOOK2_RETURN                 0x096C24 // main_loop_body+0x314
#define ADDRESS_RUMBLE_UPDATE_HOOK                                       0x16A296 // rumble_update+0x6
#define ADDRESS_RUMBLE_UPDATE_HOOK_RETURN                                0x16A29D // rumble_update+0xd
#define ADDRESS_FONT_INITIALIZE_HOOK                                     0x09FCB7 // font_initialize+0x27
#define ADDRESS_FONT_INITIALIZE_HOOK_RETURN                              0x09FCBC // font_initialize+0x2c
#define ADDRESS_FONT_INITIALIZE_EMERGENCY                                0x09FBE0 // font_initialize_emergency
#define ADDRESS_FONT_LOADING_IDLE_HOOK                                   0x0A0061 // font_loading_idle+0x1
#define ADDRESS_FONT_LOADING_IDLE_HOOK_RETURN                            0x0A0066 // font_loading_idle+0x6
#define ADDRESS_FONT_LOADING_IDLE_NOP                                    0x0A00C3 // font_loading_idle+0x63
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL                       0x17110F // font_cache_load_internal+0x5f
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_2                     0x171197 // font_cache_load_internal+0xe7
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_3                     0x1711B8 // font_cache_load_internal+0x108
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_4                     0x1711D3 // font_cache_load_internal+0x123
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP                             0x171117 // font_cache_load_internal+0x67
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_2                           0x17119E // font_cache_load_internal+0xee
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_3                           0x1711BF // font_cache_load_internal+0x10f
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_4                           0x1711D8 // font_cache_load_internal+0x128
#define ADDRESS_C_DRAW_STRING_CTOR_HOOK                                  0x171BA2 // ?0c_draw_string+0x22
#define ADDRESS_C_DRAW_STRING_CTOR_HOOK_RETURN                           0x171BCA // ?0c_draw_string+0x4a
#define ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK                         0x172948 // c_draw_string::draw_internal+0xa8
#define ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK_RETURN                  0x172975 // c_draw_string::draw_internal+0xd5
#define ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK                      0x172E79 // c_draw_string::parse_string_new+0x69
#define ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK_RETURN               0x172EA5 // c_draw_string::parse_string_new+0x95
#define ADDRESS_C_DRAW_STRING_SET_FONT                                   0x1721B0 // c_draw_string::set_font
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK                        0x0A08C9 // main_time_frame_rate_display+0x99
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK_RETURN                 0x0A0933 // main_time_frame_rate_display+0x103
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_NOP                         0x0A0939 // main_time_frame_rate_display+0x109
#define ADDRESS_DIRECTOR_RENDER_HOOK                                     0x0E52B2 // director_render+0x2b2
#define ADDRESS_DIRECTOR_RENDER_HOOK_RETURN                              0x0E52E2 // director_render+0x2e2
#define ADDRESS_DIRECTOR_RENDER_NOP                                      0x0E52EE // director_render+0x2ee
#define ADDRESS_SUBTITLE_RENDER_HOOK                                     0x17B96F // subtitle_render+0x7f
#define ADDRESS_SUBTITLE_RENDER_HOOK_RETURN                              0x17B99F // subtitle_render+0xaf
#define ADDRESS_SUBTITLE_RENDER_NOP                                      0x17B9AE // subtitle_render+0xbe
#define ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK                 0x1B6D4D // game_engine_render_watermarks+0x42d
#define ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK_RETURN          0x1B6DA0 // game_engine_render_watermarks+0x480
#define ADDRESS_GAME_ENGINE_RENDER_WATERMARKS_NOP                        0x1B6DB8 // game_engine_render_watermarks+0x498
#define ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK                              0x276C6C // render_fullscreen_text+0x7c
#define ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK_RETURN                       0x276C9B // render_fullscreen_text+0xab
#define ADDRESS_RENDER_FULLSCREEN_TEXT_NOP                               0x276C9E // render_fullscreen_text+0xae
#define ADDRESS_CHUD_GET_STRING_WIDTH_HOOK                               0x3EEF74 // chud_get_string_width+0xa4
#define ADDRESS_CHUD_GET_STRING_WIDTH_HOOK_RETURN                        0x3EEF9C // chud_get_string_width+0xcc
#define ADDRESS_CHUD_GET_STRING_WIDTH_NOP                                0x3EEF62 // chud_get_string_width+0x92
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK           0x3FAADA // c_user_interface_text::compute_text_bounds+0x14a
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK_RETURN    0x3FAB0C // c_user_interface_text::compute_text_bounds+0x17c
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP            0x3FAAAC // c_user_interface_text::compute_text_bounds+0x11c
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_2          0x3FAAB4 // c_user_interface_text::compute_text_bounds+0x124
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_3          0x3FAAC4 // c_user_interface_text::compute_text_bounds+0x134
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK                        0x3FA709 // c_user_interface_text::render+0x179
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK_RETURN                 0x3FA735 // c_user_interface_text::render+0x1a5
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP                         0x3FA67C // c_user_interface_text::render+0xec
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_2                       0x3FA691 // c_user_interface_text::render+0x101
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_3                       0x3FA6B4 // c_user_interface_text::render+0x124
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_4                       0x3FA753 // c_user_interface_text::render+0x1c3
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK                            0x4018A6 // chud_build_text_geometry+0xa6
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK_RETURN                     0x4018E4 // chud_build_text_geometry+0xe4
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP                             0x401891 // chud_build_text_geometry+0x91
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP_2                           0x4018FF // chud_build_text_geometry+0xff
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK                            0x1710C5 // font_cache_load_internal+0x15
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK_RETURN                     0x1710D9 // font_cache_load_internal+0x29
#define ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK                       0x26C4D8 // hardware_cache_load_character+0x28
#define ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK_RETURN                0x26C4EE // hardware_cache_load_character+0x3e
#define ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK                    0x26C3D3 // hardware_cache_predict_character+0x13
#define ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK_RETURN             0x26C3F0 // hardware_cache_predict_character+0x30
#define ADDRESS_SHELL_DISPOSE_HOOK                                       0x001224 // shell_dispose+0xda
#define ADDRESS_SHELL_DISPOSE_HOOK_RETURN                                0x001229 // shell_dispose+0xdf
#define ADDRESS_DIRECTOR_UPDATE_HOOK                                     0x0E4EBD // director_update+0x9d
#define ADDRESS_DIRECTOR_UPDATE_HOOK_RETURN                              0x0E4EC3 // director_update+0xa3
#define ADDRESS_C_DEBUG_DIRECTOR_UPDATE_HOOK_SLOT                        0xDBE850
#define ADDRESS_EXCEPTIONS_UPDATE                                        0x16D740 // exceptions_update
#define ADDRESS_TOPLEVELEXCEPTIONFILTER                                  0x2C1710 // TopLevelExceptionFilter
#define ADDRESS_MAIN_HALT_AND_CATCH_FIRE                                 0x097D40 // main_halt_and_catch_fire

// anvil\hooks\hooks_ds.cpp
#define ADDRESS_ANVIL_SESSION_UPDATE_HOOK                                0x024581 // network_update+0x51
#define ADDRESS_ANVIL_SESSION_UPDATE_HOOK_RETURN                         0x024586 // network_update+0x56
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_GAME_START_STATUS_SET        0x03B960 // c_network_session_parameter_game_start_status::set
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_PRE_GAME_SQUAD_GAME_START_STATUS_UPDATE_HOOK 0x04DE64 // c_life_cycle_state_handler_pre_game::squad_game_start_status_update+0x954
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_PRE_GAME_SQUAD_GAME_START_STATUS_UPDATE_HOOK_RETURN 0x04DEC9 // c_life_cycle_state_handler_pre_game::squad_game_start_status_update+0x9b9
#define ADDRESS_TRANSPORT_SECURE_KEY_CREATE                              0x003BC0 // transport_secure_key_create
#define ADDRESS_TRANSPORT_SECURE_ADDRESS_RESOLVE                         0x003C50 // transport_secure_address_resolve
#define ADDRESS_PEER_REQUEST_PLAYER_ADD_CALL                             0x02F4FC // network_session_interface_update_session+0x19c
#define ADDRESS_NETWORK_SESSION_INTERFACE_GET_LOCAL_USER_IDENTIFIER_CALL 0x02124C // c_network_session::create_host_session+0x31c
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_ENTER_HOOK            0x04EA09 // c_life_cycle_state_handler_in_game::enter+0x139
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_ENTER_HOOK_RETURN     0x04EA13 // c_life_cycle_state_handler_in_game::enter+0x143
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_EXIT_HOOK             0x04EA9B // c_life_cycle_state_handler_in_game::exit+0x7b
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_EXIT_HOOK_RETURN      0x04EAA2 // c_life_cycle_state_handler_in_game::exit+0x82
#define ADDRESS_ANVIL_SCENARIO_TAGS_LOAD_TITLE_INSTANCES                 0x07EFE6 // scenario_load+0xa8
#define ADDRESS_ANVIL_SCENARIO_TAGS_LOAD_TITLE_INSTANCES_RETURN          0x07F01B // scenario_load+0xdd
#define ADDRESS_HF2P_GAME_UPDATE_NOP                                     0x2BD861 // hf2p_game_update+0x1
#define ADDRESS_HF2P_SECURITY_INITIALIZE_NOP                             0x2BCDB6 // hf2p_security_initialize+0x6
#define ADDRESS_MAIN_LOOP_EXIT_NOP                                       0x096F0F // main_loop_exit+0x5f
#define ADDRESS_MAIN_LOOP_EXIT_NOP_2                                     0x096F14 // main_loop_exit+0x64
#define ADDRESS_MAIN_LOOP_PREGAME_NOP                                    0x0971D7 // main_loop_pregame+0x87
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL                             0x028684 // managed_session_synchronize_to_player_list+0x164
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_2                           0x0294F7 // managed_session_successful_players_remove_complete+0x27
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_3                           0x030E33 // c_network_session_membership::remove_peer+0x93
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_4                           0x0319F9 // c_network_session_membership::remove_player+0x49
#define ADDRESS_MANAGED_SESSION_SYNCHRONIZE_TO_PLAYER_LIST_NOP           0x028689 // managed_session_synchronize_to_player_list+0x169
#define ADDRESS_MANAGED_SESSION_SUCCESSFUL_PLAYERS_REMOVE_COMPLETE_NOP   0x0294FC // managed_session_successful_players_remove_complete+0x2c
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PEER_NOP             0x030E4B // c_network_session_membership::remove_peer+0xab
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_NOP           0x031A07 // c_network_session_membership::remove_player+0x57
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_START_GAME_ENTER_HOOK         0x04C35F // c_life_cycle_state_handler_start_game::enter+0xf
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_START_GAME_ENTER_HOOK_RETURN  0x04C365 // c_life_cycle_state_handler_start_game::enter+0x15
#define ADDRESS_CHUD_UPDATE_USER_DATA_HOOK                               0x3D0758 // c_chud_update_user_data::c_chud_update_user_data+0xa41
#define ADDRESS_CHUD_UPDATE_USER_DATA_HOOK_RETURN                        0x3D075E // c_chud_update_user_data::c_chud_update_user_data+0xa47
#define ADDRESS_UNIT_HANDLE_EQUIPMENT_ENERGY_COST                        0x44D9D0 // unit_handle_equipment_energy_cost
#define ADDRESS_PLAYER_CAN_USE_CONSUMABLE                                0x0C1710 // player_can_use_consumable
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE   0x04C580 // c_life_cycle_state_handler_end_game_write_stats::update

// anvil\hooks\hooks_miscellaneous.cpp
#define ADDRESS_ENCODE_MESSAGE_HEADER_HOOK                               0x0386E0 // c_network_message_type_collection::encode_message_header
#define ADDRESS_CONTRAIL_FIX_OPERAND_VA                                  0x6968FA // c_contrail_gpu::render
#define ADDRESS_CONTRAIL_FIX_SKIP_VA                                     0x696953 // c_contrail_gpu::render
#define ADDRESS_CONTRAIL_FIX_RENDER_VA                                   0x696900 // c_contrail_gpu::render
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA                       0x32DEC0 // hf2p_set_biped_texture_render_data
#define ADDRESS_C_STATIC_STRING_64_PRINT_CALL                            0x0013A3 // main_thread_routine+0x53
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL                               0x016AC8 // c_network_message_queue::send_message_in_first_fragment+0x78
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL_2                             0x016BF6 // c_network_message_queue::send_message+0x76
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL_3                             0x023354 // c_network_message_gateway::send_message_directed+0x114
#define ADDRESS_HF2P_PLAYER_PODIUM_INITIALIZE                            0x3160A0 // hf2p_player_podium_initialize
#define ADDRESS_HF2P_PODIUM_TICK_HOOK                                    0x3175BA // hf2p_podium_update+0xba
#define ADDRESS_HF2P_PODIUM_TICK_HOOK_RETURN                             0x3175BF // hf2p_podium_update+0xbf
#define ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK 0x06518C // c_simulation_player_taunt_request_event_definition::apply_game_event+0x5c
#define ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK_RETURN 0x06519E // c_simulation_player_taunt_request_event_definition::apply_game_event+0x6e
#define ADDRESS_GAME_ENGINE_RENDER_WATERMARKS                            0x1B6920 // game_engine_render_watermarks
#define ADDRESS_VSNPRINTF_S_NET_DEBUG_CALL                               0x58C3FF
#define ADDRESS_STRING_ID_INITIALIZE                                     0x08499C // shell_initialize+0xfc
#define ADDRESS_STRING_ID_INITIALIZE_RETURN                              0x0849A1 // shell_initialize+0x101
#define ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK                           0x32F888
#define ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK_RETURN                    0x32F88D

// anvil\hooks\hooks_session.cpp
#define ADDRESS_MEMBERSHIP_UPDATE_MESSAGE                                0x406D280 // membership_update_message
#define ADDRESS_HANDLE_OUT_OF_BAND_MESSAGE                               0x025090 // c_network_message_handler::handle_out_of_band_message
#define ADDRESS_HANDLE_CHANNEL_MESSAGE                                   0x025270 // c_network_message_handler::handle_channel_message
#define ADDRESS_NETWORK_JOIN_PROCESS_JOINS_FROM_QUEUE                    0x02A510 // network_join_process_joins_from_queue
#define ADDRESS_SESSION_IDLE                                             0x021A30 // c_network_session::idle
#define ADDRESS_NETWORK_SESSION_UPDATE_PEER_PROPERTIES                   0x02F5A0 // network_session_update_peer_properties
#define ADDRESS_NETWORK_LIFE_CYCLE_END_CALL                              0x40F8BC // c_gui_location_manager::update+0xcf
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL            0x02AD2E // network_life_cycle_create_local_squad+0x9e
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL_2          0x02DBD1 // network_session_interface_update+0x21
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL             0x0212C2 // c_network_session::create_host_session+0x392
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL_2           0x027FE1 // online_session_manager_update+0x1b1
#define ADDRESS_MANAGED_SESSION_DELETE_JUMP                              0x028448 // managed_session_delete+0x28
#define ADDRESS_GAME_ENGINE_SHOULD_SPAWN_PLAYER                          0x1009C0 // game_engine_should_spawn_player
#define ADDRESS_CAN_ACCEPT_PLAYER_JOIN_REQUEST_CALL                      0x02125F // c_network_session::create_host_session+0x32f
#define ADDRESS_SESSION_DISCONNECT_CALL                                  0x021C34 // c_network_session::set_disconnection_policy+0x14
#define ADDRESS_SESSION_DISCONNECT_CALL_2                                0x02257E // c_network_session::host_connection_refused+0x5e
#define ADDRESS_SESSION_DISCONNECT_CALL_3                                0x02438E // network_initialize+0x3ae
#define ADDRESS_SESSION_DISCONNECT_CALL_4                                0x025654 // c_network_message_handler::handle_join_refuse+0x74
#define ADDRESS_SESSION_DISCONNECT_CALL_5                                0x0256A3 // c_network_message_handler::handle_leave_acknowledge+0x43
#define ADDRESS_SESSION_DISCONNECT_CALL_6                                0x02ABCC // network_life_cycle_end+0x1c
#define ADDRESS_SESSION_DISCONNECT_CALL_7                                0x02ACCD // network_life_cycle_create_local_squad+0x3d
#define ADDRESS_SESSION_DISCONNECT_CALL_8                                0x03E2CF // c_network_session::change_local_state_peer_joining+0x13f
#define ADDRESS_SESSION_DISCONNECT_CALL_9                                0x03E7D4 // _dynamic_initializer_for__module_base___5+0xa4
#define ADDRESS_SESSION_DISCONNECT_CALL_10                               0x03E970 // c_network_session::idle_peer_join_abort+0x40
#define ADDRESS_SESSION_DISCONNECT_CALL_11                               0x03E9D9 // c_network_session::idle_peer_leaving+0x49
#define ADDRESS_SESSION_DISCONNECT_CALL_12                               0x04B358 // c_network_session::handle_parameters_update+0x48
#define ADDRESS_SESSION_DISCONNECT_CALL_13                               0x04B462 // c_network_session::handle_session_disband+0x52
#define ADDRESS_SESSION_DISCONNECT_CALL_14                               0x04B478 // c_network_session::handle_session_disband+0x68
#define ADDRESS_SESSION_DISCONNECT_CALL_15                               0x04B4F2 // c_network_session::handle_session_boot+0x52
#define ADDRESS_SESSION_DISCONNECT_CALL_16                               0x04B599 // c_network_session::handle_host_decline+0x69
#define ADDRESS_SESSION_DISCONNECT_CALL_17                               0x04D07E // c_life_cycle_state_handler_pre_game__enter+0x3e
#define ADDRESS_SESSION_DISCONNECT_CALL_18                               0x04F34F // c_life_cycle_state_handler_none__enter+0x1f
#define ADDRESS_SESSION_DISCONNECT_CALL_19                               0x2FE6E9 // network_life_cycle_disconnect_all_sessions+0x29
#define ADDRESS_SESSION_DISCONNECT_CALL_20                               0x2FE707 // network_life_cycle_disconnect_all_sessions+0x47
#define ADDRESS_SESSION_DISCONNECT_CALL_21                               0x3CB0E6
#define ADDRESS_SESSION_DISCONNECT_CALL_22                               0x3CBE6C // hf2p_setup_session+0x1c
#define ADDRESS_SEND_ALL_PENDING_MESSAGES                                0x023400 // c_network_message_gateway::send_all_pending_messages

// anvil\hooks\simulation\hooks_damage_updates.cpp
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK1                               0x42DD5C // object_damage_update+0xa7e
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK1_RETURN                        0x42DDE1 // object_damage_update+0xb03
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK2                               0x42DDB4 // object_damage_update+0xad6
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK2_RETURN                        0x42DDD0 // object_damage_update+0xaf2
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK3                               0x42DE4F // object_damage_update+0xb71
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK3_RETURN                        0x42DEE6 // object_damage_update+0xc10
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK1                               0x432F0C // object_damage_shield+0x17c
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK1_RETURN                        0x432F14 // object_damage_shield+0x184
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK2                               0x4330FB // object_damage_shield+0x36b
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK2_RETURN                        0x433101 // object_damage_shield+0x371
#define ADDRESS_OBJECT_DAMAGE_BODY_HOOK1                                 0x43269C // object_damage_body+0x16c
#define ADDRESS_OBJECT_DAMAGE_BODY_HOOK1_RETURN                          0x4326A3 // object_damage_body+0x173
#define ADDRESS_OBJECT_DEPLETE_BODY_INTERNAL_HOOK1                       0x42E263 // object_deplete_body_internal+0xa3
#define ADDRESS_OBJECT_DEPLETE_BODY_INTERNAL_HOOK1_RETURN                0x42E26A // object_deplete_body_internal+0xaa
#define ADDRESS_DAMAGE_SECTION_RESPONSE_FIRE_HOOK                        0x4345DF // damage_section_response_fire+0x6f
#define ADDRESS_DAMAGE_SECTION_RESPONSE_FIRE_HOOK_RETURN                 0x4345E7 // damage_section_response_fire+0x77
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER                                  0x424D30 // object_set_damage_owner
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK2                            0x11886D // event_generate_accelerations+0x45f
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK2_RETURN                     0x118873 // event_generate_accelerations+0x465
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK3                            0x214BC2 // havok_collision_damage_update+0x837
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK3_RETURN                     0x214BC8 // havok_collision_damage_update+0x83d
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK4                            0x42F88A // object_cause_damage+0x57c
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK4_RETURN                     0x42F890 // object_cause_damage+0x582
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK5                            0x477FB5 // motor_animation_exit_seat_immediate_internal+0x575
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK5_RETURN                     0x477FBB // motor_animation_exit_seat_immediate_internal+0x57b
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK6                            0x4DF58B // vehicle_flip_submit+0x1bd
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK6_RETURN                     0x4DF591 // vehicle_flip_submit+0x1c3

// anvil\hooks\simulation\hooks_object_creation.cpp
#define ADDRESS_PLAYER_SET_FACING_PLAYER_SPAWN_CALL                      0x0BCFBC // player_spawn+0x3a4
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL                         0x0B0651 // c_map_variant::create_object+0x313
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL_2                       0x1787B6 // c_candy_spawner::spawn_object+0x3f6
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL_3                       0x429F3B // object_new_from_scenario_internal+0x13b
#define ADDRESS_EVENT_GENERATE_PART_HOOK                                 0x11978F // event_generate_part+0x78f
#define ADDRESS_EVENT_GENERATE_PART_HOOK_RETURN                          0x1197CC // event_generate_part+0x7cc
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_END                     0x45A3A2
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES                         0x457BA0 // weapon_barrel_create_projectiles
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK0                   0x457D65 // weapon_barrel_create_projectiles+0x1c4
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK0_RETURN            0x457D6C // weapon_barrel_create_projectiles+0x1cb
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK1                   0x459AF5 // weapon_barrel_create_projectiles+0x1fdd
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK1_RETURN            0x459B01 // weapon_barrel_create_projectiles+0x1fe9
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK2                   0x459DE8 // weapon_barrel_create_projectiles+0x22b2
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK2_RETURN            0x459DEE // weapon_barrel_create_projectiles+0x22b8
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_STACK_CLEANUP           0x45A39C // weapon_barrel_create_projectiles+0x285b
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_STACK_CLEANUP_RETURN    0x45A3A1 // weapon_barrel_create_projectiles+0x2860
#define ADDRESS_THROW_RELEASE_END                                        0x49DADC
#define ADDRESS_THROW_RELEASE                                            0x49D6C0 // throw_release
#define ADDRESS_THROW_RELEASE_HOOK0                                      0x49DA34 // throw_release+0x374
#define ADDRESS_THROW_RELEASE_HOOK0_RETURN                               0x49DA39 // throw_release+0x379
#define ADDRESS_THROW_RELEASE_HOOK1                                      0x49DA45 // throw_release+0x385
#define ADDRESS_THROW_RELEASE_HOOK1_RETURN                               0x49DA4D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_STACK_CLEANUP                              0x49DAD1 // throw_release+0x411
#define ADDRESS_THROW_RELEASE_STACK_CLEANUP_RETURN                       0x49DADB // throw_release+0x41b
#define ADDRESS_THROW_RELEASE_HOOK3                                      0x49DA3B // throw_release+0x37b
#define ADDRESS_THROW_RELEASE_HOOK3_RETURN                               0x49DA4D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_NOP                                        0x49DA40 // throw_release+0x380
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK                                  0x471DAA // equipment_activate+0xd6a
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK_RETURN                           0x471DB4 // equipment_activate+0xd74
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK                              0x4A4D17 // item_in_unit_inventory+0x287
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK_RETURN                       0x4A4D1D // item_in_unit_inventory+0x28d
#define ADDRESS_UNIT_DROP_PLASMA_ON_DEATH_HOOK                           0x4476A9 // unit_drop_plasma_on_death+0x139
#define ADDRESS_UNIT_DROP_PLASMA_ON_DEATH_HOOK_RETURN                    0x4476B0 // unit_drop_plasma_on_death+0x140
#define ADDRESS_CREATE_FLAG_AT_POSITION_HOOK                             0x232AD6 // create_flag_at_position+0x76
#define ADDRESS_CREATE_FLAG_AT_POSITION_HOOK_RETURN                      0x232ADB // create_flag_at_position+0x7b

// anvil\hooks\simulation\hooks_object_deletion.cpp
#define ADDRESS_OBJECT_SCRIPTING_CLEAR_ALL_FUNCTION_VARIABLES_CALL       0x41E8CE // object_delete+0xfe
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK2                             0x4A4B66 // item_in_unit_inventory+0xd6
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK2_RETURN                      0x4A4B6D // item_in_unit_inventory+0xdd

// anvil\hooks\simulation\hooks_object_updates.cpp
#define ADDRESS_OBJECT_UPDATE_HOOK                                       0x425317 // object_update+0x197
#define ADDRESS_OBJECT_UPDATE_HOOK_RETURN                                0x42531E // object_update+0x19e
#define ADDRESS_PLAYER_SET_FACING                                        0x0B8030 // player_set_facing
#define ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK                         0x0AF77B // c_map_variant::remove_object+0x8b
#define ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK_RETURN                  0x0AF795 // c_map_variant::remove_object+0xa5
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1                             0x0AD6CE // c_map_variant::unknown4+0x21e
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1_RETURN                      0x0AD6D6 // c_map_variant::unknown4+0x226
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2                             0x0AD6FA // c_map_variant::unknown4+0x24a
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2_RETURN                      0x0AD701 // c_map_variant::unknown4+0x251
#define ADDRESS_PLAYER_SET_UNIT_INDEX_CALL                               0x0B7C20 // player_set_unit_index+0x7e
#define ADDRESS_PLAYER_SET_UNIT_INDEX_CALL_2                             0x0B7E15 // player_set_unit_index+0x1d4
#define ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2                              0x0B7FB8 // player_set_unit_index+0x379
#define ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2_RETURN                       0x0B7FBD // player_set_unit_index+0x37e
#define ADDRESS_PLAYER_INCREMENT_CONTROL_CONTEXT                         0x0BB9C0 // player_increment_control_context
#define ADDRESS_UNIT_DIED_HOOK                                           0x441B79 // unit_died+0x1f9
#define ADDRESS_UNIT_DIED_HOOK_RETURN                                    0x441B81 // unit_died+0x201
#define ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK                          0x49DCE3 // grenade_throw_move_to_hand+0x18f
#define ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK_RETURN                   0x49DCE9 // grenade_throw_move_to_hand+0x195
#define ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK                       0x444AA8 // unit_add_grenade_to_inventory+0xb8
#define ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK_RETURN                0x444AAF // unit_add_grenade_to_inventory+0xbf
#define ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK                     0x444C56 // unit_add_equipment_to_inventory+0x116
#define ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK_RETURN              0x444C5C // unit_add_equipment_to_inventory+0x11c
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK                                 0x438D78 // unit_update_control+0xaa
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_RETURN                          0x438D7E // unit_update_control+0xb0
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2                               0x438EBB // unit_update_control+0x1ed
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2_RETURN                        0x438EC1 // unit_update_control+0x1f3
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3                               0x4392A1 // unit_update_control+0x5d3
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3_RETURN                        0x4392A7 // unit_update_control+0x5d9
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_END                             0x100914
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT                                 0x1004D0 // unit_add_initial_loadout
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0                           0x1004E0 // unit_add_initial_loadout+0x11
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0_RETURN                    0x1004E5 // unit_add_initial_loadout+0x16
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1                           0x10084F // unit_add_initial_loadout+0x354
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1_RETURN                    0x100855 // unit_add_initial_loadout+0x35a
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2                           0x1008F0 // unit_add_initial_loadout+0x3f9
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2_RETURN                    0x1008F7 // unit_add_initial_loadout+0x400
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP                   0x1008F7 // unit_add_initial_loadout+0x400
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_RETURN            0x1008FD // unit_add_initial_loadout+0x406
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2                 0x100909 // unit_add_initial_loadout+0x412
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2_RETURN          0x100913 // unit_add_initial_loadout+0x41c
#define ADDRESS_PROJECTILE_ATTACH_HOOK                                   0x488BD3 // projectile_attach+0x2c1
#define ADDRESS_PROJECTILE_ATTACH_HOOK_RETURN                            0x488BDB // projectile_attach+0x2c9
#define ADDRESS_BIPED_UPDATE_MELEE_TURNING                               0x4617A0 // biped_update_melee_turning
#define ADDRESS_UNIT_CONTROL_CALL                                        0x02BAF2 // simulation_apply_before_game+0x92
#define ADDRESS_UNIT_CONTROL_CALL_2                                      0x0BF3C7 // player_submit_control+0x1c7
#define ADDRESS_UNIT_CONTROL_CALL_3                                      0x0BF453
#define ADDRESS_UNIT_CONTROL_CALL_4                                      0x187300 // recorded_animations_update+0x120
#define ADDRESS_UNIT_CONTROL_CALL_5                                      0x6CCAF6
#define ADDRESS_UNIT_CONTROL_CALL_6                                      0x6CE9D6
#define ADDRESS_UNIT_SET_AIMING_VECTORS                                  0x44ABB0 // unit_set_aiming_vectors
#define ADDRESS_C_GAME_ENGINE_APPLY_PLAYER_UPDATE_NOP                    0x1D13A5 // c_game_engine::apply_player_update+0x635
#define ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH                      0x0BB043 // player_teleport_on_bsp_switch+0x14b
#define ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH_2                    0x0BB046 // player_teleport_on_bsp_switch+0x14e
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1                            0x059ECD // c_simulation_unit_entity_definition::apply_object_update+0x32f
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1_RETURN                     0x059F16 // c_simulation_unit_entity_definition::apply_object_update+0x378
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2                            0x0611C3 // c_simulation_weapon_fire_event_definition::apply_game_event+0x533
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2_RETURN                     0x0611FF // c_simulation_weapon_fire_event_definition::apply_game_event+0x56f
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3                            0x4C1A3B // c_vehicle_auto_turret::control+0xdb
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3_RETURN                     0x4C1A80 // c_vehicle_auto_turret::control+0x120
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4                            0x0FD2B2 // attach_biped_to_player+0x202
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4_RETURN                     0x0FD2D4 // attach_biped_to_player+0x224
#define ADDRESS_ATTACH_BIPED_TO_PLAYER_NOP                               0x0FD29F // attach_biped_to_player+0x1ef
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK2                                 0x472152 // equipment_activate+0x1112
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK2_RETURN                          0x472158 // equipment_activate+0x1118
#define ADDRESS_UNIT_UPDATE_ENERGY_HOOK                                  0x43BDE0 // unit_update+0x110
#define ADDRESS_UNIT_UPDATE_ENERGY_HOOK_RETURN                           0x43BDE6 // unit_update+0x116
#define ADDRESS_UNIT_SET_HOLOGRAM_HOOK                                   0x44CCF0 // unit_set_hologram+0x17e
#define ADDRESS_UNIT_SET_HOLOGRAM_HOOK_RETURN                            0x44CCFA // unit_set_hologram+0x188
#define ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK                       0x4336BF // object_apply_damage_aftermath+0x341
#define ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK_RETURN                0x4336CD // object_apply_damage_aftermath+0x34f
#define ADDRESS_UNIT_UPDATE_DAMAGE_HOOK                                  0x43B5E9 // unit_update_damage+0xd9
#define ADDRESS_UNIT_UPDATE_DAMAGE_HOOK_RETURN                           0x43B5F0 // unit_update_damage+0xe0
#define ADDRESS_UNIT_RESPOND_TO_EMP_HOOK                                 0x4383CB // unit_respond_to_emp+0xfd
#define ADDRESS_UNIT_RESPOND_TO_EMP_HOOK_RETURN                          0x4383D2 // unit_respond_to_emp+0x104
#define ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK                       0x438505 // unit_delete+0x95
#define ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK_RETURN                0x43850F // unit_delete+0x9f
#define ADDRESS_UNIT_DELETE_EQUIPMENT                                    0x444D30 // unit_delete_equipment
#define ADDRESS_UNIT_PLACE_HOOK                                          0x437A15 // unit_place+0x115
#define ADDRESS_UNIT_PLACE_HOOK_RETURN                                   0x437A9C // unit_place+0x19c
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3                           0x100604 // unit_add_initial_loadout+0x114
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3_RETURN                    0x100678 // unit_add_initial_loadout+0x184
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4                           0x100678 // unit_add_initial_loadout+0x184
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4_RETURN                    0x10067D // unit_add_initial_loadout+0x189
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK 0x05A239 // c_simulation_unit_entity_definition::apply_object_update+0x66a
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN 0x05A2A3 // c_simulation_unit_entity_definition::apply_object_update+0x6d4
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DING                              0x44B260 // unit_active_camouflage_ding
#define ADDRESS_THROW_RELEASE_HOOK2                                      0x49DA4D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_HOOK2_RETURN                               0x49DACF // throw_release+0x40f
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DISABLE                           0x44B200 // unit_active_camouflage_disable
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_LEVEL                         0x44B1A0 // unit_active_camouflage_set_level
#define ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK                      0x4A621F // unit_scripting_set_active_camo+0xf
#define ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK_RETURN               0x4A6274 // unit_scripting_set_active_camo+0x64
#define ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK                          0x0C1C89 // player_update_invisibility+0xb9
#define ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK_RETURN                   0x0C1D01 // player_update_invisibility+0x131
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2 0x05A13A // c_simulation_unit_entity_definition::apply_object_update+0x567
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2_RETURN 0x05A1B5 // c_simulation_unit_entity_definition::apply_object_update+0x5e2
#define ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK 0x0766EA // c_simulation_vehicle_entity_definition::apply_object_update+0x1ed
#define ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN 0x076765 // c_simulation_vehicle_entity_definition::apply_object_update+0x268
#define ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK                               0x6CCC97 // actor_set_active_camo+0x77
#define ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK_RETURN                        0x6CCCE5 // actor_set_active_camo+0xc5
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_MAXIMUM                       0x44B150 // unit_active_camouflage_set_maximum
#define ADDRESS_BIPED_NEW_HOOK                                           0x45D85F // biped_new+0x4d
#define ADDRESS_BIPED_NEW_HOOK_RETURN                                    0x45D88D // biped_new+0x77
#define ADDRESS_VEHICLE_NEW_HOOK                                         0x473116 // vehicle_new+0xd7
#define ADDRESS_VEHICLE_NEW_HOOK_RETURN                                  0x473144 // vehicle_new+0x109
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_END                        0x43B9B7
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE                            0x43B730 // unit_update_active_camouflage
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0                      0x43B74C // unit_update_active_camouflage+0x1c
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0_RETURN               0x43B753 // unit_update_active_camouflage+0x23
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1                      0x43B999 // unit_update_active_camouflage+0x269
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1_RETURN               0x43B9B0 // unit_update_active_camouflage+0x280
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP              0x43B95F // unit_update_active_camouflage+0x22f
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_RETURN       0x43B965 // unit_update_active_camouflage+0x235
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2            0x43B973 // unit_update_active_camouflage+0x243
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2_RETURN     0x43B979 // unit_update_active_camouflage+0x249
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3            0x43B9B0 // unit_update_active_camouflage+0x280
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3_RETURN     0x43B9B6 // unit_update_active_camouflage+0x286
#define ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK                    0x46EB73 // unit_action_assassinate_finished+0x63
#define ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK_RETURN             0x46EB78 // unit_action_assassinate_finished+0x68
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1                     0x46E450 // unit_action_assassinate_submit+0x21f
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1_RETURN              0x46E457 // unit_action_assassinate_submit+0x226
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2                     0x46E4A9 // unit_action_assassinate_submit+0x282
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2_RETURN              0x46E4B0 // unit_action_assassinate_submit+0x289
#define ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK                 0x46EC1D // unit_action_assassinate_interrupted+0x7d
#define ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK_RETURN          0x46EC22 // unit_action_assassinate_interrupted+0x82
#define ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK                      0x47736C // motor_task_enter_seat_internal+0x4be
#define ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK_RETURN               0x477373 // motor_task_enter_seat_internal+0x4c5
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK        0x477D53 // motor_animation_exit_seat_immediate_internal+0x323
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK_RETURN 0x477D58 // motor_animation_exit_seat_immediate_internal+0x328
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1                      0x47E6F9 // device_group_set_actual_value+0xc9
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1_RETURN               0x47E72E // device_group_set_actual_value+0xfe
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2                      0x47E753 // device_group_set_actual_value+0x123
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2_RETURN               0x47E78C // device_group_set_actual_value+0x15c
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1                     0x47E3DF // device_group_set_desired_value+0x111
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1_RETURN              0x47E3F2 // device_group_set_desired_value+0x124
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2                     0x47E443 // device_group_set_desired_value+0x175
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2_RETURN              0x47E449 // device_group_set_desired_value+0x17b
#define ADDRESS_DEVICE_SET_POWER_HOOK                                    0x47E2BE // device_set_power+0x3e
#define ADDRESS_DEVICE_SET_POWER_HOOK_RETURN                             0x47E2C3 // device_set_power+0x43
#define ADDRESS_MACHINE_UPDATE_HOOK                                      0x4ADD36 // machine_update+0x268
#define ADDRESS_MACHINE_UPDATE_HOOK_RETURN                               0x4ADD3D // machine_update+0x26f
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_END              0x4C1954
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET                  0x4C1610 // c_vehicle_auto_turret::track_auto_target
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0            0x4C1626 // c_vehicle_auto_turret::track_auto_target+0x16
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0_RETURN     0x4C162C // c_vehicle_auto_turret::track_auto_target+0x1c
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1            0x4C18AE // c_vehicle_auto_turret::track_auto_target+0x29e
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1_RETURN     0x4C18B4 // c_vehicle_auto_turret::track_auto_target+0x2a4
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2            0x4C192F // c_vehicle_auto_turret::track_auto_target+0x31f
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_RETURN     0x4C1936 // c_vehicle_auto_turret::track_auto_target+0x326
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2          0x4C1944 // c_vehicle_auto_turret::track_auto_target+0x334
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2_RETURN   0x4C194B // c_vehicle_auto_turret::track_auto_target+0x33b
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP    0x4C1936 // c_vehicle_auto_turret::track_auto_target+0x326
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_RETURN 0x4C193C // c_vehicle_auto_turret::track_auto_target+0x32c
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2  0x4C194B // c_vehicle_auto_turret::track_auto_target+0x33b
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2_RETURN 0x4C1951 // c_vehicle_auto_turret::track_auto_target+0x341
#define ADDRESS_OBJECT_MOVE_POSITION_HOOK                                0x41C9B6 // object_move_position+0xa6
#define ADDRESS_OBJECT_MOVE_POSITION_HOOK_RETURN                         0x41C9BC // object_move_position+0xac
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK                           0x0CB5DE // game_engine_update_player+0xae
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK_RETURN                    0x0CB601 // game_engine_update_player+0xd1
#define ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK                         0x1CFEC0 // c_game_engine::player_update+0xe1
#define ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK_RETURN                  0x1CFEC6 // c_game_engine::player_update+0xe7
#define ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK                      0x11BED2 // game_engine_teleporters_update+0x22
#define ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK_RETURN               0x11BED9 // game_engine_teleporters_update+0x29
#define ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK                  0x0C8A07 // game_engine_initialize_for_new_map+0x33e
#define ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK_RETURN           0x0C8A20 // game_engine_initialize_for_new_map+0x357
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1              0x2306B6 // c_ctf_engine::initialize_for_new_round+0x66
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1_RETURN       0x2306CF // c_ctf_engine::initialize_for_new_round+0x7f
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2              0x230719 // c_ctf_engine::initialize_for_new_round+0xc9
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2_RETURN       0x230732 // c_ctf_engine::initialize_for_new_round+0xe2
#define ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK                      0x233CE3 // ?initialize@?$c_area_set@Vc_area@@$02@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK_RETURN               0x233CFC // ?initialize@?$c_area_set@Vc_area@@$02@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK                     0x23A012 // ?initialize@?$c_area_set@Vc_area@@$09@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x42
#define ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK_RETURN              0x23A02B // ?initialize@?$c_area_set@Vc_area@@$09@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5b
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1                   0x23A108 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1_RETURN            0x23A121 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2                   0x23A1BD // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2_RETURN            0x23A1D6 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0xf6
#define ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK                     0x2366F4 // ?initialize@?$c_area_set@Vc_area@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x44
#define ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK_RETURN              0x23670D // ?initialize@?$c_area_set@Vc_area@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5d
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1                   0x2367F8 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1_RETURN            0x236811 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2                   0x2368AD // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2_RETURN            0x2368C6 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xf6
#define ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK            0x240D23 // ?initialize@?$c_area_set@Us_territory_data@@$07@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK_RETURN     0x240D3C // ?initialize@?$c_area_set@Us_territory_data@@$07@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK         0x23C533 // ?initialize@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK_RETURN  0x23C54C // ?initialize@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1       0x23C638 // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1_RETURN 0x23C651 // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2       0x23C6ED // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2_RETURN 0x23C70A // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xfa
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK             0x0FF8CB // game_engine_multiplayer_weapon_register+0x6b
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK_RETURN      0x0FF8D2 // game_engine_multiplayer_weapon_register+0x72
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK           0x0FFA04 // game_engine_multiplayer_weapon_deregister+0x9b
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK_RETURN    0x0FFA0B // game_engine_multiplayer_weapon_deregister+0xa2
#define ADDRESS_CREATE_FLAG_HOOK                                         0x232BA7 // create_flag+0x158
#define ADDRESS_CREATE_FLAG_HOOK_RETURN                                  0x232BB6 // create_flag+0x168
#define ADDRESS_FLAG_RESET_PATCH                                         0x232DE4 // flag_reset+0x3b
#define ADDRESS_FLAG_RESET_PATCH_2                                       0x232E19 // flag_reset+0x73
#define ADDRESS_FLAG_RESET_HOOK                                          0x232E96 // flag_reset+0x174
#define ADDRESS_FLAG_RESET_HOOK_RETURN                                   0x232E9B // flag_reset+0x179

// anvil\hooks\simulation\hooks_physics_updates.cpp
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1                       0x41C778 // object_set_position_internal+0xa8
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1_RETURN                0x41C77E // object_set_position_internal+0xae
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2                       0x41C7A0 // object_set_position_internal+0xd0
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2_RETURN                0x41C7A6 // object_set_position_internal+0xd6
#define ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK                      0x425461 // object_move_respond_to_physics+0x121
#define ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK_RETURN               0x4254ED // object_move_respond_to_physics+0x1ad
#define ADDRESS_OBJECT_SET_VELOCITIES_INTERNAL                           0x41CC40 // object_set_velocities_internal
#define ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK                           0x41CF28 // object_apply_acceleration+0x98
#define ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK_RETURN                    0x41CF63 // object_apply_acceleration+0xd3
#define ADDRESS_OBJECT_SET_AT_REST                                       0x421C30 // object_set_at_rest
#define ADDRESS_OBJECT_SET_AT_REST_CALL                                  0x06F999 // c_simulation_object_entity_definition::object_apply_update+0x3b8
#define ADDRESS_OBJECT_SET_AT_REST_CALL_2                                0x07B316 // c_simulation_generic_entity_definition::apply_object_update+0x276
#define ADDRESS_OBJECT_SET_AT_REST_CALL_3                                0x0CA231 // garbage_collect_multiplayer+0x381
#define ADDRESS_OBJECT_SET_AT_REST_CALL_4                                0x1786E3 // c_candy_spawner::spawn_object+0x323
#define ADDRESS_OBJECT_SET_AT_REST_CALL_5                                0x1321B8 // c_havok_component::wake_all_bodies_in_phantoms+0x98
#define ADDRESS_OBJECT_SET_AT_REST_CALL_6                                0x41C675 // object_reset+0x85
#define ADDRESS_OBJECT_SET_AT_REST_CALL_7                                0x439B87 // unit_fix_position+0x307
#define ADDRESS_OBJECT_SET_AT_REST_CALL_8                                0x434673 // damage_section_response_fire+0x103
#define ADDRESS_OBJECT_SET_AT_REST_CALL_9                                0x436625 // object_damage_constraints+0xc3
#define ADDRESS_OBJECT_SET_AT_REST_CALL_10                               0x45E524 // biped_update_without_parent+0x256
#define ADDRESS_OBJECT_SET_AT_REST_CALL_11                               0x483F85 // projectile_accelerate+0x3b7
#define ADDRESS_OBJECT_SET_AT_REST_CALL_12                               0x477BB5 // motor_animation_exit_seat_immediate_internal+0x185
#define ADDRESS_OBJECT_SET_AT_REST_CALL_13                               0x1FA28D // object_wake_physics_evaluate+0x4d
#define ADDRESS_OBJECT_SET_AT_REST_CALL_14                               0x48DCBE // object_early_mover_delete+0x27e
#define ADDRESS_OBJECT_SET_AT_REST_CALL_15                               0x4A52B8 // item_multiplayer_at_rest_state_initialize+0x148
#define ADDRESS_OBJECT_SET_AT_REST_CALL_16                               0x4B8B41 // biped_stun_submit+0x1a3
#define ADDRESS_OBJECT_SET_AT_REST_CALL_17                               0x4C9D14 // c_vehicle_type_mantis::update_physics+0xf6
#define ADDRESS_OBJECT_SET_AT_REST_CALL_18                               0x4C4E00 // biped_dead_force_airborne+0xe0
#define ADDRESS_OBJECT_SET_AT_REST_CALL_19                               0x4C2970 // biped_exit_relaxation+0x200
#define ADDRESS_OBJECT_SET_AT_REST_CALL_20                               0x4C2E00 // biped_start_relaxation+0x136
#define ADDRESS_OBJECT_SET_AT_REST_HOOK2                                 0x07B032 // c_simulation_generic_entity_definition::create_object+0xd5
#define ADDRESS_OBJECT_SET_AT_REST_HOOK2_RETURN                          0x07B07F // c_simulation_generic_entity_definition::create_object+0x122
#define ADDRESS_OBJECT_SET_AT_REST_HOOK3                                 0x0763B5 // c_simulation_vehicle_entity_definition::create_object+0xab
#define ADDRESS_OBJECT_SET_AT_REST_HOOK3_RETURN                          0x076402 // c_simulation_vehicle_entity_definition::create_object+0xf8
#define ADDRESS_OBJECT_SET_AT_REST_HOOK4                                 0x4214C7 // object_attach_to_node_immediate+0x3e7
#define ADDRESS_OBJECT_SET_AT_REST_HOOK4_RETURN                          0x421501 // object_attach_to_node_immediate+0x421
#define ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP                      0x4214B9 // object_attach_to_node_immediate+0x3d9
#define ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP_2                    0x4214C1 // object_attach_to_node_immediate+0x3e1
#define ADDRESS_OBJECT_SET_AT_REST_HOOK5                                 0x488A73 // projectile_attach+0x161
#define ADDRESS_OBJECT_SET_AT_REST_HOOK5_RETURN                          0x488AA9 // projectile_attach+0x197
#define ADDRESS_PROJECTILE_ATTACH_NOP                                    0x488A55 // projectile_attach+0x143
#define ADDRESS_PROJECTILE_ATTACH_NOP_2                                  0x488A66 // projectile_attach+0x154
#define ADDRESS_OBJECT_SET_AT_REST_HOOK6                                 0x485B50 // projectile_collision+0x1552
#define ADDRESS_OBJECT_SET_AT_REST_HOOK6_RETURN                          0x485B95 // projectile_collision+0x1597
#define ADDRESS_OBJECT_SET_AT_REST_HOOK7                                 0x483076 // projectile_initial_update+0x426
#define ADDRESS_OBJECT_SET_AT_REST_HOOK7_RETURN                          0x483101 // projectile_initial_update+0x4b1
#define ADDRESS_PROJECTILE_INITIAL_UPDATE_NOP                            0x483068 // projectile_initial_update+0x418
#define ADDRESS_OBJECT_SET_AT_REST_HOOK8                                 0x48DB80 // object_early_mover_delete+0x140
#define ADDRESS_OBJECT_SET_AT_REST_HOOK8_RETURN                          0x48DC01 // object_early_mover_delete+0x1c1
#define ADDRESS_OBJECT_SET_AT_REST_HOOK9                                 0x48DC50 // object_early_mover_delete+0x210
#define ADDRESS_OBJECT_SET_AT_REST_HOOK9_RETURN                          0x48DCC3 // object_early_mover_delete+0x283
#define ADDRESS_OBJECT_SET_AT_REST_HOOK10                                0x705F77 // swarm_accelerate+0xd7
#define ADDRESS_OBJECT_SET_AT_REST_HOOK10_RETURN                         0x705F9F // swarm_accelerate+0xff
#define ADDRESS_SWARM_ACCELERATE_NOP                                     0x705F1C // swarm_accelerate+0x7c
#define ADDRESS_OBJECT_SET_AT_REST_CALL_21                               0x4B05B1 // scenery_new+0xaa
#define ADDRESS_OBJECT_SET_AT_REST_HOOK12                                0x4DF6B2 // vehicle_program_activate+0x54
#define ADDRESS_OBJECT_SET_AT_REST_HOOK12_RETURN                         0x4DF6E5 // vehicle_program_activate+0x87
#define ADDRESS_OBJECT_SET_AT_REST_HOOK13                                0x4E0ACF // vehicle_program_update+0x141
#define ADDRESS_OBJECT_SET_AT_REST_HOOK13_RETURN                         0x4E0B14 // vehicle_program_update+0x186
#define ADDRESS_OBJECT_SET_AT_REST_CALL_22                               0x4E85B6 // unit_custom_animation_play_animation_submit+0x2c6

// anvil\hooks\simulation\hooks_player_updates.cpp
#define ADDRESS_PLAYER_SPAWN_HOOK1                                       0x0BCFCB // player_spawn+0x3b3
#define ADDRESS_PLAYER_SPAWN_HOOK1_RETURN                                0x0BCFD0 // player_spawn+0x3b8
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2                          0x0B9FEA // players_update_after_game+0x37a
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2_RETURN                   0x0B9FF8 // players_update_after_game+0x388
#define ADDRESS_PLAYER_SPAWN_HOOK2                                       0x0BD363 // player_spawn+0x755
#define ADDRESS_PLAYER_SPAWN_HOOK2_RETURN                                0x0BD369 // player_spawn+0x75b
#define ADDRESS_GAME_ENGINE_PLAYER_SET_SPAWN_TIMER                       0x0C9690 // game_engine_player_set_spawn_timer
#define ADDRESS_PLAYER_SPAWN_HOOK3                                       0x0BD387 // player_spawn+0x779
#define ADDRESS_PLAYER_SPAWN_HOOK3_RETURN                                0x0BD38E // player_spawn+0x780
#define ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK                0x0FEE48 // game_engine_setup_player_for_respawn+0x228
#define ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK_RETURN         0x0FEE4F // game_engine_setup_player_for_respawn+0x22f
#define ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK           0x100E51 // objective_game_player_forced_base_respawn+0xc1
#define ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK_RETURN    0x100E58 // objective_game_player_forced_base_respawn+0xc8
#define ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK  0x1D1854 // player_killed_player_perform_respawn_on_kill_check+0x104
#define ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK_RETURN 0x1D1860 // player_killed_player_perform_respawn_on_kill_check+0x110
#define ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK             0x10099E // game_engine_reset_player_respawn_timers+0x7e
#define ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK_RETURN      0x1009A6 // game_engine_reset_player_respawn_timers+0x86
#define ADDRESS_C_SIMULATION_PLAYER_RESPAWN_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT 0x064FE0 // c_simulation_player_respawn_request_event_definition::apply_game_event
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1                              0x0BCD1D // player_spawn+0x242
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1_RETURN                       0x0BCD24 // player_spawn+0x249
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2                              0x0E3ED6 // equipment_add+0x78
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2_RETURN                       0x0E3EDD // equipment_add+0x7f
#define ADDRESS_PLAYER_UPDATE_LOADOUT                                    0x0A93D0 // player_update_loadout
#define ADDRESS_PLAYER_RESET_HOOK                                        0x0B6F09 // player_reset+0x639
#define ADDRESS_PLAYER_RESET_HOOK_RETURN                                 0x0B6FDD // player_reset+0x70d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK            0x0CBACD // game_engine_update+0x17d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK_RETURN     0x0CBAD3 // game_engine_update+0x183
#define ADDRESS_SIMULATION_QUEUE_PLAYER_EVENT_APPLY_SET_ACTIVATION       0x054990 // simulation_queue_player_event_apply_set_activation
#define ADDRESS_GAME_ENGINE_BOOT_PLAYER_SAFE                             0x054A00 // game_engine_boot_player_safe
#define ADDRESS_GAME_ENGINE_BOOT_PLAYER                                  0x0CEBA0 // game_engine_boot_player
#define ADDRESS_PLAYER_NOTIFY_VEHICLE_EJECTION_FINISHED                  0x0C1A50 // player_notify_vehicle_ejection_finished
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3                          0x0BA02E // players_update_after_game+0x3be
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3_RETURN                   0x0BA042 // players_update_after_game+0x3d2
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1                          0x0B9FD6 // players_update_after_game+0x366
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1_RETURN                   0x0B9FDD // players_update_after_game+0x36d
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1                          0x0FDAFB // game_engine_player_killed+0xdb
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1_RETURN                   0x0FDB02 // game_engine_player_killed+0xe2
#define ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1                0x1B60B8 // c_game_statborg::record_player_death+0xe8
#define ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1_RETURN         0x1B60BE // c_game_statborg::record_player_death+0xee
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2                          0x0CB6D5 // game_engine_update_player+0x1a5
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2_RETURN                   0x0CB6DF // game_engine_update_player+0x1af
#define ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK                     0x0FEE79 // game_engine_player_fired_weapon+0x9
#define ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK_RETURN              0x0FEF74 // game_engine_player_fired_weapon+0x104
#define ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK                   0x0FD90D // game_engine_player_damaged_player+0xe0
#define ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK_RETURN            0x0FD9DE // game_engine_player_damaged_player+0x1cb
#define ADDRESS_UPDATE_PLAYER_NAVPOINT_DATA                              0x0CE9C0 // update_player_navpoint_data
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3         0x0CBC31 // game_engine_update_after_game_update_state+0x51
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3_RETURN  0x0CBC6F // game_engine_update_after_game_update_state+0x8f
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_PATCH         0x0CBC6F // game_engine_update_after_game_update_state+0x8f
#define ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK                         0x0FF44B // game_engine_player_rejoined+0x83
#define ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK_RETURN                  0x0FF452 // game_engine_player_rejoined+0x8a
#define ADDRESS_GAME_ENGINE_APPLY_APPEARANCE_TRAITS                      0x122F70 // game_engine_apply_appearance_traits
#define ADDRESS_GAME_ENGINE_APPLY_MOVEMENT_TRAITS                        0x122E40 // game_engine_apply_movement_traits
#define ADDRESS_GAME_ENGINE_APPLY_SENSORS_TRAITS                         0x123090 // game_engine_apply_sensors_traits
#define ADDRESS_GAME_ENGINE_APPLY_SHIELD_VITALITY_TRAITS                 0x122C70 // game_engine_apply_shield_vitality_traits
#define ADDRESS_GAME_ENGINE_APPLY_WEAPONS_TRAITS                         0x122D20 // game_engine_apply_weapons_traits
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4         0x0CBD41 // game_engine_update_after_game_update_state+0x161
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4_RETURN  0x0CBD48 // game_engine_update_after_game_update_state+0x168
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2                          0x0FDCB4 // game_engine_player_killed+0x28f
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2_RETURN                   0x0FDCDC // game_engine_player_killed+0x2b7
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK               0x0CD1CD // game_engine_update_player_sitting_out+0x4d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK_RETURN        0x0CD1D5 // game_engine_update_player_sitting_out+0x55
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1                 0x0B71F4 // player_swap+0x204
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1_RETURN          0x0B71FE // player_swap+0x20e
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2                 0x0B723C // player_swap+0x24c
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2_RETURN          0x0B7247 // player_swap+0x257
#define ADDRESS_PLAYER_DELETE_HOOK                                       0x0B7397 // player_delete+0xc7
#define ADDRESS_PLAYER_DELETE_HOOK_RETURN                                0x0B739C // player_delete+0xcc
#define ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK                             0x0FF391 // game_engine_player_left+0x101
#define ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK_RETURN                      0x0FF396 // game_engine_player_left+0x106
#define ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK                          0x11D4DD // teleporter_teleport_object+0x18d
#define ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK_RETURN                   0x11D4E2 // teleporter_teleport_object+0x192

// anvil\hooks\simulation\hooks_simulation.cpp
#define ADDRESS_UPDATE_ESTABLISHING_VIEW                                 0x037020 // c_simulation_world::update_establishing_view
#define ADDRESS_INTERNAL_HALT_RENDER_THREAD_AND_LOCK_RESOURCES_CALL      0x0C8FEE // game_engine_game_starting+0x5e
#define ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY            0x0C92E0 // game_engine_game_ending+0x50
#define ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY_RETURN     0x0C9313 // game_engine_game_ending+0x83

// anvil\hooks\simulation\hooks_simulation_events.cpp
#define ADDRESS_DAMAGE_SECTION_DEPLETE_HOOK                              0x435765 // damage_section_deplete+0x245
#define ADDRESS_DAMAGE_SECTION_DEPLETE_HOOK_RETURN                       0x43576A // damage_section_deplete+0x24a
#define ADDRESS_DAMAGE_SECTION_RESPOND_TO_DAMAGE_HOOK                    0x435436 // damage_section_respond_to_damage+0x466
#define ADDRESS_DAMAGE_SECTION_RESPOND_TO_DAMAGE_HOOK_RETURN             0x43543E // damage_section_respond_to_damage+0x46e
#define ADDRESS_OBJECT_DAMAGE_NEW_HOOK                                   0x42D254 // object_damage_new+0x224
#define ADDRESS_OBJECT_DAMAGE_NEW_HOOK_RETURN                            0x42D259 // object_damage_new+0x229
#define ADDRESS_OBJECT_CAUSE_DAMAGE_HOOK                                 0x43037E // object_cause_damage+0x107e
#define ADDRESS_OBJECT_CAUSE_DAMAGE_HOOK_RETURN                          0x430383 // object_cause_damage+0x1083
#define ADDRESS_PROJECTILE_ATTACH_HOOK2                                  0x488D07 // projectile_attach+0x3f5
#define ADDRESS_PROJECTILE_ATTACH_HOOK2_RETURN                           0x488D0D // projectile_attach+0x3fb
#define ADDRESS_PROJECTILE_DETONATE_EFFECTS_AND_DAMAGE                   0x4874B0 // projectile_detonate_effects_and_damage
#define ADDRESS_PROJECTILE_DETONATE_PATCH                                0x487F30 // projectile_detonate+0x8f0
#define ADDRESS_PROJECTILE_COLLISION_END                                 0x486337
#define ADDRESS_PROJECTILE_COLLISION                                     0x484600 // projectile_collision
#define ADDRESS_PROJECTILE_COLLISION_HOOK0                               0x485CA2 // projectile_collision+0x16a4
#define ADDRESS_PROJECTILE_COLLISION_HOOK0_RETURN                        0x485CAA // projectile_collision+0x16ac
#define ADDRESS_PROJECTILE_COLLISION_HOOK1                               0x485F59 // projectile_collision+0x195b
#define ADDRESS_PROJECTILE_COLLISION_HOOK1_RETURN                        0x485F60 // projectile_collision+0x1961
#define ADDRESS_PROJECTILE_COLLISION_HOOK2                               0x485FDA // projectile_collision+0x19db
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_RETURN                        0x485FE2 // projectile_collision+0x19e3
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_2                             0x4860A2 // projectile_collision+0x1aa3
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_2_RETURN                      0x4860A7 // projectile_collision+0x1aa8
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_3                             0x486124 // projectile_collision+0x1b24
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_3_RETURN                      0x48612D // projectile_collision+0x1b2d
#define ADDRESS_PROJECTILE_COLLISION_HOOK3                               0x486142 // projectile_collision+0x1b42
#define ADDRESS_PROJECTILE_COLLISION_HOOK3_RETURN                        0x48614A // projectile_collision+0x1b4a
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP                       0x486261 // projectile_collision+0x1c61
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_RETURN                0x486266 // projectile_collision+0x1c66
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_2                     0x486293 // projectile_collision+0x1c93
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_2_RETURN              0x486298 // projectile_collision+0x1c98
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_3                     0x4862C1 // projectile_collision+0x1cc1
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_3_RETURN              0x4862C6 // projectile_collision+0x1cc6
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_4                     0x486327 // projectile_collision+0x1d27
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_4_RETURN              0x48632C // projectile_collision+0x1d2c
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_5                     0x48632F // projectile_collision+0x1d2f
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_5_RETURN              0x486336 // projectile_collision+0x1d36
#define ADDRESS_UNIT_ACTION_VEHICLE_BOARD_SUBMIT_HOOK                    0x4685D1 // unit_action_vehicle_board_submit+0x133
#define ADDRESS_UNIT_ACTION_VEHICLE_BOARD_SUBMIT_HOOK_RETURN             0x4685D8 // unit_action_vehicle_board_submit+0x13a
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_INTERNAL_HOOK                  0x4779C6 // motor_animation_exit_seat_internal+0x156
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_INTERNAL_HOOK_RETURN           0x4779CF // motor_animation_exit_seat_internal+0x15f
#define ADDRESS_UNIT_RESOLVE_MELEE_ATTACK_HOOK                           0x44C9F0 // unit_resolve_melee_attack+0x360
#define ADDRESS_UNIT_RESOLVE_MELEE_ATTACK_HOOK_RETURN                    0x44C9F6 // unit_resolve_melee_attack+0x366
#define ADDRESS_GAME_ENGINE_SEND_EVENT                                   0x120F20 // game_engine_send_event
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK2                          0x0FFD79 // game_engine_earn_wp_event+0x119
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK2_RETURN                   0x0FFDB3 // game_engine_earn_wp_event+0x153
#define ADDRESS_GAME_ENGINE_SCORING_UPDATE_LEADERS_INTERNAL_HOOK         0x0E3A0F // game_engine_scoring_update_leaders_internal+0x2ff
#define ADDRESS_GAME_ENGINE_SCORING_UPDATE_LEADERS_INTERNAL_HOOK_RETURN  0x0E3A4A // game_engine_scoring_update_leaders_internal+0x33a
#define ADDRESS_GAME_ENGINE_AWARD_MEDAL_HOOK                             0x0FFFC8 // award_medal+0x178
#define ADDRESS_GAME_ENGINE_AWARD_MEDAL_HOOK_RETURN                      0x0FFFF9 // award_medal+0x1a9
#define ADDRESS_C_SLAYER_ENGINE_EMIT_GAME_START_EVENT_HOOK               0x238462 // c_slayer_engine::emit_game_start_event+0x62
#define ADDRESS_C_SLAYER_ENGINE_EMIT_GAME_START_EVENT_HOOK_RETURN        0x238498 // c_slayer_engine::emit_game_start_event+0x98
#define ADDRESS_DISPLAY_TELEPORTER_BLOCKED_MESSAGE_HOOK                  0x11D6DF // display_teleporter_blocked_message+0x4f
#define ADDRESS_DISPLAY_TELEPORTER_BLOCKED_MESSAGE_HOOK_RETURN           0x11D714 // display_teleporter_blocked_message+0x84
#define ADDRESS_C_TELEPORTER_AREA_UPDATE_PLAYERS_HOOK                    0x11D64F
#define ADDRESS_C_TELEPORTER_AREA_UPDATE_PLAYERS_HOOK_RETURN             0x11D684

// anvil\hooks\simulation\hooks_simulation_globals.cpp
#define ADDRESS_GAME_ENGINE_UPDATE_ROUND_CONDITIONS                      0x0C8BC0 // game_engine_update_round_conditions
#define ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK                             0x0CB8BB // game_engine_update_time+0x14b
#define ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK_RETURN                      0x0CB8C1 // game_engine_update_time+0x151
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2                      0x0CC255 // game_engine_update_after_game+0xe5
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2_RETURN               0x0CC25C // game_engine_update_after_game+0xec
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1         0x0CBC1A // game_engine_update_after_game_update_state+0x3a
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1_RETURN  0x0CBC21 // game_engine_update_after_game_update_state+0x41
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2         0x0CBD8B // game_engine_update_after_game_update_state+0x1ab
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2_RETURN  0x0CBD92 // game_engine_update_after_game_update_state+0x1b2
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1                    0x0E0302 // game_engine_build_initial_teams+0xf2
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1_RETURN             0x0E0309 // game_engine_build_initial_teams+0xf9
#define ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK                0x0DFD4D // game_engine_build_valid_team_mapping+0x10d
#define ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK_RETURN         0x0DFD52 // game_engine_build_valid_team_mapping+0x112
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK                  0x0DFE91 // game_engine_recompute_active_teams+0x81
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK_RETURN           0x0DFE96 // game_engine_recompute_active_teams+0x86
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2                    0x0E0329 // game_engine_build_initial_teams+0x119
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2_RETURN             0x0E032F // game_engine_build_initial_teams+0x11f
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK               0x0E0077 // game_engine_teams_use_one_shared_life+0x57
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK_RETURN        0x0E007C // game_engine_teams_use_one_shared_life+0x5c
#define ADDRESS_C_CTF_ENGINE_GAME_STARTING                               0x23090F // c_ctf_engine::game_starting+0x2f
#define ADDRESS_C_CTF_ENGINE_GAME_STARTING_RETURN                        0x230915 // c_ctf_engine::game_starting+0x35
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1                0x231F3D // c_ctf_engine::get_time_left_in_ticks+0x283
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1_RETURN         0x231F44 // c_ctf_engine::get_time_left_in_ticks+0x28a
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2                0x231F64 // c_ctf_engine::get_time_left_in_ticks+0x2aa
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2_RETURN         0x231F6B // c_ctf_engine::get_time_left_in_ticks+0x2b1
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3                0x231F95 // c_ctf_engine::get_time_left_in_ticks+0x2db
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3_RETURN         0x231F9C // c_ctf_engine::get_time_left_in_ticks+0x2e2
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4                0x231FB5 // c_ctf_engine::get_time_left_in_ticks+0x2fb
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4_RETURN         0x231FBC // c_ctf_engine::get_time_left_in_ticks+0x302
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5                0x231EEC // c_ctf_engine::get_time_left_in_ticks+0x212
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5_RETURN         0x231EF4 // c_ctf_engine::get_time_left_in_ticks+0x21a
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6                0x232015 // c_ctf_engine::get_time_left_in_ticks+0x35b
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6_RETURN         0x23201C // c_ctf_engine::get_time_left_in_ticks+0x362
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK               0x230759 // c_ctf_engine::initialize_for_new_round+0x109
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK_RETURN        0x230763 // c_ctf_engine::initialize_for_new_round+0x113
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_OBJECT_DATA                      0x230600 // c_ctf_engine::initialize_object_data

// anvil\hooks\simulation\hooks_statborg.cpp
#define ADDRESS_GAME_ENGINE_PLAYER_ADDED                                 0x0FF090 // game_engine_player_added
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK                       0x0CC2E0 // game_engine_update_after_game+0x170
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK_RETURN                0x0CC2E7 // game_engine_update_after_game+0x177
#define ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK                  0x1B54BD // c_game_statborg::adjust_player_stat+0x4d
#define ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK_RETURN           0x1B54C4 // c_game_statborg::adjust_player_stat+0x54
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1                  0x0CAA83 // game_engine_end_round_with_winner+0x143
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1_RETURN           0x0CAA8A // game_engine_end_round_with_winner+0x14a
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2                  0x0CABCD // game_engine_end_round_with_winner+0x28d
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2_RETURN           0x0CABD4 // game_engine_end_round_with_winner+0x294
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK                           0x0FFDF0 // game_engine_earn_wp_event+0x190
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK_RETURN                    0x0FFDF7 // game_engine_earn_wp_event+0x197
#define ADDRESS_ADJUST_TEAM_STAT                                         0x1B55B0 // c_game_statborg::adjust_team_stat
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3                  0x0CA9EF // game_engine_end_round_with_winner+0xaf
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3_RETURN           0x0CA9F6 // game_engine_end_round_with_winner+0xb6
#define ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK                  0x1CF7F4 // c_game_engine::recompute_team_score+0x104
#define ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK_RETURN           0x1CF802 // c_game_engine::recompute_team_score+0x112
#define ADDRESS_PLAYER_CHANGED_TEAMS_HOOK                                0x0FF736 // game_engine_player_changed_teams+0x56
#define ADDRESS_PLAYER_CHANGED_TEAMS_HOOK_RETURN                         0x0FF73F // game_engine_player_changed_teams+0x5f
#define ADDRESS_GAME_ENGINE_PLAYER_INDICES_SWAPPED                       0x0FF520 // game_engine_player_indices_swapped
#define ADDRESS_STATS_RESET_FOR_ROUND_SWITCH                             0x1B4CA0 // c_game_statborg::stats_reset_for_round_switch

// anvil\hooks\simulation\hooks_weapon_updates.cpp
#define ADDRESS_WEAPON_AGE_HOOK                                          0x454702 // weapon_age+0xc6
#define ADDRESS_WEAPON_AGE_HOOK_RETURN                                   0x45470C // weapon_age+0xd0
#define ADDRESS_WEAPON_BARREL_FIRE_HOOK                                  0x456156 // weapon_barrel_fire+0x35a
#define ADDRESS_WEAPON_BARREL_FIRE_HOOK_RETURN                           0x45615B // weapon_barrel_fire+0x35f
#define ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK                      0x4558CC // weapon_magazine_execute_reload+0x10e
#define ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK_RETURN               0x4558D4 // weapon_magazine_execute_reload+0x116
#define ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK                              0x44E493 // weapon_magazine_update+0xd4
#define ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK_RETURN                       0x44E498 // weapon_magazine_update+0xd9
#define ADDRESS_WEAPON_REPORT_KILL_HOOK                                  0x4543ED // weapon_report_kill+0xf5
#define ADDRESS_WEAPON_REPORT_KILL_HOOK_RETURN                           0x4543F2 // weapon_report_kill+0xfa
#define ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK                           0x454179 // weapon_set_current_amount+0x10b
#define ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK_RETURN                    0x45417E // weapon_set_current_amount+0x110
#define ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK                             0x453EAC // weapon_set_total_rounds+0xb9
#define ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK_RETURN                      0x453EB2 // weapon_set_total_rounds+0xbf
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1                       0x452B23 // weapon_take_inventory_rounds+0xa7
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1_RETURN                0x452B2A // weapon_take_inventory_rounds+0xae
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2                       0x452BBF // weapon_take_inventory_rounds+0x141
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2_RETURN                0x452BC6 // weapon_take_inventory_rounds+0x148
#define ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK                               0x44E960 // weapon_trigger_update+0x205
#define ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK_RETURN                        0x44E965 // weapon_trigger_update+0x20a
#define ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK              0x451AB5 // weapon_handle_potential_inventory_item+0x3bc
#define ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK_RETURN       0x451ABB // weapon_handle_potential_inventory_item+0x3c2
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_END                      0x447566
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX                          0x4474A0 // unit_inventory_set_weapon_index
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0                    0x4474A6 // unit_inventory_set_weapon_index+0x6
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0_RETURN             0x4474AC // unit_inventory_set_weapon_index+0xc
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1                    0x44751E // unit_inventory_set_weapon_index+0x7e
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1_RETURN             0x44755F // unit_inventory_set_weapon_index+0xbf
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP            0x44755F // unit_inventory_set_weapon_index+0xbf
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP_RETURN     0x447565 // unit_inventory_set_weapon_index+0xc5
#define ADDRESS_UNIT_INVENTORY_CYCLE_WEAPON_SET_IDENTIFIER               0x447450 // unit_inventory_cycle_weapon_set_identifier
#define ADDRESS_UNIT_DELETE_ALL_WEAPONS_INTERNAL                         0x4455E0 // unit_delete_all_weapons_internal
#define ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK                          0x4478A9 // unit_handle_deleted_object+0xa9
#define ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK_RETURN                   0x447908 // unit_handle_deleted_object+0x108

// anvil\server_tools.cpp
#define ADDRESS_TUTORIAL_START_PATCH                                     0x35046D // tutorial_start+0x4d
#define ADDRESS_TUTORIAL_START_PATCH_2                                   0x3504B8 // tutorial_start+0x98
#define ADDRESS_LEVELS_GET_TUTORIAL_MAP_ID_PATCH                         0x0E0A86 // levels_get_tutorial_map_id+0xa6

// cache\cache_file_tag_resource_runtime.cpp
#define ADDRESS_TAG_RESOURCES_LOCK_GAME                                  0x07EE80 // tag_resources_lock_game
#define ADDRESS_TAG_RESOURCES_UNLOCK_GAME                                0x07EED0 // tag_resources_unlock_game

// cache\cache_files.cpp
#define ADDRESS_G_CACHE_FILE_GLOBALS                                     0x2B47FF8 // g_cache_file_globals
#define ADDRESS_CACHE_FILE_GET_GLOBAL_TAG_INDEX                          0x083A40 // cache_file_get_global_tag_index

// cache\cache_files_windows.cpp
#define ADDRESS_CACHE_FILE_TABLE_OF_CONTENTS                             0x2D7E978 // cache_file_table_of_contents
#define ADDRESS_CACHED_MAP_FILE_OPEN_FOR_RUNNING_OFF_DVD                 0x0EFE20 // cached_map_file_open_for_running_off_dvd

// cache\physical_memory_map.cpp
#define ADDRESS_PHYSICAL_MEMORY_MALLOC_FIXED                             0x0A0FC0 // _physical_memory_malloc_fixed

// cache\restricted_memory.cpp
#define ADDRESS_G_RESTRICTED_SECTION                                     0x2CB3240 // g_restricted_section
#define ADDRESS_G_RESTRICTED_REGIONS                                     0x2D79FE4 // g_restricted_regions
#define ADDRESS_C_RESTRICTED_MEMORY_ADD_MEMBER                           0x16EB30 // c_restricted_memory::add_member

// camera\camera_globals.cpp
#define ADDRESS_G_DIRECTOR_CAMERA_SPEED_SCALE                            0xEEEEB8 // g_director_camera_speed_scale

// camera\debug_director.cpp
#define ADDRESS_C_DIRECTOR_UPDATE                                        0x0E57D0 // c_director::update

// camera\director.cpp
#define ADDRESS_C_DIRECTOR_SET_CAMERA_MODE_INTERNAL                      0x0E5B20 // c_director::set_camera_mode_internal
#define ADDRESS_DIRECTOR_SET_MODE                                        0x0E5540 // director_set_mode

// cseries\async.cpp
#define ADDRESS_ASYNC_YIELD_UNTIL_DONE_FUNCTION                          0x099590 // async_yield_until_done_function

// cseries\async_helpers.cpp
#define ADDRESS_ASYNC_READ_POSITION                                      0x0C6080 // async_read_position

// cseries\async_xoverlapped.cpp
#define ADDRESS_OVERLAPPED_UPDATE                                        0x0C7740 // overlapped_update

// cseries\cseries.cpp
#define ADDRESS_G_NORMAL_ALLOCATION                                      0xEE6960 // g_normal_allocation
#define ADDRESS_BIT_VECTOR_COUNT_BITS                                    0x0C5990 // bit_vector_count_bits
#define ADDRESS_INDEX_FROM_MASK                                          0x0C5BD0 // player_index_from_mask

// cseries\cseries_windows_debug_pc.cpp
#define ADDRESS_G_EXCEPTION_TIME                                         0x10BFB60 // g_exception_time
#define ADDRESS_G_EXCEPTION_CACHING_IN_PROGRESS                          0x2DC4A00 // g_exception_caching_in_progress
#define ADDRESS_G_EXCEPTION_INFORMATION                                  0x2DC4A04 // g_exception_information
#define ADDRESS_EXCEPTION_CODE_GET_STRING                                0x16D8D0 // exception_code_get_string
#define ADDRESS_CRASHDUMP_FROM_EXCEPTION                                 0x2C15E0 // crashdump_from_exception
#define ADDRESS_BUILD_EXCEPTION_POINTERS                                 0x16D620 // build_exception_pointers

// cseries\language.cpp
#define ADDRESS_GET_CURRENT_LANGUAGE                                     0x0B2840 // get_current_language

// cseries\progress.cpp
#define ADDRESS_PROGRESS_GLOBALS                                         0x3F36738 // progress_globals

// game\game.cpp
#define ADDRESS_G_DISABLE_VIDEO                                          0x109FA3F // g_disable_video
#define ADDRESS_G_DISABLE_AUDIO                                          0x109FA3D // g_disable_audio
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE                    0x0E0020 // game_engine_teams_use_one_shared_life

// game\game_allegiance.cpp
#define ADDRESS_GAME_TEAM_IS_ENEMY                                       0x176AF0 // game_team_is_enemy

// game\game_engine.cpp
#define ADDRESS_K_GAME_ENGINE_END_CONDITIONS                             0xEE7260 // k_game_engine_end_conditions
#define ADDRESS_GAME_ENGINES                                             0xF4D5D4 // game_engines
#define ADDRESS_GAME_ENGINE_PLAYER_IS_PLAYING                            0x0C9730 // game_engine_player_is_playing
#define ADDRESS_GAME_ENGINE_GET_MULTIPLAYER_STRING                       0x0CF270 // game_engine_get_multiplayer_string

// game\game_engine_assault.cpp
#define ADDRESS_C_GAME_ENGINE_ASSAULT_VARIANT_CONSTRUCTOR_VFTABLE        0xDAE260 // ?_7c_game_engine_assault_variant
#define ADDRESS_C_GAME_ENGINE_ASSAULT_VARIANT_SET                        0x1AD8E0 // c_game_engine_assault_variant::set

// game\game_engine_candy_monitor.cpp
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT                              0x178030 // game_engine_register_object

// game\game_engine_ctf.cpp
#define ADDRESS_C_GAME_ENGINE_CTF_VARIANT_CONSTRUCTOR_VFTABLE            0xDAE230 // ?_7c_game_engine_ctf_variant
#define ADDRESS_C_GAME_ENGINE_CTF_VARIANT_SET                            0x1B4400 // c_game_engine_ctf_variant::set

// game\game_engine_default.cpp
#define ADDRESS_C_GAME_ENGINE_BASE_VARIANT_CONSTRUCTOR_VFTABLE           0xDAE1D0 // ?_7c_game_engine_base_variant
#define ADDRESS_C_GAME_ENGINE_BASE_VARIANT_SET                           0x176010 // c_game_engine_base_variant::set_0

// game\game_engine_events.cpp
#define ADDRESS_AUDIENCE_MEMBER_FIND_RESPONSE                            0x0D2E40 // audience_member_find_response

// game\game_engine_infection.cpp
#define ADDRESS_C_GAME_ENGINE_INFECTION_VARIANT_CONSTRUCTOR_VFTABLE      0xDAE0E0 // ?_7c_game_engine_infection_variant
#define ADDRESS_C_GAME_ENGINE_INFECTION_VARIANT_SET                      0x1B3B40 // c_game_engine_infection_variant::set

// game\game_engine_juggernaut.cpp
#define ADDRESS_C_GAME_ENGINE_JUGGERNAUT_VARIANT_CONSTRUCTOR_VFTABLE     0xDAE290 // ?_7c_game_engine_juggernaut_variant
#define ADDRESS_C_GAME_ENGINE_JUGGERNAUT_VARIANT_SET                     0x1B03A0 // c_game_engine_juggernaut_variant::set

// game\game_engine_king.cpp
#define ADDRESS_C_GAME_ENGINE_KING_VARIANT_CONSTRUCTOR_VFTABLE           0xDAE170 // ?_7c_game_engine_king_variant
#define ADDRESS_C_GAME_ENGINE_KING_VARIANT_SET                           0x1B20D0 // c_game_engine_king_variant::set

// game\game_engine_oddball.cpp
#define ADDRESS_C_GAME_ENGINE_ODDBALL_VARIANT_CONSTRUCTOR_VFTABLE        0xDAE200 // ?_7c_game_engine_oddball_variant
#define ADDRESS_C_GAME_ENGINE_ODDBALL_VARIANT_SET                        0x1AFBA0 // c_game_engine_oddball_variant::set

// game\game_engine_sandbox.cpp
#define ADDRESS_C_GAME_ENGINE_SANDBOX_VARIANT_CONSTRUCTOR_VFTABLE        0xDAE110 // ?_7c_game_engine_sandbox_variant
#define ADDRESS_C_GAME_ENGINE_SANDBOX_VARIANT_SET                        0x1B1A20 // c_game_engine_sandbox_variant::set

// game\game_engine_scoring.cpp
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_0                           0x2D79E58
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_1                           0x2D79ED8
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_2                           0x2D79F58

// game\game_engine_slayer.cpp
#define ADDRESS_C_GAME_ENGINE_SLAYER_VARIANT_CONSTRUCTOR_VFTABLE         0xDAE2C0 // ?_7c_game_engine_slayer_variant
#define ADDRESS_C_GAME_ENGINE_SLAYER_VARIANT_SET                         0x1B2880 // c_game_engine_slayer_variant::set

// game\game_engine_team.cpp
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS                       0x0DFE10 // game_engine_recompute_active_teams
#define ADDRESS_GAME_ENGINE_ADJUST_TEAM_SCORE_FOR_COMPOSITION            0x0CB2C0 // game_engine_adjust_team_score_for_composition
#define ADDRESS_GAME_ENGINE_VARIANT_GET_MAXIMUM_TEAM_COUNT               0x0DFF20 // game_engine_variant_get_maximum_team_count
#define ADDRESS_GAME_ENGINE_TEAM_INDEX_TO_TEAM_DESIGNATOR                0x0DFBE0 // game_engine_team_index_to_team_designator

// game\game_engine_territories.cpp
#define ADDRESS_C_GAME_ENGINE_TERRITORIES_VARIANT_CONSTRUCTOR_VFTABLE    0xDAE140 // ?_7c_game_engine_territories_variant
#define ADDRESS_C_GAME_ENGINE_TERRITORIES_VARIANT_SET                    0x1B1240 // c_game_engine_territories_variant::set

// game\game_engine_traits.cpp
#define ADDRESS_GAME_ENGINE_ASSEMBLE_PLAYER_TRAITS                       0x1226F0 // game_engine_assemble_player_traits

// game\game_engine_util.cpp
#define ADDRESS_GAME_ENGINE_GET_MULTIPLAYER_WEAPON_SELECTION_ABSOLUTE_INDEX 0x1210D0 // game_engine_get_multiplayer_weapon_selection_absolute_index
#define ADDRESS_GAME_ENGINE_HANDLE_EVENT                                 0x0D2AD0 // game_engine_handle_event

// game\game_engine_variant.cpp
#define ADDRESS_BUILD_DEFAULT_GAME_VARIANT                               0x0EC2A0 // build_default_game_variant

// game\game_engine_vip.cpp
#define ADDRESS_C_GAME_ENGINE_VIP_VARIANT_CONSTRUCTOR_VFTABLE            0xDAE1A0 // ?_7c_game_engine_vip_variant
#define ADDRESS_C_GAME_ENGINE_VIP_VARIANT_SET                            0x1AEDC0 // c_game_engine_vip_variant::set

// game\game_globals.cpp
#define ADDRESS_K_DIFFICULTY_VALUE_INDIRECTION_TABLE                     0xD79FC0 // game_difficulty_value_table
#define ADDRESS_GLOBAL_GAME_GLOBALS                                      0x109041C // global_game_globals

// game\game_results.cpp
#define ADDRESS_G_CURRENT_GAME_RESULTS                                   0x2D602A0 // g_current_game_results
#define ADDRESS_GAME_RESULTS_NOTIFY_PLAYER_INDICES_CHANGED               0x0CFC00 // game_results_notify_player_indices_changed
#define ADDRESS_GAME_RESULTS_STATISTIC_SET_CALL                          0x0D0100 // game_results_statistic_set
#define ADDRESS_GAME_RESULTS_STATISTIC_INCREMENT                         0x0D0000 // game_results_statistic_increment

// game\player_appearance.cpp
#define ADDRESS_MODIFIER_GET_NAME                                        0x314F40 // modifier_get_name

// game\player_mapping.cpp
#define ADDRESS_PLAYER_MAPPING_GET_NEXT_OUTPUT_USER                      0x0E4640 // player_mapping_get_next_output_user
#define ADDRESS_PLAYER_MAPPING_SET_INPUT_USER                            0x0E4090 // player_mapping_set_input_user
#define ADDRESS_PLAYER_MAPPING_SET_INPUT_CONTROLLER                      0x0E4150
#define ADDRESS_PLAYER_MAPPING_ATTACH_OUTPUT_USER                        0x0E4390

// game\players.cpp
#define ADDRESS_PLAYER_CONTROL_SET_FACING                                0x10B510 // player_control_set_facing
#define ADDRESS_PLAYER_IS_LOCAL                                          0x0C3390 // player_is_local
#define ADDRESS_PLAYER_CLEAR_ASSASSINATION_STATE                         0x0BC030 // player_clear_assassination_state
#define ADDRESS_PLAYER_SWAP                                              0x0B6FF0 // player_swap
#define ADDRESS_PLAYER_DELETE                                            0x0B72D0 // player_delete
#define ADDRESS_PLAYER_SET_CONFIGURATION                                 0x0B6590 // player_set_configuration
#define ADDRESS_PLAYERS_REBUILD_USER_MAPPING                             0x0B6330 // players_rebuild_user_mapping

// hf2p\hf2p.cpp
#define ADDRESS_GAME_STARTUP_MAIN                                        0x2BCE10 // hf2p_main_initialize
#define ADDRESS_GAME_STARTUP_CLIENT                                      0x2BD0F0 // game_startup_client
#define ADDRESS_HF2P_SESSION_INVALID                                     0x585C30 // hf2p_session_invalid

// hf2p\hf2p_session.cpp
#define ADDRESS_HF2P_SETUP_SESSION                                       0x3CBE50 // hf2p_setup_session
#define ADDRESS_HF2P_HANDLE_DISCONNECTION                                0x31F220 // hf2p_handle_disconnection
#define ADDRESS_HF2P_JOIN_GAME                                           0x32F2F0 // hf2p_join_game

// hf2p\hq.cpp
#define ADDRESS_HQ_START_TUTORIAL_LEVEL                                  0x350420 // tutorial_start

// hf2p\loadouts.cpp
#define ADDRESS_PLAYER_UPDATE_LOADOUT_INTERNAL                           0x0A9350 // player_update_loadout_internal
#define ADDRESS_EQUIPMENT_ADD                                            0x0E3E60 // equipment_add

// hf2p\podium.cpp
#define ADDRESS_G_PLAYER_PODIUM_COUNT                                    0x37E38BC // g_player_podiums_count
#define ADDRESS_G_PLAYER_PODIUMS                                         0x37E38C0 // g_player_podiums
#define ADDRESS_HF2P_PLAYER_PODIUM_INCREMENT_LOOP_COUNT                  0x315CE0 // hf2p_player_podium_increment_loop_count

// hs\hs_function.cpp
#define ADDRESS_HS_FUNCTION_TABLE                                        0xEEF198 // hs_function_table

// hs\hs_runtime.cpp
#define ADDRESS_HS_RETURN                                                0x12B1D0 // hs_return
#define ADDRESS_HS_ARGUMENTS_EVALUATE                                    0x12B960 // hs_arguments_evaluate

// input\input_windows.cpp
#define ADDRESS_INPUT_GLOBALS                                            0x2B47388 // input_globals
#define ADDRESS_INPUT_CLEAR_ALL_RUMBLERS                                 0x082390 // input_clear_all_rumblers
#define ADDRESS_INPUT_UPDATE                                             0x081830 // input_update

// interface\interface_constants.cpp
#define ADDRESS_G_ASPECT_RATIO_SCALE                                     0xEFBEC4 // g_aspect_ratio_scale
#define ADDRESS_CALCULATE_ASPECT_RATIO_SCALING                           0x3E6890 // calculate_aspect_ratio_scaling

// interface\user_interface_controller.cpp
#define ADDRESS_USER_INTERFACE_CONTROLLER_GET_SIGNED_IN_CONTROLLER_COUNT 0x3CE700 // user_interface_controller_get_signed_in_controller_count

// interface\user_interface_session.cpp
#define ADDRESS_USER_INTERFACE_SQUAD_SET_GAME_VARIANT                    0x3CCC40 // user_interface_squad_set_game_variant
#define ADDRESS_USER_INTERFACE_SQUAD_SET_MULTIPLAYER_MAP                 0x3CCAC0 // user_interface_squad_set_multiplayer_map
#define ADDRESS_USER_INTERFACE_SET_DESIRED_MULTIPLAYER_MODE              0x3CB710 // user_interface_set_desired_multiplayer_mode

// items\projectiles.cpp
#define ADDRESS_PROJECTILE_DETONATE_EFFECTS_AND_DAMAGE_SHARED            0x487300 // projectile_detonate_effects_and_damage_shared

// items\weapons.cpp
#define ADDRESS_WEAPON_DELAY_PREDICTED_STATE                             0x452D20 // weapon_delay_predicted_state
#define ADDRESS_WEAPON_GET_OWNER_UNIT_INVENTORY_INDEX                    0x4547A0 // weapon_get_owner_unit_inventory_index

// main\console.cpp
#define ADDRESS_NET_SHOW_LATENCY_AND_FRAMERATE_METRICS_ON_CHUD           0x108B78C // g_network_interface_show_latency_and_framerate_metrics_on_chud
#define ADDRESS_NET_FAKE_LATENCY_AND_FRAMERATE_METRICS_ON_CHUD           0x1089F03 // g_network_interface_fake_latency_and_framerate_metrics_on_chud

// main\debug_keys.cpp
#define ADDRESS_DISPLAY_FRAMERATE                                        0x1090457 // display_framerate

// main\levels.cpp
#define ADDRESS_LEVELS_ADD_CAMPAIGN                                      0x0E1B30 // levels_add_campaign
#define ADDRESS_LEVELS_GET_AVAILABLE_MAP_MASK                            0x0E1060 // build_peer_mp_map_mask

// main\main.cpp
#define ADDRESS_G_STATUS_VALUES                                          0x1090458 // g_status_values
#define ADDRESS_MAIN_GLOBALS                                             0x2C7E970 // main_globals
#define ADDRESS_INTERNAL_HALT_RENDER_THREAD_AND_LOCK_RESOURCES           0x095DA0 // _internal_halt_render_thread_and_lock_resources
#define ADDRESS_LAST_RESOURCE_OWNER                                      ADDRESS_NOT_PRESENT // $TODO:
#define ADDRESS_UNLOCK_RESOURCES_AND_RESUME_RENDER_THREAD                0x095EF0 // unlock_resources_and_resume_render_thread

// main\main.h
#define ADDRESS_MAIN_STATUS                                              0x097E90 // main_status

// main\main_game.cpp
#define ADDRESS_MAIN_GAME_GLOBALS                                        0x2D2B4B8 // main_game_globals

// main\main_render.cpp
#define ADDRESS_GAME_SESSION_ID                                          0x37F3650
#define ADDRESS_G_WATERMARK_SCALES                                       0xDBE230 // g_watermark_scales
#define ADDRESS_MAIN_RENDER_PREGAME                                      0x169510 // main_render_pregame

// main\main_time.cpp
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY                             0x0A0830 // main_time_frame_rate_display

// math\random_math.cpp
#define ADDRESS_RANDOM_SEED_LOCAL                                        0x3F3103C // g_nondeterministic_random_seed

// math\real_math.cpp
#define ADDRESS_GENERATE_UP_VECTOR3D                                     0x0858B0 // generate_up_vector3d

// memory\bitstream.cpp
#define ADDRESS_C_BITSTREAM_WRITE_ACCUMULATOR_TO_MEMORY                  0x0A8B40 // c_bitstream::write_accumulator_to_memory
#define ADDRESS_C_BITSTREAM_FINISH_WRITING                               0x0A6A60 // c_bitstream::finish_writing
#define ADDRESS_C_BITSTREAM_RESET                                        0x0A8600 // c_bitstream::reset
#define ADDRESS_C_BITSTREAM_READ_BOOL                                    0x0A8D10 // c_bitstream::read_bit_internal

// memory\data.cpp
#define ADDRESS_DATA_NEXT_ABSOLUTE_INDEX                                 0x0AAC20 // data_next_absolute_index
#define ADDRESS_DATUM_DELETE                                             0x0AAB60 // datum_delete
#define ADDRESS_DATA_DELETE_ALL                                          0x0AA7C0 // data_delete_all
#define ADDRESS_DATA_INITIALIZE                                          0x0AA560 // data_initialize

// memory\read_write_lock.cpp
#define ADDRESS_C_READ_WRITE_LOCK_SETUP                                  0x1BBE00 // c_read_write_lock::setup
#define ADDRESS_C_READ_WRITE_LOCK_WRITE_LOCK                             0x1BBEA0 // c_read_write_lock::write_lock
#define ADDRESS_C_READ_WRITE_LOCK_WRITE_UNLOCK                           0x1BBF80 // c_read_write_lock::write_unlock

// multithreading\synchronization.cpp
#define ADDRESS_G_SYNCH_GLOBALS                                          0x2C7DCB4 // g_synch_globals
#define ADDRESS_INTERNAL_SEMAPHORE_RELEASE                               0x0952F0 // internal_semaphore_release
#define ADDRESS_INTERNAL_SEMAPHORE_TAKE                                  0x095290 // internal_semaphore_take
#define ADDRESS_RELEASE_LOCKS_SAFE_FOR_CRASH_RELEASE                     0x095520 // release_locks_safe_for_crash_release

// multithreading\threads.cpp
#define ADDRESS_K_REGISTERED_THREAD_DEFINITIONS                          0xD7A068 // k_registered_thread_definitions
#define ADDRESS_G_THREAD_GLOBALS                                         0x1096768 // g_thread_globals
#define ADDRESS_G_THREAD_OWNING_DEVICE                                   0x374E7E0 // g_thread_owning_device
#define ADDRESS_START_THREAD                                             0x0A6750 // start_thread

// networking\delivery\network_channel.cpp
#define ADDRESS_C_NETWORK_CHANNEL_OPEN                                   0x00BE30 // c_network_channel::open
#define ADDRESS_C_NETWORK_CHANNEL_SEND_CONNECTION_ESTABLISHED            0x00BF90 // c_network_channel::send_connection_established

// networking\delivery\network_link.cpp
#define ADDRESS_C_NETWORK_LINK_GET_ASSOCIATED_CHANNEL                    0x006590 // c_network_link::get_associated_channel
#define ADDRESS_C_NETWORK_LINK_SEND_OUT_OF_BAND                          0x006800 // c_network_link::send_out_of_band

// networking\logic\life_cycle\life_cycle_handler_end_game_write_stats.cpp
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE_SESSION_START 0x04CAA0 // c_life_cycle_state_handler_end_game_write_stats::update_session_start
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE_SESSION_END 0x04CB10 // c_life_cycle_state_handler_end_game_write_stats::update_session_end

// networking\logic\life_cycle\life_cycle_state_handler.cpp
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_ALL_PEERS_HAVE_MAIN_MENU_READY 0x04BF40 // c_life_cycle_state_handler::all_peers_have_main_menu_ready

// networking\logic\network_join.cpp
#define ADDRESS_G_NETWORK_JOIN_DATA                                      0x108B790 // g_network_join_data
#define ADDRESS_NETWORK_JOIN_FLUSH_JOIN_QUEUE                            0x02A5D0 // network_join_flush_join_queue

// networking\logic\network_life_cycle.cpp
#define ADDRESS_LIFE_CYCLE_GLOBALS                                       0x2AC8240 // simulation_globals
#define ADDRESS_NETWORK_LIFE_CYCLE_END                                   0x02ABB0 // network_life_cycle_end
#define ADDRESS_NETWORK_LIFE_CYCLE_CREATE_LOCAL_SQUAD                    0x02AC90 // network_life_cycle_create_local_squad

// networking\logic\network_session_interface.cpp
#define ADDRESS_SESSION_INTERFACE_GLOBALS                                0x2AC8358 // session_interface_globals
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION                 0x02F360 // network_session_interface_update_session
#define ADDRESS_NETWORK_SESSION_INTERFACE_GET_LOCAL_USER_IDENTIFIER      0x003D50 // network_session_interface_get_local_user_identifier
#define ADDRESS_NETWORK_SESSION_CALCULATE_PEER_CONNECTIVITY              0x02E450 // network_session_calculate_peer_connectivity
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE                         0x02DBB0 // network_session_interface_update

// networking\messages\network_message_gateway.cpp
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_RECEIVE_OUT_OF_BAND_PACKET     0x023120 // c_network_message_gateway::receive_out_of_band_packet
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_SEND_MESSAGE_DIRECTED          0x023240 // c_network_message_gateway::send_message_directed
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_READ_PACKET_HEADER             0x023540 // c_network_message_gateway::read_packet_header

// networking\messages\network_message_handler.cpp
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_CONNECT_REFUSE          0x025A40 // c_network_message_handler::handle_connect_refuse
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_JOIN_REFUSE             0x0255E0 // c_network_message_handler::handle_join_refuse
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_LEAVE_ACKNOWLEDGE       0x025660 // c_network_message_handler::handle_leave_acknowledge
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_VIEW_ESTABLISHMENT      0x025730 // c_network_message_handler::handle_view_establishment
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_PLAYER_ACKNOWLEDGE      0x025790 // c_network_message_handler::handle_player_acknowledge
#define ADDRESS_HANDLE_SYNCHRONOUS_UPDATE_CALL                           0x0257E0 // c_network_message_handler::handle_synchronous_update
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_PLAYBACK_CONTROL 0x025860 // c_network_message_handler::handle_synchronous_playback_control
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_ACTIONS     0x025910 // c_network_message_handler::handle_synchronous_actions
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_GAMESTATE   0x0259A0 // c_network_message_handler::handle_synchronous_gamestate
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_DISTRIBUTED_GAME_RESULTS 0x0259F0 // c_network_message_handler::handle_game_results
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_CLIENT_READY 0x0258D0 // c_network_message_handler::handle_synchronous_client_ready

// networking\messages\network_message_queue.cpp
#define ADDRESS_C_NETWORK_MESSAGE_QUEUE_SEND_MESSAGE                     0x016B80 // c_network_message_queue::send_message

// networking\messages\network_message_type_collection.cpp
#define ADDRESS_C_NETWORK_MESSAGE_TYPE_COLLECTION_DECODE_MESSAGE_HEADER  0x038770 // c_network_message_type_collection::decode_message_header

// networking\network_configuration.cpp
#define ADDRESS_G_NETWORK_CONFIGURATION                                  0x108A050 // g_network_configuration
#define ADDRESS_G_NETWORK_CONFIGURATION_INITIALIZED                      0x1089F02 // g_network_configuration_initialized

// networking\network_globals.cpp
#define ADDRESS_NETWORK_GLOBALS                                          0x2A9BCDC // network_globals
#define ADDRESS_G_GAME_PORT                                              0xEE67A0 // g_broadcast_port
#define ADDRESS_G_NETWORK_SESSION_MANAGER                                0x108B768 // g_network_session_manager

// networking\network_memory.cpp
#define ADDRESS_NETWORK_SHARED_MEMORY_GLOBALS                            0x1089F04 // network_shared_memory_globals
#define ADDRESS_NETWORK_BASE_MEMORY_GLOBALS                              0x255D6F0 // network_base_memory_globals

// networking\network_time.cpp
#define ADDRESS_NETWORK_TIME_GLOBALS                                     0x1089FC4 // network_time_globals

// networking\replication\replication_entity_manager_view.cpp
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_SET_STATE              0x020C20 // c_replication_entity_manager_view::set_state
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_CLEAR_ENTITY_MASK      0x01F720 // c_replication_entity_manager_view::clear_entity_mask
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_SET_ENTITY_MASK        0x01F6E0 // c_replication_entity_manager_view::set_entity_mask

// networking\session\network_managed_session.cpp
#define ADDRESS_ONLINE_SESSION_MANAGER_GLOBALS                           0x2AC53B8 // online_session_manager_globals
#define ADDRESS_MANAGED_SESSION_COMPARE_ID                               0x028AD0 // managed_session_compare_id
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL                  0x028BC0 // managed_session_delete_session_internal

// networking\session\network_observer.cpp
#define ADDRESS_C_NETWORK_OBSERVER_HANDLE_CONNECT_REQUEST                0x010E40 // c_network_observer::handle_connect_request
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_INITIATE_CONNECTION  0x00F980 // c_network_observer::observer_channel_initiate_connection
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_SEND_MESSAGE         0x00F450 // c_network_observer::observer_channel_send_message
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_BACKLOGGED           0x00F890 // c_network_observer::observer_channel_backlogged
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_SET_WAITING_ON_BACKLOG 0x00F910 // c_network_observer::observer_channel_set_waiting_on_backlog
#define ADDRESS_C_NETWORK_OBSERVER_QUALITY_STATISTICS_GET_RATINGS        0x00EF40 // c_network_observer::quality_statistics_get_ratings
#define ADDRESS_C_NETWORK_OBSERVER_QUALITY_STATISTICS_REPORT_BADNESS     0x00F170 // c_network_observer::quality_statistics_report_badness

// networking\session\network_session.cpp
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PEER_CONNECT                    0x04B220 // c_network_session::handle_peer_connect
#define ADDRESS_C_NETWORK_SESSION_HANDLE_SESSION_DISBAND                 0x04B410 // c_network_session::handle_session_disband
#define ADDRESS_C_NETWORK_SESSION_HANDLE_SESSION_BOOT                    0x04B4A0 // c_network_session::handle_session_boot
#define ADDRESS_C_NETWORK_SESSION_HANDLE_HOST_DECLINE                    0x04B530 // c_network_session::handle_host_decline
#define ADDRESS_C_NETWORK_SESSION_CHANNEL_IS_AUTHORITATIVE               0x022720 // c_network_session::channel_is_authoritative
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PLAYER_REFUSE                   0x04B700 // c_network_session::handle_player_refuse
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PARAMETERS_UPDATE               0x04B310 // c_network_session::handle_parameters_update
#define ADDRESS_C_NETWORK_SESSION_DISCONNECT                             0x021C40 // c_network_session::disconnect
#define ADDRESS_C_NETWORK_SESSION_IS_PEER_JOINING_THIS_SESSION           0x022470 // c_network_session::is_peer_joining_this_session
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_CREATING                     0x03E730 // _dynamic_initializer_for__module_base___5
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_JOINING                      0x03E7E0 // c_network_session::idle_peer_joining
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_JOIN_ABORT                   0x03E930 // c_network_session::idle_peer_join_abort
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_LEAVING                      0x03E990 // c_network_session::idle_peer_leaving
#define ADDRESS_C_NETWORK_SESSION_GET_MAXIMUM_PLAYER_COUNT               0x022E00 // c_network_session::get_maximum_player_count
#define ADDRESS_C_NETWORK_SESSION_CHECK_TO_SEND_TIME_SYNCHRONIZATION     0x022AD0 // c_network_session::check_to_send_time_synchronization
#define ADDRESS_C_NETWORK_SESSION_IDLE_OBSERVER_STATE                    0x03E9F0 // c_network_session::idle_observer_state
#define ADDRESS_C_NETWORK_SESSION_PEER_REQUEST_PLAYER_ADD                0x021E70 // c_network_session::peer_request_player_add
#define ADDRESS_C_NETWORK_SESSION_INITIATE_LEAVE_PROTOCOL                0x021BB0 // c_network_session::initiate_leave_protocol

// networking\session\network_session_manager.cpp
#define ADDRESS_C_NETWORK_SESSION_MANAGER_GET_SESSION                    0x027C10 // c_network_session_manager::get_session

// networking\session\network_session_membership.cpp
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_SECURE_ADDRESS 0x031200 // c_network_session_membership::get_peer_from_secure_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ADD_PEER                    0x030D30 // c_network_session_membership::add_peer
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_INCOMING_ADDRESS 0x031180 // c_network_session_membership::get_peer_from_incoming_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_IDLE                        0x032840 // c_network_session_membership::idle
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ALL_PEERS_ESTABLISHED       0x020DD0 // c_network_session_membership::all_peers_established
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PEER                 0x030DA0 // c_network_session_membership::remove_peer
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_FIND_PLAYER_IN_PLAYER_ADD_QUEUE 0x032CF0 // c_network_session_membership::find_player_in_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_FROM_PLAYER_ADD_QUEUE 0x032B60 // c_network_session_membership::remove_player_from_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_SET_PLAYER_PROPERTIES       0x031B60 // c_network_session_membership::set_player_properties
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_INTERNAL      0x031AD0 // c_network_session_membership::remove_player_internal
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PLAYER_FROM_IDENTIFIER  0x031800 // c_network_session_membership::get_player_from_identifier
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ADD_PLAYER_TO_PLAYER_ADD_QUEUE 0x032AA0 // c_network_session_membership::add_player_to_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_OBSERVER_CHANNEL 0x0310D0 // c_network_session_membership::get_peer_from_observer_channel
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_HOST_EXISTS_AT_INCOMING_ADDRESS 0x0312C0 // c_network_session_membership::host_exists_at_incoming_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_HANDLE_MEMBERSHIP_UPDATE    0x031D30 // c_network_session_membership::handle_membership_update
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_PEER_PROPERTY_FLAG_TEST     0x0327F0 // c_network_session_membership::peer_property_flag_test

// networking\session\network_session_parameters.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETERS_CHECK_TO_SEND_UPDATES       0x01A360 // c_network_session_parameters::check_to_send_updates
#define ADDRESS_C_NETWORK_SESSION_PARAMETERS_CHECK_TO_SEND_CHANGE_REQUESTS 0x01A780 // c_network_session_parameters::check_to_send_change_requests

// networking\session\network_session_parameters_session.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_SESSION_SIZE_SET_MAX_PLAYER_COUNT 0x02D5F0 // c_network_session_parameter_session_size::set_max_player_count
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_SESSION_MODE_SET             0x02D7A0 // c_network_session_parameter_session_mode::set

// networking\session\network_session_parameters_ui.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_UI_GAME_MODE_REQUEST_CHANGE  0x03B430 // c_network_session_parameter_ui_game_mode::request_change

// networking\transport\transport.cpp
#define ADDRESS_TRANSPORT_GLOBALS                                        0x3F30960 // transport_globals
#define ADDRESS_TRANSPORT_INITIALIZE                                     0x0039C0 // transport_initialize
#define ADDRESS_TRANSPORT_STARTUP                                        0x003A50 // transport_startup

// networking\transport\transport_address.cpp
#define ADDRESS_TRANSPORT_ADDRESS_EQUIVALENT                             0x0077D0 // transport_address_equivalent

// networking\transport\transport_security.cpp
#define ADDRESS_TRANSPORT_SECURITY_GLOBALS                               0x3F309E8
#define ADDRESS_G_SESSION_SECURE_ADDRESS                                 0x375E508 // g_session_secure_address2

// networking\transport\transport_shim.cpp
#define ADDRESS_G_XNET_SHIM_TABLE                                        0x375D7A8 // g_transport_address_mapping

// objects\object_scripting.cpp
#define ADDRESS_OBJECT_SCRIPTING_CLEAR_ALL_FUNCTION_VARIABLES            0x496650 // object_scripting_clear_all_function_variables

// objects\object_types.cpp
#define ADDRESS_OBJECT_TYPE_DEFINITIONS                                  0xEFD558 // object_type_definitions

// objects\objects.cpp
#define ADDRESS_OBJECT_TRY_AND_GET_AND_VERIFY_TYPE                       0x423A10 // object_try_and_get_and_verify_type
#define ADDRESS_OBJECT_WAKE                                              0x41C5B0 // object_wake
#define ADDRESS_OBJECT_SET_REQUIRES_MOTION                               0x424860 // object_set_requires_motion
#define ADDRESS_OBJECT_NEEDS_RIGID_BODY_UPDATE                           0x41ED30 // object_needs_rigid_body_update
#define ADDRESS_ATTACHMENTS_UPDATE                                       0x4299F0 // attachments_update
#define ADDRESS_OBJECT_COMPUTE_NODE_MATRICES                             0x4260B0 // object_compute_node_matrices
#define ADDRESS_OBJECT_NEW                                               0x41D630 // object_new
#define ADDRESS_OBJECT_SET_GARBAGE                                       0x424660 // object_set_garbage
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL                             0x41C6D0 // object_set_position_internal
#define ADDRESS_OBJECT_GET_ORIGIN_INTERPOLATED                           0x421DD0 // object_get_origin_interpolated
#define ADDRESS_OBJECT_DELETE                                            0x41E7D0 // object_delete
#define ADDRESS_OBJECT_TRY_AND_GET_MULTIPLAYER                           0x42A140 // object_try_and_get_multiplayer

// objects\scenery.cpp
#define ADDRESS_SCENERY_ANIMATION_IDLE                                   0x4B0C10 // scenery_animation_idle

// online\online_lsp.cpp
#define ADDRESS_G_ONLINE_LSP_MANAGER                                     0x255D258 // g_online_lsp_manager
#define ADDRESS_C_ONLINE_LSP_MANAGER_GO_INTO_CRASH_MODE                  0x005E80 // c_online_lsp_manager::go_into_crash_mode

// physics\collisions.cpp
#define ADDRESS_COLLISION_TEST_VECTOR_TARGET                             0x1A7DB0 // collision_test_vector

// physics\collisions.h
#define ADDRESS_COLLISION_TEST_FOR_PROJECTILES_FLAGS                     0x2E88084 // _collision_test_for_projectiles_flags
#define ADDRESS_COLLISION_TEST_PATHFINDING_FLAGS                         0x2E8801C // _collision_test_pathfinding_flags

// physics\havok_component.cpp
#define ADDRESS_G_HAVOK_COMPONENTS                                       0x109F9B0 // g_havok_component_data
#define ADDRESS_C_HAVOK_COMPONENT_FORCE_ACTIVATE                         0x131B20 // c_havok_component::force_activate

// rasterizer\dx9\rasterizer_dx9_main.cpp
#define ADDRESS_C_RASTERIZER_SET_SAMPLER_FILTER_MODE_CUSTOM_DEVICE_NO_CACHE 0x269200 // c_rasterizer::set_sampler_filter_mode_custom_device_no_cache

// rasterizer\rasterizer.cpp
#define ADDRESS_RASTERIZER_RENDER_GLOBALS                                0xEF71BC // c_rasterizer::render_globals
#define ADDRESS_RASTERIZER_G_LAST_VIEWPORT                               0x3F412D0 // c_rasterizer::g_last_viewport
#define ADDRESS_RASTERIZER_G_CURRENT_VERTEX_DECLARATION                  0x238AD80 // c_rasterizer::g_current_vertex_declaration
#define ADDRESS_RASTERIZER_G_DEVICE                                      0x238936C // c_rasterizer::g_device
#define ADDRESS_RASTERIZER_G_CURRENT_INDEX_BUFFER                        0x238AD6C // c_rasterizer::g_current_index_buffer
#define ADDRESS_RASTERIZER_X_LAST_SAMPLER_FILTER_MODES                   0xEF5EC0 // c_rasterizer::x_last_sampler_filter_modes
#define ADDRESS_RASTERIZER_X_LAST_RENDER_STATE_VALUE                     0x238AD84 // c_rasterizer::x_last_render_state_value
#define ADDRESS_RASTERIZER_G_CURRENT_ALPHA_BLEND_MODE                    0x238AD98 // c_rasterizer::g_current_alpha_blend_mode
#define ADDRESS_C_RASTERIZER_GET_DISPLAY_PIXEL_BOUNDS                    0x265320 // c_rasterizer::get_display_pixel_bounds
#define ADDRESS_C_RASTERIZER_GET_DISPLAY_TITLE_SAFE_PIXEL_BOUNDS         0x265390 // c_rasterizer::get_display_title_safe_pixel_bounds
#define ADDRESS_C_RASTERIZER_RESTORE_LAST_VIEWPORT                       0x265C30 // c_rasterizer::restore_last_viewport
#define ADDRESS_C_RASTERIZER_SET_DEPTH_STENCIL_SURFACE                   0x26B890 // c_rasterizer::set_depth_stencil_surface
#define ADDRESS_C_RASTERIZER_SET_VERTEX_SHADER                           0x2697C0 // c_rasterizer::set_vertex_shader
#define ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER                            0x2696F0 // c_rasterizer::set_pixel_shader
#define ADDRESS_C_RASTERIZER_DRAW_PRIMITIVE_UP                           0x26DC00 // c_rasterizer::draw_primitive_up
#define ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER_CONSTANT                   0x29D5A0 // c_rasterizer::set_pixel_shader_constant
#define ADDRESS_C_RASTERIZER_SET_Z_BUFFER_MODE                           0x2682A0 // c_rasterizer::set_z_buffer_mode
#define ADDRESS_C_RASTERIZER_SET_CULL_MODE                               0x269030 // c_rasterizer::set_cull_mode
#define ADDRESS_C_RASTERIZER_SET_ALPHA_BLEND_MODE_CUSTOM_DEVICE_NO_CACHE 0x267E60 // c_rasterizer::set_alpha_blend_mode_custom_device_no_cache
#define ADDRESS_C_RASTERIZER_BEGIN_FRAME                                 0x266C10 // c_rasterizer::begin_frame
#define ADDRESS_C_RASTERIZER_SETUP_TARGETS_SIMPLE                        0x26A320 // c_rasterizer::setup_targets_simple
#define ADDRESS_C_RASTERIZER_END_FRAME                                   0x2671A0 // c_rasterizer::end_frame

// rasterizer\rasterizer_resource_definitions.cpp
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_LIGHTING_VERTEX_DECLS         0xD8D550 // c_vertex_declaration_table::lighting_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_TRANSFER_VERTEX_DECLS         0xD8C548 // c_vertex_declaration_table::transfer_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_OTHER_VERTEX_DECLS            0xD8CD48 // c_vertex_declaration_table::other_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_BASE_VERTEX_DECLS             0xEF7DD0 // c_vertex_declaration_table::base_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_M_VERTEX_DECLARATIONS         0x23B6240 // c_vertex_declaration_table::m_vertex_declarations

// render\render.cpp
#define ADDRESS_C_RENDER_GLOBALS_M_FRAME_INDEX                           0xEF7248 // c_render_globals::m_frame_index

// render\render_cameras.cpp
#define ADDRESS_RENDER_CAMERA_VIEW_TO_SCREEN                             0x2A0A40 // render_camera_view_to_screen

// render\views\render_view.cpp
#define ADDRESS_C_VIEW_G_VIEW_STACK_TOP                                  0xEF71F0 // c_view::g_view_stack_top
#define ADDRESS_C_VIEW_G_VIEW_STACK                                      0x2391A00 // c_view::g_view_stack
#define ADDRESS_C_PLAYER_VIEW_X_CURRENT_PLAYER_VIEW                      0x2391A10 // c_player_view::x_current_player_view

// scenario\scenario_map_variant.cpp
#define ADDRESS_C_MAP_VARIANT_C_MAP_VARIANT                              0x0ACF40 // c_map_variant::c_map_variant
#define ADDRESS_C_MAP_VARIANT_CREATE_DEFAULT                             0x0ACFD0 // c_map_variant::create_default

// shell\shell.cpp
#define ADDRESS_SHELL_APPLICATION_PAUSED                                 0x1089F00 // shell_application_paused

// shell\shell_windows.cpp
#define ADDRESS_G_WINDOWS_PARAMS                                         0x1089F30 // window_globals

// simulation\game_interface\simulation_game_events.cpp
#define ADDRESS_SIMULATION_EVENT_GENERATE_FOR_REMOTE_PEERS_CALL          0x07EB30 // simulation_event_generate_for_remote_peers
#define ADDRESS_SIMULATION_EVENT_BUILD_ENTITY_REFERENCE_INDICES          0x07ECB0 // simulation_event_build_entity_reference_indices

// simulation\simulation.cpp
#define ADDRESS_SIMULATION_GLOBALS                                       0x3F30BC0 // simulation_globals

// simulation\simulation_event_handler.cpp
#define ADDRESS_C_SIMULATION_EVENT_HANDLER_SEND_EVENT                    0x03A860 // c_simulation_event_handler::send_event

// simulation\simulation_queue_entities.cpp
#define ADDRESS_SIMULATION_QUEUE_ENTITY_DELETION_INSERT                  0x055AA0 // simulation_queue_entity_deletion_insert

// simulation\simulation_view.cpp
#define ADDRESS_C_SIMULATION_VIEW_SET_VIEW_ESTABLISHMENT                 0x033510 // c_simulation_view::set_view_establishment

// simulation\simulation_watcher.cpp
#define ADDRESS_C_SIMULATION_WATCHER_GET_MACHINE_INDEX_BY_IDENTIFIER     0x015630 // c_simulation_watcher::get_machine_index_by_identifier

// tag_files\files.cpp
#define ADDRESS_FILE_REFERENCE_ADD_DIRECTORY                             0x0A60C0 // file_reference_add_directory
#define ADDRESS_FILE_REFERENCE_SET_NAME                                  0x0A6140 // file_reference_set_name
#define ADDRESS_FILE_CREATE_PARENT_DIRECTORIES_IF_NOT_PRESENT            0x0A6310 // file_create_parent_directories_if_not_present
#define ADDRESS_FILE_REFERENCE_GET_NAME                                  0x0A61C0 // file_reference_get_name

// tag_files\files_windows.cpp
#define ADDRESS_FILE_WRITE                                               0x0A5200 // file_write
#define ADDRESS_FILE_GET_EOF                                             0x0A5100 // file_get_eof
#define ADDRESS_FIND_FILES_NEXT                                          0x0A5570 // find_files_next
#define ADDRESS_FIND_FILES_END                                           0x0A5520 // find_files_end
#define ADDRESS_FILE_DELETE                                              0x0A49F0 // file_delete

// tag_files\tag_resource_cache_control.cpp
#define ADDRESS_C_TAG_RESOURCE_CACHE_CONTROLLER_ACQUIRE_PAGE_RESERVATION 0x255340 // c_tag_resource_cache_controller::acquire_page_reservation
#define ADDRESS_C_TAG_RESOURCE_CACHE_CONTROLLER_RELEASE_PAGE_RESERVATION_FORCE 0x255440 // c_tag_resource_cache_controller::release_page_reservation_force

// text\draw_string.cpp
#define ADDRESS__VFTABLE                                                 0xDBD838 // ?_7c_draw_string
#define ADDRESS_C_DRAW_STRING_DRAW_MORE                                  0x172630 // c_draw_string::draw_more
#define ADDRESS_M_CHARACTER_CACHE_VFTABLE                                0xDC6008 // ?_7c_simple_font_draw_string
#define ADDRESS_M_CHARACTER_CACHE                                        0x2B8170 // ?0c_simple_font_draw_string
#define ADDRESS_M_RENDER_DATA_VFTABLE                                    0xDC2C88 // ?_7c_rasterizer_draw_string
#define ADDRESS_DRAW_STRING_GET_GLYPH_SCALING_FOR_DISPLAY_SETTINGS       0x173680 // draw_string_get_glyph_scaling_for_display_settings

// text\font_cache.cpp
#define ADDRESS_G_INTERNAL_FONT_CACHE_GLOBALS                            0x2E04E48 // g_internal_font_cache_globals
#define ADDRESS_M_LOCKED_VFTABLE                                         0xDBD81C // ?_7c_font_cache_mt_safe
#define ADDRESS_FONT_CACHE_NEW                                           0x1702D0 // font_cache_new

// text\font_loading.cpp
#define ADDRESS_G_FONT_GLOBALS                                           0x2D23300 // g_font_globals

// text\font_package_cache.cpp
#define ADDRESS_FONT_PACKAGE_GET                                         0x16FDC0 // font_package_get
#define ADDRESS_FONT_PACKAGE_GET_CHARACTER                               0x171410 // font_package_get_character
#define ADDRESS_PACKAGE_TABLE_SEARCH_FUNCTION                            0x1713D0 // package_table_search_function
#define ADDRESS_FONT_PACKAGE_CACHE_NEW                                   0x16FAA0 // font_package_cache_new

// units\bipeds.cpp
#define ADDRESS_BIPED_CALCULATE_MELEE_AIMING                             0x461360 // biped_calculate_melee_aiming

// units\units.cpp
#define ADDRESS_UNIT_SET_ACTIVELY_CONTROLLED                             0x4436E0 // unit_set_actively_controlled
#define ADDRESS_UNIT_DROP_ITEM                                           0x446CA0 // unit_drop_item
#define ADDRESS_UNIT_CONTROL                                             0x43C1F0 // unit_control
#define ADDRESS_UNIT_INVENTORY_GET_WEAPON                                0x443530 // unit_inventory_get_weapon

// visibility\visibility_collection.cpp
#define ADDRESS_G_VISIBILITY_GLOBALS_KEEPER                              0x2E589F0 // g_visibility_globals_keeper
#define ADDRESS_VISIBILITY_VOLUME_TEST_SPHERE                            0x1D46F0 // visibility_volume_test_sphere

// addresses which only exist in some engine versions

// anvil\hooks\simulation\hooks_player_updates.cpp
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK3                              0x3174B4 // sub_717360+0x154
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK3_RETURN                       0x3174BB
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK4                              0x0588BA // sub_4587C0+0xfa
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK4_RETURN                       0x0588C1

// anvil\hooks\hooks_miscellaneous.cpp
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_1           0x350E12
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_2           0x350F22
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_3           0x350FCE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_4           0x35105E
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_5           0x3510DE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_6           0x35115E
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_7           0x3511DE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_8           0x35125E
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_9           0x3512DE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_10          0x35135C
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_11          0x35137B
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_12          0x35139A
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_13          0x351A25
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_14          0x351BB5
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_15          0x351D7D
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_MS30_CALL_16          0x3520D5

// engine version specific lengths
#define LENGTH_UNIT_HANDLE_EQUIPMENT_ENERGY_COST                         0x152
#define LENGTH_PLAYER_CAN_USE_CONSUMABLE                                 0x51
#define LENGTH_GAME_ENGINE_SHOULD_SPAWN_PLAYER                           0xF1
#define LENGTH_GAME_ENGINE_UPDATE_ROUND_CONDITIONS                       0x112
#define LENGTH_OBJECT_SET_DAMAGE_OWNER                                   0x73
#define LENGTH_HF2P_GAME_UPDATE_NOP                                      60 // ms30 removed the hf2p_game_statistics_service_tick call

#endif // ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
