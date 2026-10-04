#pragma once

// Engine address map for 11.1.604673 cert_ms29

#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)


// ai\path.cpp
#define ADDRESS_AI_SCRATCH_ALLOCATE                                      0x6A3250 // ai_scratch_allocate
#define ADDRESS_AI_SCRATCH_FREE                                          0x6A3300 // ai_scratch_free
#define ADDRESS_AI_POINT3D_NEW_TARGET                                    0x6BD350 // ai_point3d_new
#define ADDRESS_PATH_STATE_NEW_TARGET                                    0x72C180 // path_state_new
#define ADDRESS_PATH_STATE_DESTINATION                                   0x72C350 // path_state_destination
#define ADDRESS_PATH_STATE_FIND                                          0x72C680 // path_state_find
#define ADDRESS_PATH_STATE_BUILD_PATH_TARGET                             0x72B4E0 // path_state_build_path
#define ADDRESS_COLLISION_SURFACE_GET_SECTOR                             0x6D0AE0 // collision_surface_get_sector
#define ADDRESS_OBJECT_GET_PATHFINDING_LOCATION_TARGET                   0x708230 // object_get_pathfinding_location

// algorithms\binary_search.cpp
#define ADDRESS_BINARY_SEARCH_ELEMENTS                                   0x160FC0 // binary_search_elements

// anvil\hooks\effects\hooks_effect_system.cpp
#define ADDRESS_WRITE_PARTICLE_STATE                                     0x590560 // write_particle_state
#define ADDRESS_G_PARTICLE_STATE_WRITE_BUFFER                            0x242B628
#define ADDRESS_G_PARTICLE_STATE_WRITE_BUFFER_STRIDE                     0x2423E24
#define ADDRESS_WRITE_PARTICLE_STATE_CALL                                0x59151B
#define ADDRESS_WRITE_PARTICLE_STATE_CALL_2                              0x591628

// anvil\hooks\hooks.cpp
#define ADDRESS_CACHE_FILE_HEADER_VERIFY_PATCH                           0x082DB4 // cache_file_header_verify+0x104
#define ADDRESS_TAG_LOAD_CHECKSUM_NOP                                    0x083120
#define ADDRESS_SCENARIO_TAGS_LOAD_NOP                                   0x083AFC // scenario_tags_load+0x2fc
#define ADDRESS_ENGLISH_LANGUAGE_PATCH                                   0x2B923A

// anvil\hooks\hooks_debug.cpp
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_HOOK                       0x16B870 // font_cache_retrieve_character
#define ADDRESS_MAIN_GAME_RESET_MAP_HOOK                                 0x0A9333 // main_game_reset_map+0x1b3
#define ADDRESS_MAIN_GAME_RESET_MAP_HOOK_RETURN                          0x0A9338 // main_game_reset_map+0x1b8
#define ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK                          0x0A9603 // main_game_change_immediate+0xd3
#define ADDRESS_MAIN_GAME_CHANGE_IMMEDIATE_HOOK_RETURN                   0x0A9608 // main_game_change_immediate+0xd8
#define ADDRESS_SHELL_INITIALIZED_HOOK                                   0x001045 // shell_initialize+0x35
#define ADDRESS_SHELL_INITIALIZED_HOOK_RETURN                            0x00104D // shell_initialize+0x3d
#define ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK                          0x271938 // c_player_view::render+0x818
#define ADDRESS_RENDER_DEBUG_WINDOW_RENDER_HOOK_RETURN                   0x27193D // c_player_view::render+0x81d
#define ADDRESS_RENDER_INITIALIZE_HOOK                                   0x268BEB // vision_mode_initialize+0xeb
#define ADDRESS_RENDER_INITIALIZE_HOOK_RETURN                            0x268BF0 // vision_mode_initialize+0xf0
#define ADDRESS_MAIN_LOOP_BODY_HOOK2                                     0x0957EE // main_loop_body+0x1e
#define ADDRESS_MAIN_LOOP_BODY_HOOK2_RETURN                              0x0957F3 // main_loop_body+0x23
#define ADDRESS_GAME_TICK_HOOK1                                          0x0B1A1B // game_tick+0x6b
#define ADDRESS_GAME_TICK_HOOK1_RETURN                                   0x0B1A22 // game_tick+0x72
#define ADDRESS_GAME_TICK_HOOK2                                          0x0B1C53 // game_tick+0x2a3
#define ADDRESS_GAME_TICK_HOOK2_RETURN                                   0x0B1C5B // game_tick+0x2ab
#define ADDRESS_MAIN_LOOP_BODY_HOOK1                                     0x095883 // main_loop_body+0xb3
#define ADDRESS_MAIN_LOOP_BODY_HOOK1_RETURN                              0x095888 // main_loop_body+0xb8
#define ADDRESS_MAIN_LOOP_ENTER_HOOK1                                    0x0952A8 // main_loop_enter+0x78
#define ADDRESS_MAIN_LOOP_ENTER_HOOK1_RETURN                             0x0952AD // main_loop_enter+0x7d
#define ADDRESS_MAIN_LOOP_ENTER_HOOK2                                    0x095346 // main_loop_enter+0x116
#define ADDRESS_MAIN_LOOP_ENTER_HOOK2_RETURN                             0x09534B // main_loop_enter+0x11b
#define ADDRESS_MAIN_LOOP_EXIT_HOOK                                      0x095DB1 // main_loop_exit+0x71
#define ADDRESS_MAIN_LOOP_EXIT_HOOK_RETURN                               0x095DBB // main_loop_exit+0x7b
#define ADDRESS_RENDER_DEBUG_FRAME_RENDER_CALL                           0x1644A1 // main_render_game+0x5e1
#define ADDRESS_PRINT_HS_PRINT_1_EVALUATE_SLOT                           0xD4A234
#define ADDRESS_LOG_PRINT_HS_LOG_PRINT_1_EVALUATE_SLOT                   0xD4C028
#define ADDRESS_EVENTS_SUPPRESS_DISPLAY_EVENTS_SUPPRESS_OUTPUT_1_EVALUATE_SLOT 0xD4C67C
#define ADDRESS_MAIN_LOOP_BODY_HOOK3                                     0x0958FD // main_loop_body+0x12d
#define ADDRESS_MAIN_LOOP_BODY_HOOK3_RETURN                              0x095903 // main_loop_body+0x133
#define ADDRESS_MAIN_LOOP_BODY_HOOK4                                     0x095A9D // main_loop_body+0x2cd
#define ADDRESS_MAIN_LOOP_BODY_HOOK4_RETURN                              0x095AA4 // main_loop_body+0x2d4
#define ADDRESS_RUMBLE_UPDATE_HOOK                                       0x164856 // rumble_update+0x6
#define ADDRESS_RUMBLE_UPDATE_HOOK_RETURN                                0x16485D // rumble_update+0xd
#define ADDRESS_FONT_INITIALIZE_HOOK                                     0x09F2B7 // font_initialize+0x27
#define ADDRESS_FONT_INITIALIZE_HOOK_RETURN                              0x09F2BC // font_initialize+0x2c
#define ADDRESS_FONT_INITIALIZE_EMERGENCY                                0x09F1E0 // font_initialize_emergency
#define ADDRESS_FONT_LOADING_IDLE_HOOK                                   0x09F661 // font_loading_idle+0x1
#define ADDRESS_FONT_LOADING_IDLE_HOOK_RETURN                            0x09F666 // font_loading_idle+0x6
#define ADDRESS_FONT_LOADING_IDLE_NOP                                    0x09F6C3 // font_loading_idle+0x63
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL                       0x16B6BF // font_cache_load_internal+0x5f
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_2                     0x16B747 // font_cache_load_internal+0xe7
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_3                     0x16B768 // font_cache_load_internal+0x108
#define ADDRESS_FONT_CACHE_RETRIEVE_CHARACTER_CALL_4                     0x16B783 // font_cache_load_internal+0x123
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP                             0x16B6C7 // font_cache_load_internal+0x67
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_2                           0x16B74E // font_cache_load_internal+0xee
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_3                           0x16B76F // font_cache_load_internal+0x10f
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_NOP_4                           0x16B788 // font_cache_load_internal+0x128
#define ADDRESS_C_DRAW_STRING_CTOR_HOOK                                  0x16C152 // ?0c_draw_string+0x22
#define ADDRESS_C_DRAW_STRING_CTOR_HOOK_RETURN                           0x16C17A // ?0c_draw_string+0x4a
#define ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK                         0x16CEF8 // c_draw_string::draw_internal+0xa8
#define ADDRESS_C_DRAW_STRING_DRAW_INTERNAL_HOOK_RETURN                  0x16CF25 // c_draw_string::draw_internal+0xd5
#define ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK                      0x16D429 // c_draw_string::parse_string_new+0x69
#define ADDRESS_C_DRAW_STRING_PARSE_STRING_NEW_HOOK_RETURN               0x16D455 // c_draw_string::parse_string_new+0x95
#define ADDRESS_C_DRAW_STRING_SET_FONT                                   0x16C760 // c_draw_string::set_font
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK                        0x09FE99 // main_time_frame_rate_display+0x99
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_HOOK_RETURN                 0x09FF03 // main_time_frame_rate_display+0x103
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY_NOP                         0x09FF09 // main_time_frame_rate_display+0x109
#define ADDRESS_DIRECTOR_RENDER_HOOK                                     0x0E2BF2 // director_render+0x2b2
#define ADDRESS_DIRECTOR_RENDER_HOOK_RETURN                              0x0E2C22 // director_render+0x2e2
#define ADDRESS_DIRECTOR_RENDER_NOP                                      0x0E2C2E // director_render+0x2ee
#define ADDRESS_SUBTITLE_RENDER_HOOK                                     0x175F4F // subtitle_render+0x7f
#define ADDRESS_SUBTITLE_RENDER_HOOK_RETURN                              0x175F7F // subtitle_render+0xaf
#define ADDRESS_SUBTITLE_RENDER_NOP                                      0x175F8E // subtitle_render+0xbe
#define ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK                 0x1B0EDD // game_engine_render_watermarks+0x42d
#define ADDRESS_GAME_ENGINE_RENDER_FRAME_WATERMARKS_HOOK_RETURN          0x1B0F30 // game_engine_render_watermarks+0x480
#define ADDRESS_GAME_ENGINE_RENDER_WATERMARKS_NOP                        0x1B0F48 // game_engine_render_watermarks+0x498
#define ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK                              0x269BBC // render_fullscreen_text+0x7c
#define ADDRESS_RENDER_FULLSCREEN_TEXT_HOOK_RETURN                       0x269BEB // render_fullscreen_text+0xab
#define ADDRESS_RENDER_FULLSCREEN_TEXT_NOP                               0x269BEE // render_fullscreen_text+0xae
#define ADDRESS_CHUD_GET_STRING_WIDTH_HOOK                               0x3CE494 // chud_get_string_width+0xa4
#define ADDRESS_CHUD_GET_STRING_WIDTH_HOOK_RETURN                        0x3CE4BC // chud_get_string_width+0xcc
#define ADDRESS_CHUD_GET_STRING_WIDTH_NOP                                0x3CE482 // chud_get_string_width+0x92
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK           0x3DA0CA // c_user_interface_text::compute_text_bounds+0x14a
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_HOOK_RETURN    0x3DA0FC // c_user_interface_text::compute_text_bounds+0x17c
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP            0x3DA09C // c_user_interface_text::compute_text_bounds+0x11c
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_2          0x3DA0A4 // c_user_interface_text::compute_text_bounds+0x124
#define ADDRESS_C_USER_INTERFACE_TEXT_COMPUTE_TEXT_BOUNDS_NOP_3          0x3DA0B4 // c_user_interface_text::compute_text_bounds+0x134
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK                        0x3D9CF9 // c_user_interface_text::render+0x179
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_HOOK_RETURN                 0x3D9D25 // c_user_interface_text::render+0x1a5
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP                         0x3D9C6C // c_user_interface_text::render+0xec
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_2                       0x3D9C81 // c_user_interface_text::render+0x101
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_3                       0x3D9CA4 // c_user_interface_text::render+0x124
#define ADDRESS_C_USER_INTERFACE_TEXT_RENDER_NOP_4                       0x3D9D43 // c_user_interface_text::render+0x1c3
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK                            0x3E0EA6 // chud_build_text_geometry+0xa6
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_HOOK_RETURN                     0x3E0EE4 // chud_build_text_geometry+0xe4
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP                             0x3E0E91 // chud_build_text_geometry+0x91
#define ADDRESS_CHUD_BUILD_TEXT_GEOMETRY_NOP_2                           0x3E0EFF // chud_build_text_geometry+0xff
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK                            0x16B675 // font_cache_load_internal+0x15
#define ADDRESS_FONT_CACHE_LOAD_INTERNAL_HOOK_RETURN                     0x16B689 // font_cache_load_internal+0x29
#define ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK                       0x25F3E8 // hardware_cache_load_character+0x28
#define ADDRESS_HARDWARE_CACHE_LOAD_CHARACTER_HOOK_RETURN                0x25F3FE // hardware_cache_load_character+0x3e
#define ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK                    0x25F2E3 // hardware_cache_predict_character+0x13
#define ADDRESS_HARDWARE_CACHE_PREDICT_CHARACTER_HOOK_RETURN             0x25F300 // hardware_cache_predict_character+0x30
#define ADDRESS_SHELL_DISPOSE_HOOK                                       0x00123A // shell_dispose+0xda
#define ADDRESS_SHELL_DISPOSE_HOOK_RETURN                                0x00123F // shell_dispose+0xdf
#define ADDRESS_DIRECTOR_UPDATE_HOOK                                     0x0E280D // director_update+0x9d
#define ADDRESS_DIRECTOR_UPDATE_HOOK_RETURN                              0x0E2813 // director_update+0xa3
#define ADDRESS_C_DEBUG_DIRECTOR_UPDATE_HOOK_SLOT                        0xD81A24
#define ADDRESS_EXCEPTIONS_UPDATE                                        0x167CF0 // exceptions_update
#define ADDRESS_TOPLEVELEXCEPTIONFILTER                                  0x2B4780 // TopLevelExceptionFilter
#define ADDRESS_MAIN_HALT_AND_CATCH_FIRE                                 0x096BD0 // main_halt_and_catch_fire

// anvil\hooks\hooks_ds.cpp
#define ADDRESS_ANVIL_SESSION_UPDATE_HOOK                                0x024601 // network_update+0x51
#define ADDRESS_ANVIL_SESSION_UPDATE_HOOK_RETURN                         0x024606 // network_update+0x56
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_GAME_START_STATUS_SET        0x03BA00 // c_network_session_parameter_game_start_status::set
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_PRE_GAME_SQUAD_GAME_START_STATUS_UPDATE_HOOK 0x04DF34 // c_life_cycle_state_handler_pre_game::squad_game_start_status_update+0x954
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_PRE_GAME_SQUAD_GAME_START_STATUS_UPDATE_HOOK_RETURN 0x04DF99 // c_life_cycle_state_handler_pre_game::squad_game_start_status_update+0x9b9
#define ADDRESS_TRANSPORT_SECURE_KEY_CREATE                              0x003BC0 // transport_secure_key_create
#define ADDRESS_TRANSPORT_SECURE_ADDRESS_RESOLVE                         0x003C50 // transport_secure_address_resolve
#define ADDRESS_PEER_REQUEST_PLAYER_ADD_CALL                             0x02F5AC // network_session_interface_update_session+0x19c
#define ADDRESS_NETWORK_SESSION_INTERFACE_GET_LOCAL_USER_IDENTIFIER_CALL 0x0212CC // c_network_session::create_host_session+0x31c
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_ENTER_HOOK            0x04EAE9 // c_life_cycle_state_handler_in_game::enter+0x139
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_ENTER_HOOK_RETURN     0x04EAF3 // c_life_cycle_state_handler_in_game::enter+0x143
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_EXIT_HOOK             0x04EB7B // c_life_cycle_state_handler_in_game::exit+0x7b
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_IN_GAME_EXIT_HOOK_RETURN      0x04EB82 // c_life_cycle_state_handler_in_game::exit+0x82
#define ADDRESS_ANVIL_SCENARIO_TAGS_LOAD_TITLE_INSTANCES                 0x07E978 // scenario_load+0xa8
#define ADDRESS_ANVIL_SCENARIO_TAGS_LOAD_TITLE_INSTANCES_RETURN          0x07E9AD // scenario_load+0xdd
#define ADDRESS_HF2P_GAME_UPDATE_NOP                                     0x2B0C51 // hf2p_game_update+0x1
#define ADDRESS_HF2P_SECURITY_INITIALIZE_NOP                             0x2B0226 // hf2p_security_initialize+0x6
#define ADDRESS_MAIN_LOOP_EXIT_NOP                                       0x095D9F // main_loop_exit+0x5f
#define ADDRESS_MAIN_LOOP_EXIT_NOP_2                                     0x095DA4 // main_loop_exit+0x64
#define ADDRESS_MAIN_LOOP_PREGAME_NOP                                    0x096067 // main_loop_pregame+0x87
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL                             0x0286F4 // managed_session_synchronize_to_player_list+0x164
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_2                           0x029567 // managed_session_successful_players_remove_complete+0x27
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_3                           0x030EE3 // c_network_session_membership::remove_peer+0x93
#define ADDRESS_REMOVE_FROM_PLAYER_LIST_CALL_4                           0x031AA9 // c_network_session_membership::remove_player+0x49
#define ADDRESS_MANAGED_SESSION_SYNCHRONIZE_TO_PLAYER_LIST_NOP           0x0286F9 // managed_session_synchronize_to_player_list+0x169
#define ADDRESS_MANAGED_SESSION_SUCCESSFUL_PLAYERS_REMOVE_COMPLETE_NOP   0x02956C // managed_session_successful_players_remove_complete+0x2c
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PEER_NOP             0x030EFB // c_network_session_membership::remove_peer+0xab
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_NOP           0x031AB7 // c_network_session_membership::remove_player+0x57
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_START_GAME_ENTER_HOOK         0x04C40F // c_life_cycle_state_handler_start_game::enter+0xf
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_START_GAME_ENTER_HOOK_RETURN  0x04C415 // c_life_cycle_state_handler_start_game::enter+0x15
#define ADDRESS_CHUD_UPDATE_USER_DATA_HOOK                               0x3AF851 // c_chud_update_user_data::c_chud_update_user_data+0xa41
#define ADDRESS_CHUD_UPDATE_USER_DATA_HOOK_RETURN                        0x3AF857 // c_chud_update_user_data::c_chud_update_user_data+0xa47
#define ADDRESS_UNIT_HANDLE_EQUIPMENT_ENERGY_COST                        0x42D290 // unit_handle_equipment_energy_cost
#define ADDRESS_PLAYER_CAN_USE_CONSUMABLE                                0x0BF840 // player_can_use_consumable
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE   0x04C630 // c_life_cycle_state_handler_end_game_write_stats::update

// anvil\hooks\hooks_miscellaneous.cpp
#define ADDRESS_ENCODE_MESSAGE_HEADER_HOOK                               0x0387A0 // c_network_message_type_collection::encode_message_header
#define ADDRESS_CONTRAIL_FIX_OPERAND_VA                                  0x68A38A // c_contrail_gpu::render
#define ADDRESS_CONTRAIL_FIX_SKIP_VA                                     0x68A3E3 // c_contrail_gpu::render
#define ADDRESS_CONTRAIL_FIX_RENDER_VA                                   0x68A390 // c_contrail_gpu::render
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA                       0x318BF0 // hf2p_set_biped_texture_render_data
#define ADDRESS_C_STATIC_STRING_64_PRINT_CALL                            0x0013C3 // main_thread_routine+0x53
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL                               0x016AF8 // c_network_message_queue::send_message_in_first_fragment+0x78
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL_2                             0x016C26 // c_network_message_queue::send_message+0x76
#define ADDRESS_ENCODE_MESSAGE_HEADER_CALL_3                             0x0233D4 // c_network_message_gateway::send_message_directed+0x114
#define ADDRESS_HF2P_PLAYER_PODIUM_INITIALIZE                            0x2E8750 // hf2p_player_podium_initialize
#define ADDRESS_HF2P_PODIUM_TICK_HOOK                                    0x2E9C3A // hf2p_podium_update+0xba
#define ADDRESS_HF2P_PODIUM_TICK_HOOK_RETURN                             0x2E9C3F // hf2p_podium_update+0xbf
#define ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK 0x068CDC // c_simulation_player_taunt_request_event_definition::apply_game_event+0x5c
#define ADDRESS_C_SIMULATION_PLAYER_TAUNT_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT_HOOK_RETURN 0x068CEE // c_simulation_player_taunt_request_event_definition::apply_game_event+0x6e
#define ADDRESS_GAME_ENGINE_RENDER_WATERMARKS                            0x1B0AB0 // game_engine_render_watermarks
#define ADDRESS_VSNPRINTF_S_NET_DEBUG_CALL                               0x55D8BF
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_1                0x33B1E0
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_1             0x33B1E5
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_2                0x33B2B0
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_2             0x33B2B5
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_3                0x33B33A
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_3             0x33B341
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_4                0x33B3BA
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_4             0x33B3C1
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_5                0x33B43A
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_5             0x33B441
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_6                0x33B4B7
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_6             0x33B4BE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_7                0x33B53A // TextureRenderBipedKillerSetUserData+0x6a
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_7             0x33B541 // TextureRenderBipedKillerSetUserData+0x71
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_8                0x33B5BA
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_8             0x33B5C1
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_9                0x33B63A
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_9             0x33B641
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_10               0x33B6B8
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_10            0x33B6BD
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_11               0x33B6D3
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_11            0x33B6D8
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CALL_12               0x33B6EE
#define ADDRESS_HF2P_SET_BIPED_TEXTURE_RENDER_DATA_CLEANUP_12            0x33B6F3
#define ADDRESS_STRING_ID_INITIALIZE                                     0x00110C // shell_initialize+0xfc
#define ADDRESS_STRING_ID_INITIALIZE_RETURN                              0x001111 // shell_initialize+0x101
#define ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK                           0x319D38
#define ADDRESS_LEAVE_SESSIONS_GRACEFULLY_HOOK_RETURN                    0x319D3D

// anvil\hooks\hooks_session.cpp
#define ADDRESS_MEMBERSHIP_UPDATE_MESSAGE                                0x4FFB090 // membership_update_message
#define ADDRESS_HANDLE_OUT_OF_BAND_MESSAGE                               0x025110 // c_network_message_handler::handle_out_of_band_message
#define ADDRESS_HANDLE_CHANNEL_MESSAGE                                   0x0252F0 // c_network_message_handler::handle_channel_message
#define ADDRESS_NETWORK_JOIN_PROCESS_JOINS_FROM_QUEUE                    0x02A580 // network_join_process_joins_from_queue
#define ADDRESS_SESSION_IDLE                                             0x021AB0 // c_network_session::idle
#define ADDRESS_NETWORK_SESSION_UPDATE_PEER_PROPERTIES                   0x02F650 // network_session_update_peer_properties
#define ADDRESS_NETWORK_LIFE_CYCLE_END_CALL                              0x3EEE1F // c_gui_location_manager::update+0xcf
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL            0x02AD9E // network_life_cycle_create_local_squad+0x9e
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION_CALL_2          0x02DC71 // network_session_interface_update+0x21
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL             0x021342 // c_network_session::create_host_session+0x392
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL_CALL_2           0x028051 // online_session_manager_update+0x1b1
#define ADDRESS_MANAGED_SESSION_DELETE_JUMP                              0x0284B8 // managed_session_delete+0x28
#define ADDRESS_GAME_ENGINE_SHOULD_SPAWN_PLAYER                          0x0FBBA0 // game_engine_should_spawn_player
#define ADDRESS_CAN_ACCEPT_PLAYER_JOIN_REQUEST_CALL                      0x0212DF // c_network_session::create_host_session+0x32f
#define ADDRESS_SESSION_DISCONNECT_CALL                                  0x021CB4 // c_network_session::set_disconnection_policy+0x14
#define ADDRESS_SESSION_DISCONNECT_CALL_2                                0x0225FE // c_network_session::host_connection_refused+0x5e
#define ADDRESS_SESSION_DISCONNECT_CALL_3                                0x02440E // network_initialize+0x3ae
#define ADDRESS_SESSION_DISCONNECT_CALL_4                                0x0256D4 // c_network_message_handler::handle_join_refuse+0x74
#define ADDRESS_SESSION_DISCONNECT_CALL_5                                0x025723 // c_network_message_handler::handle_leave_acknowledge+0x43
#define ADDRESS_SESSION_DISCONNECT_CALL_6                                0x02AC3C // network_life_cycle_end+0x1c
#define ADDRESS_SESSION_DISCONNECT_CALL_7                                0x02AD3D // network_life_cycle_create_local_squad+0x3d
#define ADDRESS_SESSION_DISCONNECT_CALL_8                                0x03E36F // c_network_session::change_local_state_peer_joining+0x13f
#define ADDRESS_SESSION_DISCONNECT_CALL_9                                0x03E8A4 // _dynamic_initializer_for__module_base___5+0xa4
#define ADDRESS_SESSION_DISCONNECT_CALL_10                               0x03EA40 // c_network_session::idle_peer_join_abort+0x40
#define ADDRESS_SESSION_DISCONNECT_CALL_11                               0x03EAA9 // c_network_session::idle_peer_leaving+0x49
#define ADDRESS_SESSION_DISCONNECT_CALL_12                               0x04B418 // c_network_session::handle_parameters_update+0x48
#define ADDRESS_SESSION_DISCONNECT_CALL_13                               0x04B522 // c_network_session::handle_session_disband+0x52
#define ADDRESS_SESSION_DISCONNECT_CALL_14                               0x04B538 // c_network_session::handle_session_disband+0x68
#define ADDRESS_SESSION_DISCONNECT_CALL_15                               0x04B5B2 // c_network_session::handle_session_boot+0x52
#define ADDRESS_SESSION_DISCONNECT_CALL_16                               0x04B659 // c_network_session::handle_host_decline+0x69
#define ADDRESS_SESSION_DISCONNECT_CALL_17                               0x04D14E // c_life_cycle_state_handler_pre_game__enter+0x3e
#define ADDRESS_SESSION_DISCONNECT_CALL_18                               0x04F42F // c_life_cycle_state_handler_none__enter+0x1f
#define ADDRESS_SESSION_DISCONNECT_CALL_19                               0x311749 // network_life_cycle_disconnect_all_sessions+0x29
#define ADDRESS_SESSION_DISCONNECT_CALL_20                               0x311767 // network_life_cycle_disconnect_all_sessions+0x47
#define ADDRESS_SESSION_DISCONNECT_CALL_21                               0x3AA106
#define ADDRESS_SESSION_DISCONNECT_CALL_22                               0x3AAF2C // hf2p_setup_session+0x1c
#define ADDRESS_SEND_ALL_PENDING_MESSAGES                                0x023480 // c_network_message_gateway::send_all_pending_messages

// anvil\hooks\simulation\hooks_damage_updates.cpp
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK1                               0x40D4CE // object_damage_update+0xa7e
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK1_RETURN                        0x40D553 // object_damage_update+0xb03
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK2                               0x40D526 // object_damage_update+0xad6
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK2_RETURN                        0x40D542 // object_damage_update+0xaf2
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK3                               0x40D5C1 // object_damage_update+0xb71
#define ADDRESS_OBJECT_DAMAGE_UPDATE_HOOK3_RETURN                        0x40D660 // object_damage_update+0xc10
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK1                               0x41268C // object_damage_shield+0x17c
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK1_RETURN                        0x412694 // object_damage_shield+0x184
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK2                               0x41287B // object_damage_shield+0x36b
#define ADDRESS_OBJECT_DAMAGE_SHIELD_HOOK2_RETURN                        0x412881 // object_damage_shield+0x371
#define ADDRESS_OBJECT_DAMAGE_BODY_HOOK1                                 0x411E3C // object_damage_body+0x16c
#define ADDRESS_OBJECT_DAMAGE_BODY_HOOK1_RETURN                          0x411E43 // object_damage_body+0x173
#define ADDRESS_OBJECT_DEPLETE_BODY_INTERNAL_HOOK1                       0x40D9D3 // object_deplete_body_internal+0xa3
#define ADDRESS_OBJECT_DEPLETE_BODY_INTERNAL_HOOK1_RETURN                0x40D9DA // object_deplete_body_internal+0xaa
#define ADDRESS_DAMAGE_SECTION_RESPONSE_FIRE_HOOK                        0x413D3F // damage_section_response_fire+0x6f
#define ADDRESS_DAMAGE_SECTION_RESPONSE_FIRE_HOOK_RETURN                 0x413D47 // damage_section_response_fire+0x77
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER                                  0x404320 // object_set_damage_owner
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK2                            0x113B0F // event_generate_accelerations+0x45f
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK2_RETURN                     0x113B15 // event_generate_accelerations+0x465
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK3                            0x20CF07 // havok_collision_damage_update+0x837
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK3_RETURN                     0x20CF0D // havok_collision_damage_update+0x83d
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK4                            0x40F00C // object_cause_damage+0x57c
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK4_RETURN                     0x40F012 // object_cause_damage+0x582
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK5                            0x4572A5 // motor_animation_exit_seat_immediate_internal+0x575
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK5_RETURN                     0x4572AB // motor_animation_exit_seat_immediate_internal+0x57b
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK6                            0x4BEA5D // vehicle_flip_submit+0x1bd
#define ADDRESS_OBJECT_SET_DAMAGE_OWNER_HOOK6_RETURN                     0x4BEA63 // vehicle_flip_submit+0x1c3

// anvil\hooks\simulation\hooks_object_creation.cpp
#define ADDRESS_PLAYER_SET_FACING_PLAYER_SPAWN_CALL                      0x0BB084 // player_spawn+0x3a4
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL                         0x0AEA03 // c_map_variant::create_object+0x313
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL_2                       0x172D86 // c_candy_spawner::spawn_object+0x3f6
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT_CALL_3                       0x4095BB // object_new_from_scenario_internal+0x13b
#define ADDRESS_EVENT_GENERATE_PART_HOOK                                 0x114A2F // event_generate_part+0x78f
#define ADDRESS_EVENT_GENERATE_PART_HOOK_RETURN                          0x114A6C // event_generate_part+0x7cc
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_END                     0x439A51
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES                         0x4371F0 // weapon_barrel_create_projectiles
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK0                   0x4373B4 // weapon_barrel_create_projectiles+0x1c4
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK0_RETURN            0x4373BB // weapon_barrel_create_projectiles+0x1cb
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK1                   0x4391CD // weapon_barrel_create_projectiles+0x1fdd
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK1_RETURN            0x4391D9 // weapon_barrel_create_projectiles+0x1fe9
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK2                   0x4394A2 // weapon_barrel_create_projectiles+0x22b2
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_HOOK2_RETURN            0x4394A8 // weapon_barrel_create_projectiles+0x22b8
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_STACK_CLEANUP           0x439A4B // weapon_barrel_create_projectiles+0x285b
#define ADDRESS_WEAPON_BARREL_CREATE_PROJECTILES_STACK_CLEANUP_RETURN    0x439A50 // weapon_barrel_create_projectiles+0x2860
#define ADDRESS_THROW_RELEASE_END                                        0x47D21C
#define ADDRESS_THROW_RELEASE                                            0x47CE00 // throw_release
#define ADDRESS_THROW_RELEASE_HOOK0                                      0x47D174 // throw_release+0x374
#define ADDRESS_THROW_RELEASE_HOOK0_RETURN                               0x47D179 // throw_release+0x379
#define ADDRESS_THROW_RELEASE_HOOK1                                      0x47D185 // throw_release+0x385
#define ADDRESS_THROW_RELEASE_HOOK1_RETURN                               0x47D18D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_STACK_CLEANUP                              0x47D211 // throw_release+0x411
#define ADDRESS_THROW_RELEASE_STACK_CLEANUP_RETURN                       0x47D21B // throw_release+0x41b
#define ADDRESS_THROW_RELEASE_HOOK3                                      0x47D17B // throw_release+0x37b
#define ADDRESS_THROW_RELEASE_HOOK3_RETURN                               0x47D18D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_NOP                                        0x47D180 // throw_release+0x380
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK                                  0x45113A // equipment_activate+0xd6a
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK_RETURN                           0x451144 // equipment_activate+0xd74
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK                              0x484337 // item_in_unit_inventory+0x287
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK_RETURN                       0x48433D // item_in_unit_inventory+0x28d
#define ADDRESS_UNIT_DROP_PLASMA_ON_DEATH_HOOK                           0x426F19 // unit_drop_plasma_on_death+0x139
#define ADDRESS_UNIT_DROP_PLASMA_ON_DEATH_HOOK_RETURN                    0x426F20 // unit_drop_plasma_on_death+0x140
#define ADDRESS_CREATE_FLAG_AT_POSITION_HOOK                             0x22A596 // create_flag_at_position+0x76
#define ADDRESS_CREATE_FLAG_AT_POSITION_HOOK_RETURN                      0x22A59B // create_flag_at_position+0x7b

// anvil\hooks\simulation\hooks_object_deletion.cpp
#define ADDRESS_OBJECT_SCRIPTING_CLEAR_ALL_FUNCTION_VARIABLES_CALL       0x3FE1BE // object_delete+0xfe
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK2                             0x484186 // item_in_unit_inventory+0xd6
#define ADDRESS_ITEM_IN_UNIT_INVENTORY_HOOK2_RETURN                      0x48418D // item_in_unit_inventory+0xdd

// anvil\hooks\simulation\hooks_object_updates.cpp
#define ADDRESS_OBJECT_UPDATE_HOOK                                       0x404907 // object_update+0x197
#define ADDRESS_OBJECT_UPDATE_HOOK_RETURN                                0x40490E // object_update+0x19e
#define ADDRESS_PLAYER_SET_FACING                                        0x0B6300 // player_set_facing
#define ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK                         0x0ADB2B // c_map_variant::remove_object+0x8b
#define ADDRESS_C_MAP_VARIANT_REMOVE_OBJECT_HOOK_RETURN                  0x0ADB45 // c_map_variant::remove_object+0xa5
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1                             0x0ABA7E // c_map_variant::unknown4+0x21e
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK1_RETURN                      0x0ABA86 // c_map_variant::unknown4+0x226
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2                             0x0ABAAA // c_map_variant::unknown4+0x24a
#define ADDRESS_C_MAP_VARIANT_UNKNOWN4_HOOK2_RETURN                      0x0ABAB1 // c_map_variant::unknown4+0x251
#define ADDRESS_PLAYER_SET_UNIT_INDEX_CALL                               0x0B5F8E // player_set_unit_index+0x7e
#define ADDRESS_PLAYER_SET_UNIT_INDEX_CALL_2                             0x0B60E4 // player_set_unit_index+0x1d4
#define ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2                              0x0B6289 // player_set_unit_index+0x379
#define ADDRESS_PLAYER_SET_UNIT_INDEX_HOOK2_RETURN                       0x0B628E // player_set_unit_index+0x37e
#define ADDRESS_PLAYER_INCREMENT_CONTROL_CONTEXT                         0x0B9A90 // player_increment_control_context
#define ADDRESS_UNIT_DIED_HOOK                                           0x421469 // unit_died+0x1f9
#define ADDRESS_UNIT_DIED_HOOK_RETURN                                    0x421471 // unit_died+0x201
#define ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK                          0x47D42F // grenade_throw_move_to_hand+0x18f
#define ADDRESS_GRENADE_THROW_MOVE_TO_HAND_HOOK_RETURN                   0x47D435 // grenade_throw_move_to_hand+0x195
#define ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK                       0x4243D8 // unit_add_grenade_to_inventory+0xb8
#define ADDRESS_UNIT_ADD_GRENADE_TO_INVENTORY_HOOK_RETURN                0x4243DF // unit_add_grenade_to_inventory+0xbf
#define ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK                     0x424586 // unit_add_equipment_to_inventory+0x116
#define ADDRESS_UNIT_ADD_EQUIPMENT_TO_INVENTORY_HOOK_RETURN              0x42458C // unit_add_equipment_to_inventory+0x11c
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK                                 0x41854A // unit_update_control+0xaa
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_RETURN                          0x418550 // unit_update_control+0xb0
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2                               0x41868D // unit_update_control+0x1ed
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_2_RETURN                        0x418693 // unit_update_control+0x1f3
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3                               0x418A73 // unit_update_control+0x5d3
#define ADDRESS_UNIT_UPDATE_CONTROL_HOOK_3_RETURN                        0x418A79 // unit_update_control+0x5d9
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_END                             0x0FBAFD
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT                                 0x0FB6E0 // unit_add_initial_loadout
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0                           0x0FB6F1 // unit_add_initial_loadout+0x11
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK0_RETURN                    0x0FB6F6 // unit_add_initial_loadout+0x16
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1                           0x0FBA34 // unit_add_initial_loadout+0x354
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK1_RETURN                    0x0FBA3A // unit_add_initial_loadout+0x35a
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2                           0x0FBAD9 // unit_add_initial_loadout+0x3f9
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK2_RETURN                    0x0FBAE0 // unit_add_initial_loadout+0x400
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP                   0x0FBAE0 // unit_add_initial_loadout+0x400
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_RETURN            0x0FBAE6 // unit_add_initial_loadout+0x406
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2                 0x0FBAF2 // unit_add_initial_loadout+0x412
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_STACK_CLEANUP_2_RETURN          0x0FBAFC // unit_add_initial_loadout+0x41c
#define ADDRESS_PROJECTILE_ATTACH_HOOK                                   0x467F11 // projectile_attach+0x2c1
#define ADDRESS_PROJECTILE_ATTACH_HOOK_RETURN                            0x467F19 // projectile_attach+0x2c9
#define ADDRESS_BIPED_UPDATE_MELEE_TURNING                               0x440E30 // biped_update_melee_turning
#define ADDRESS_UNIT_CONTROL_CALL                                        0x02BB82 // simulation_apply_before_game+0x92
#define ADDRESS_UNIT_CONTROL_CALL_2                                      0x0BD4E7 // player_submit_control+0x1c7
#define ADDRESS_UNIT_CONTROL_CALL_3                                      0x0BD573
#define ADDRESS_UNIT_CONTROL_CALL_4                                      0x181410 // recorded_animations_update+0x120
#define ADDRESS_UNIT_CONTROL_CALL_5                                      0x69BB96
#define ADDRESS_UNIT_CONTROL_CALL_6                                      0x69DA76
#define ADDRESS_UNIT_SET_AIMING_VECTORS                                  0x42A490 // unit_set_aiming_vectors
#define ADDRESS_C_GAME_ENGINE_APPLY_PLAYER_UPDATE_NOP                    0x1C9AF5 // c_game_engine::apply_player_update+0x635
#define ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH                      0x0B911B // player_teleport_on_bsp_switch+0x14b
#define ADDRESS_PLAYER_TELEPORT_ON_BSP_SWITCH_PATCH_2                    0x0B911E // player_teleport_on_bsp_switch+0x14e
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1                            0x059EFF // c_simulation_unit_entity_definition::apply_object_update+0x32f
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK1_RETURN                     0x059F48 // c_simulation_unit_entity_definition::apply_object_update+0x378
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2                            0x061093 // c_simulation_weapon_fire_event_definition::apply_game_event+0x533
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK2_RETURN                     0x0610CF // c_simulation_weapon_fire_event_definition::apply_game_event+0x56f
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3                            0x4A0FCB // c_vehicle_auto_turret::control+0xdb
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK3_RETURN                     0x4A1010 // c_vehicle_auto_turret::control+0x120
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4                            0x0F87D2 // attach_biped_to_player+0x202
#define ADDRESS_UNIT_SET_AIMING_VECTORS_HOOK4_RETURN                     0x0F87F4 // attach_biped_to_player+0x224
#define ADDRESS_ATTACH_BIPED_TO_PLAYER_NOP                               0x0F87BF // attach_biped_to_player+0x1ef
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK2                                 0x4514E2 // equipment_activate+0x1112
#define ADDRESS_EQUIPMENT_ACTIVATE_HOOK2_RETURN                          0x4514E8 // equipment_activate+0x1118
#define ADDRESS_UNIT_UPDATE_ENERGY_HOOK                                  0x41B600 // unit_update+0x110
#define ADDRESS_UNIT_UPDATE_ENERGY_HOOK_RETURN                           0x41B606 // unit_update+0x116
#define ADDRESS_UNIT_SET_HOLOGRAM_HOOK                                   0x42C56E // unit_set_hologram+0x17e
#define ADDRESS_UNIT_SET_HOLOGRAM_HOOK_RETURN                            0x42C578 // unit_set_hologram+0x188
#define ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK                       0x412E41 // object_apply_damage_aftermath+0x341
#define ADDRESS_OBJECT_APPLY_DAMAGE_AFTERMATH_HOOK_RETURN                0x412E4F // object_apply_damage_aftermath+0x34f
#define ADDRESS_UNIT_UPDATE_DAMAGE_HOOK                                  0x41AE09 // unit_update_damage+0xd9
#define ADDRESS_UNIT_UPDATE_DAMAGE_HOOK_RETURN                           0x41AE10 // unit_update_damage+0xe0
#define ADDRESS_UNIT_RESPOND_TO_EMP_HOOK                                 0x417B9D // unit_respond_to_emp+0xfd
#define ADDRESS_UNIT_RESPOND_TO_EMP_HOOK_RETURN                          0x417BA4 // unit_respond_to_emp+0x104
#define ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK                       0x417CE5 // unit_delete+0x95
#define ADDRESS_UNIT_DELETE_CURRENT_EQUIPMENT_HOOK_RETURN                0x417CEF // unit_delete+0x9f
#define ADDRESS_UNIT_DELETE_EQUIPMENT                                    0x424660 // unit_delete_equipment
#define ADDRESS_UNIT_PLACE_HOOK                                          0x4171D5 // unit_place+0x115
#define ADDRESS_UNIT_PLACE_HOOK_RETURN                                   0x41725C // unit_place+0x19c
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3                           0x0FB7F4 // unit_add_initial_loadout+0x114
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK3_RETURN                    0x0FB864 // unit_add_initial_loadout+0x184
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4                           0x0FB864 // unit_add_initial_loadout+0x184
#define ADDRESS_UNIT_ADD_INITIAL_LOADOUT_HOOK4_RETURN                    0x0FB869 // unit_add_initial_loadout+0x189
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK 0x05A23A // c_simulation_unit_entity_definition::apply_object_update+0x66a
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN 0x05A2A4 // c_simulation_unit_entity_definition::apply_object_update+0x6d4
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DING                              0x42AAE0 // unit_active_camouflage_ding
#define ADDRESS_THROW_RELEASE_HOOK2                                      0x47D18D // throw_release+0x38d
#define ADDRESS_THROW_RELEASE_HOOK2_RETURN                               0x47D20F // throw_release+0x40f
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_DISABLE                           0x42AA80 // unit_active_camouflage_disable
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_LEVEL                         0x42AA20 // unit_active_camouflage_set_level
#define ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK                      0x48582F // unit_scripting_set_active_camo+0xf
#define ADDRESS_UNIT_SCRIPTING_SET_ACTIVE_CAMO_HOOK_RETURN               0x485884 // unit_scripting_set_active_camo+0x64
#define ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK                          0x0BFDC9 // player_update_invisibility+0xb9
#define ADDRESS_PLAYER_UPDATE_INVISIBILITY_HOOK_RETURN                   0x0BFE41 // player_update_invisibility+0x131
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2 0x05A137 // c_simulation_unit_entity_definition::apply_object_update+0x567
#define ADDRESS_C_SIMULATION_UNIT_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK2_RETURN 0x05A1B2 // c_simulation_unit_entity_definition::apply_object_update+0x5e2
#define ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK 0x07280D // c_simulation_vehicle_entity_definition::apply_object_update+0x1ed
#define ADDRESS_C_SIMULATION_VEHICLE_ENTITY_DEFINITION_APPLY_OBJECT_UPDATE_HOOK_RETURN 0x072888 // c_simulation_vehicle_entity_definition::apply_object_update+0x268
#define ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK                               0x69BD37 // actor_set_active_camo+0x77
#define ADDRESS_ACTOR_SET_ACTIVE_CAMO_HOOK_RETURN                        0x69BD85 // actor_set_active_camo+0xc5
#define ADDRESS_UNIT_ACTIVE_CAMOUFLAGE_SET_MAXIMUM                       0x42A9D0 // unit_active_camouflage_set_maximum
#define ADDRESS_BIPED_NEW_HOOK                                           0x43CEDD // biped_new+0x4d
#define ADDRESS_BIPED_NEW_HOOK_RETURN                                    0x43CF07 // biped_new+0x77
#define ADDRESS_VEHICLE_NEW_HOOK                                         0x4524B7 // vehicle_new+0xd7
#define ADDRESS_VEHICLE_NEW_HOOK_RETURN                                  0x4524E9 // vehicle_new+0x109
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_END                        0x41B1D7
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE                            0x41AF50 // unit_update_active_camouflage
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0                      0x41AF6C // unit_update_active_camouflage+0x1c
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK0_RETURN               0x41AF73 // unit_update_active_camouflage+0x23
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1                      0x41B1B9 // unit_update_active_camouflage+0x269
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_HOOK1_RETURN               0x41B1D0 // unit_update_active_camouflage+0x280
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP              0x41B17F // unit_update_active_camouflage+0x22f
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_RETURN       0x41B185 // unit_update_active_camouflage+0x235
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2            0x41B193 // unit_update_active_camouflage+0x243
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_2_RETURN     0x41B199 // unit_update_active_camouflage+0x249
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3            0x41B1D0 // unit_update_active_camouflage+0x280
#define ADDRESS_UNIT_UPDATE_ACTIVE_CAMOUFLAGE_STACK_CLEANUP_3_RETURN     0x41B1D6 // unit_update_active_camouflage+0x286
#define ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK                    0x44DF23 // unit_action_assassinate_finished+0x63
#define ADDRESS_UNIT_ACTION_ASSASSINATE_FINISHED_HOOK_RETURN             0x44DF28 // unit_action_assassinate_finished+0x68
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1                     0x44D7EF // unit_action_assassinate_submit+0x21f
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK1_RETURN              0x44D7F6 // unit_action_assassinate_submit+0x226
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2                     0x44D852 // unit_action_assassinate_submit+0x282
#define ADDRESS_UNIT_ACTION_ASSASSINATE_SUBMIT_HOOK2_RETURN              0x44D859 // unit_action_assassinate_submit+0x289
#define ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK                 0x44DFCD // unit_action_assassinate_interrupted+0x7d
#define ADDRESS_UNIT_ACTION_ASSASSINATE_INTERRUPTED_HOOK_RETURN          0x44DFD2 // unit_action_assassinate_interrupted+0x82
#define ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK                      0x45666E // motor_task_enter_seat_internal+0x4be
#define ADDRESS_MOTOR_TASK_ENTER_SEAT_INTERNAL_HOOK_RETURN               0x456675 // motor_task_enter_seat_internal+0x4c5
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK        0x457053 // motor_animation_exit_seat_immediate_internal+0x323
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_IMMEDIATE_INTERNAL_HOOK_RETURN 0x457058 // motor_animation_exit_seat_immediate_internal+0x328
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1                      0x45D9F9 // device_group_set_actual_value+0xc9
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK1_RETURN               0x45DA2E // device_group_set_actual_value+0xfe
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2                      0x45DA53 // device_group_set_actual_value+0x123
#define ADDRESS_DEVICE_GROUP_SET_ACTUAL_VALUE_HOOK2_RETURN               0x45DA8C // device_group_set_actual_value+0x15c
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1                     0x45D6E1 // device_group_set_desired_value+0x111
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK1_RETURN              0x45D6F4 // device_group_set_desired_value+0x124
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2                     0x45D745 // device_group_set_desired_value+0x175
#define ADDRESS_DEVICE_GROUP_SET_DESIRED_VALUE_HOOK2_RETURN              0x45D74B // device_group_set_desired_value+0x17b
#define ADDRESS_DEVICE_SET_POWER_HOOK                                    0x45D5BE // device_set_power+0x3e
#define ADDRESS_DEVICE_SET_POWER_HOOK_RETURN                             0x45D5C3 // device_set_power+0x43
#define ADDRESS_MACHINE_UPDATE_HOOK                                      0x48D398 // machine_update+0x268
#define ADDRESS_MACHINE_UPDATE_HOOK_RETURN                               0x48D39F // machine_update+0x26f
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_END              0x4A0EE4
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET                  0x4A0BA0 // c_vehicle_auto_turret::track_auto_target
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0            0x4A0BB6 // c_vehicle_auto_turret::track_auto_target+0x16
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK0_RETURN     0x4A0BBC // c_vehicle_auto_turret::track_auto_target+0x1c
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1            0x4A0E3E // c_vehicle_auto_turret::track_auto_target+0x29e
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK1_RETURN     0x4A0E44 // c_vehicle_auto_turret::track_auto_target+0x2a4
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2            0x4A0EBF // c_vehicle_auto_turret::track_auto_target+0x31f
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_RETURN     0x4A0EC6 // c_vehicle_auto_turret::track_auto_target+0x326
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2          0x4A0ED4 // c_vehicle_auto_turret::track_auto_target+0x334
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_HOOK2_2_RETURN   0x4A0EDB // c_vehicle_auto_turret::track_auto_target+0x33b
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP    0x4A0EC6 // c_vehicle_auto_turret::track_auto_target+0x326
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_RETURN 0x4A0ECC // c_vehicle_auto_turret::track_auto_target+0x32c
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2  0x4A0EDB // c_vehicle_auto_turret::track_auto_target+0x33b
#define ADDRESS_C_VEHICLE_AUTO_TURRET_TRACK_AUTO_TARGET_STACK_CLEANUP_2_RETURN 0x4A0EE1 // c_vehicle_auto_turret::track_auto_target+0x341
#define ADDRESS_OBJECT_MOVE_POSITION_HOOK                                0x3FC276 // object_move_position+0xa6
#define ADDRESS_OBJECT_MOVE_POSITION_HOOK_RETURN                         0x3FC27C // object_move_position+0xac
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK                           0x0C95EE // game_engine_update_player+0xae
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK_RETURN                    0x0C9611 // game_engine_update_player+0xd1
#define ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK                         0x1C8681 // c_game_engine::player_update+0xe1
#define ADDRESS_C_GAME_ENGINE_PLAYER_UPDATE_HOOK_RETURN                  0x1C8687 // c_game_engine::player_update+0xe7
#define ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK                      0x117072 // game_engine_teleporters_update+0x22
#define ADDRESS_GAME_ENGINE_TELEPORTERS_UPDATE_HOOK_RETURN               0x117079 // game_engine_teleporters_update+0x29
#define ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK                  0x0C6A3E // game_engine_initialize_for_new_map+0x33e
#define ADDRESS_GAME_ENGINE_INITIALIZE_FOR_NEW_MAP_HOOK_RETURN           0x0C6A57 // game_engine_initialize_for_new_map+0x357
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1              0x2282D6 // c_ctf_engine::initialize_for_new_round+0x66
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK1_RETURN       0x2282EF // c_ctf_engine::initialize_for_new_round+0x7f
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2              0x228339 // c_ctf_engine::initialize_for_new_round+0xc9
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK2_RETURN       0x228352 // c_ctf_engine::initialize_for_new_round+0xe2
#define ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK                      0x22B7E3 // ?initialize@?$c_area_set@Vc_area@@$02@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_C_AREA_3_INITIALIZE_HOOK_RETURN               0x22B7FC // ?initialize@?$c_area_set@Vc_area@@$02@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK                     0x22F172 // ?initialize@?$c_area_set@Vc_area@@$09@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x42
#define ADDRESS_C_AREA_SET_C_AREA_10_INITIALIZE_HOOK_RETURN              0x22F18B // ?initialize@?$c_area_set@Vc_area@@$09@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5b
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1                   0x22F1B8 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK1_RETURN            0x22F1D1 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2                   0x22F26D // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_AREA_10_SELECT_AREA_HOOK2_RETURN            0x22F286 // ?select_area@?$c_area_set@Vc_area@@$09@@QAAXW4e_area_cycle_options@@@Z+0xf6
#define ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK                     0x22D314 // ?initialize@?$c_area_set@Vc_area@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x44
#define ADDRESS_C_AREA_SET_C_AREA_12_INITIALIZE_HOOK_RETURN              0x22D32D // ?initialize@?$c_area_set@Vc_area@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5d
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1                   0x22D368 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK1_RETURN            0x22D381 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2                   0x22D41D // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_AREA_12_SELECT_AREA_HOOK2_RETURN            0x22D436 // ?select_area@?$c_area_set@Vc_area@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xf6
#define ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK            0x2339D3 // ?initialize@?$c_area_set@Us_territory_data@@$07@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_S_TERRITORY_DATA_8_INITIALIZE_HOOK_RETURN     0x2339EC // ?initialize@?$c_area_set@Us_territory_data@@$07@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK         0x230C43 // ?initialize@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x43
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_INITIALIZE_HOOK_RETURN  0x230C5C // ?initialize@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXJJW4e_multiplayer_team_designator@@@Z+0x5c
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1       0x230C98 // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x28
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK1_RETURN 0x230CB1 // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0x41
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2       0x230D4D // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xdd
#define ADDRESS_C_AREA_SET_C_DESTINATION_ZONE_12_SELECT_AREA_HOOK2_RETURN 0x230D6A // ?select_area@?$c_area_set@Vc_destination_zone@@$0M@@@QAAXW4e_area_cycle_options@@@Z+0xfa
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK             0x0FAAEB // game_engine_multiplayer_weapon_register+0x6b
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_REGISTER_HOOK_RETURN      0x0FAAF2 // game_engine_multiplayer_weapon_register+0x72
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK           0x0FABFB // game_engine_multiplayer_weapon_deregister+0x9b
#define ADDRESS_GAME_ENGINE_MULTIPLAYER_WEAPON_DEREGISTER_HOOK_RETURN    0x0FAC02 // game_engine_multiplayer_weapon_deregister+0xa2
#define ADDRESS_CREATE_FLAG_HOOK                                         0x22A6F8 // create_flag+0x158
#define ADDRESS_CREATE_FLAG_HOOK_RETURN                                  0x22A708 // create_flag+0x168
#define ADDRESS_FLAG_RESET_PATCH                                         0x22A93B // flag_reset+0x3b
#define ADDRESS_FLAG_RESET_PATCH_2                                       0x22A973 // flag_reset+0x73
#define ADDRESS_FLAG_RESET_HOOK                                          0x22AA74 // flag_reset+0x174
#define ADDRESS_FLAG_RESET_HOOK_RETURN                                   0x22AA79 // flag_reset+0x179

// anvil\hooks\simulation\hooks_physics_updates.cpp
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1                       0x3FC038 // object_set_position_internal+0xa8
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK1_RETURN                0x3FC03E // object_set_position_internal+0xae
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2                       0x3FC060 // object_set_position_internal+0xd0
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL_HOOK2_RETURN                0x3FC066 // object_set_position_internal+0xd6
#define ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK                      0x404A51 // object_move_respond_to_physics+0x121
#define ADDRESS_OBJECT_MOVE_RESPOND_TO_PHYSICS_HOOK_RETURN               0x404ADD // object_move_respond_to_physics+0x1ad
#define ADDRESS_OBJECT_SET_VELOCITIES_INTERNAL                           0x3FC500 // object_set_velocities_internal
#define ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK                           0x3FC7F8 // object_apply_acceleration+0x98
#define ADDRESS_OBJECT_APPLY_ACCELERATION_HOOK_RETURN                    0x3FC833 // object_apply_acceleration+0xd3
#define ADDRESS_OBJECT_SET_AT_REST                                       0x4011F0 // object_set_at_rest
#define ADDRESS_OBJECT_SET_AT_REST_CALL                                  0x06BBB8 // c_simulation_object_entity_definition::object_apply_update+0x3b8
#define ADDRESS_OBJECT_SET_AT_REST_CALL_2                                0x0773D6 // c_simulation_generic_entity_definition::apply_object_update+0x276
#define ADDRESS_OBJECT_SET_AT_REST_CALL_3                                0x0C82A1 // garbage_collect_multiplayer+0x381
#define ADDRESS_OBJECT_SET_AT_REST_CALL_4                                0x172CB3 // c_candy_spawner::spawn_object+0x323
#define ADDRESS_OBJECT_SET_AT_REST_CALL_5                                0x1282B8 // c_havok_component::wake_all_bodies_in_phantoms+0x98
#define ADDRESS_OBJECT_SET_AT_REST_CALL_6                                0x3FBF35 // object_reset+0x85
#define ADDRESS_OBJECT_SET_AT_REST_CALL_7                                0x419397 // unit_fix_position+0x307
#define ADDRESS_OBJECT_SET_AT_REST_CALL_8                                0x413DD3 // damage_section_response_fire+0x103
#define ADDRESS_OBJECT_SET_AT_REST_CALL_9                                0x415DC3 // object_damage_constraints+0xc3
#define ADDRESS_OBJECT_SET_AT_REST_CALL_10                               0x43DAF6 // biped_update_without_parent+0x256
#define ADDRESS_OBJECT_SET_AT_REST_CALL_11                               0x4632B7 // projectile_accelerate+0x3b7
#define ADDRESS_OBJECT_SET_AT_REST_CALL_12                               0x456EB5 // motor_animation_exit_seat_immediate_internal+0x185
#define ADDRESS_OBJECT_SET_AT_REST_CALL_13                               0x1EF12D // object_wake_physics_evaluate+0x4d
#define ADDRESS_OBJECT_SET_AT_REST_CALL_14                               0x46D33E // object_early_mover_delete+0x27e
#define ADDRESS_OBJECT_SET_AT_REST_CALL_15                               0x4848B8 // item_multiplayer_at_rest_state_initialize+0x148
#define ADDRESS_OBJECT_SET_AT_REST_CALL_16                               0x4980E3 // biped_stun_submit+0x1a3
#define ADDRESS_OBJECT_SET_AT_REST_CALL_17                               0x4A9126 // c_vehicle_type_mantis::update_physics+0xf6
#define ADDRESS_OBJECT_SET_AT_REST_CALL_18                               0x4A4300 // biped_dead_force_airborne+0xe0
#define ADDRESS_OBJECT_SET_AT_REST_CALL_19                               0x4A1F00 // biped_exit_relaxation+0x200
#define ADDRESS_OBJECT_SET_AT_REST_CALL_20                               0x4A2396 // biped_start_relaxation+0x136
#define ADDRESS_OBJECT_SET_AT_REST_HOOK2                                 0x0770F5 // c_simulation_generic_entity_definition::create_object+0xd5
#define ADDRESS_OBJECT_SET_AT_REST_HOOK2_RETURN                          0x077142 // c_simulation_generic_entity_definition::create_object+0x122
#define ADDRESS_OBJECT_SET_AT_REST_HOOK3                                 0x07255B // c_simulation_vehicle_entity_definition::create_object+0xab
#define ADDRESS_OBJECT_SET_AT_REST_HOOK3_RETURN                          0x0725A8 // c_simulation_vehicle_entity_definition::create_object+0xf8
#define ADDRESS_OBJECT_SET_AT_REST_HOOK4                                 0x400A97 // object_attach_to_node_immediate+0x3e7
#define ADDRESS_OBJECT_SET_AT_REST_HOOK4_RETURN                          0x400AD1 // object_attach_to_node_immediate+0x421
#define ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP                      0x400A89 // object_attach_to_node_immediate+0x3d9
#define ADDRESS_OBJECT_ATTACH_TO_NODE_IMMEDIATE_NOP_2                    0x400A91 // object_attach_to_node_immediate+0x3e1
#define ADDRESS_OBJECT_SET_AT_REST_HOOK5                                 0x467DB1 // projectile_attach+0x161
#define ADDRESS_OBJECT_SET_AT_REST_HOOK5_RETURN                          0x467DE7 // projectile_attach+0x197
#define ADDRESS_PROJECTILE_ATTACH_NOP                                    0x467D93 // projectile_attach+0x143
#define ADDRESS_PROJECTILE_ATTACH_NOP_2                                  0x467DA4 // projectile_attach+0x154
#define ADDRESS_OBJECT_SET_AT_REST_HOOK6                                 0x464E82 // projectile_collision+0x1552
#define ADDRESS_OBJECT_SET_AT_REST_HOOK6_RETURN                          0x464EC7 // projectile_collision+0x1597
#define ADDRESS_OBJECT_SET_AT_REST_HOOK7                                 0x4623A6 // projectile_initial_update+0x426
#define ADDRESS_OBJECT_SET_AT_REST_HOOK7_RETURN                          0x462431 // projectile_initial_update+0x4b1
#define ADDRESS_PROJECTILE_INITIAL_UPDATE_NOP                            0x462398 // projectile_initial_update+0x418
#define ADDRESS_OBJECT_SET_AT_REST_HOOK8                                 0x46D200 // object_early_mover_delete+0x140
#define ADDRESS_OBJECT_SET_AT_REST_HOOK8_RETURN                          0x46D281 // object_early_mover_delete+0x1c1
#define ADDRESS_OBJECT_SET_AT_REST_HOOK9                                 0x46D2D0 // object_early_mover_delete+0x210
#define ADDRESS_OBJECT_SET_AT_REST_HOOK9_RETURN                          0x46D343 // object_early_mover_delete+0x283
#define ADDRESS_OBJECT_SET_AT_REST_HOOK10                                0x6D50C7 // swarm_accelerate+0xd7
#define ADDRESS_OBJECT_SET_AT_REST_HOOK10_RETURN                         0x6D50EF // swarm_accelerate+0xff
#define ADDRESS_SWARM_ACCELERATE_NOP                                     0x6D506C // swarm_accelerate+0x7c
#define ADDRESS_OBJECT_SET_AT_REST_CALL_21                               0x48FB9A // scenery_new+0xaa
#define ADDRESS_OBJECT_SET_AT_REST_HOOK12                                0x4BEB84 // vehicle_program_activate+0x54
#define ADDRESS_OBJECT_SET_AT_REST_HOOK12_RETURN                         0x4BEBB7 // vehicle_program_activate+0x87
#define ADDRESS_OBJECT_SET_AT_REST_HOOK13                                0x4BFF31 // vehicle_program_update+0x141
#define ADDRESS_OBJECT_SET_AT_REST_HOOK13_RETURN                         0x4BFF76 // vehicle_program_update+0x186
#define ADDRESS_OBJECT_SET_AT_REST_CALL_22                               0x4C7A66 // unit_custom_animation_play_animation_submit+0x2c6

// anvil\hooks\simulation\hooks_player_updates.cpp
#define ADDRESS_PLAYER_SPAWN_HOOK1                                       0x0BB093 // player_spawn+0x3b3
#define ADDRESS_PLAYER_SPAWN_HOOK1_RETURN                                0x0BB098 // player_spawn+0x3b8
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2                          0x0B80BA // players_update_after_game+0x37a
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK2_RETURN                   0x0B80C8 // players_update_after_game+0x388
#define ADDRESS_PLAYER_SPAWN_HOOK2                                       0x0BB435 // player_spawn+0x755
#define ADDRESS_PLAYER_SPAWN_HOOK2_RETURN                                0x0BB43B // player_spawn+0x75b
#define ADDRESS_GAME_ENGINE_PLAYER_SET_SPAWN_TIMER                       0x0C7700 // game_engine_player_set_spawn_timer
#define ADDRESS_PLAYER_SPAWN_HOOK3                                       0x0BB459 // player_spawn+0x779
#define ADDRESS_PLAYER_SPAWN_HOOK3_RETURN                                0x0BB460 // player_spawn+0x780
#define ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK                0x0FA088 // game_engine_setup_player_for_respawn+0x228
#define ADDRESS_GAME_ENGINE_SETUP_PLAYER_FOR_RESPAWN_HOOK_RETURN         0x0FA08F // game_engine_setup_player_for_respawn+0x22f
#define ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK           0x0FC021 // objective_game_player_forced_base_respawn+0xc1
#define ADDRESS_OBJECTIVE_GAME_PLAYER_FORCED_BASE_RESPAWN_HOOK_RETURN    0x0FC028 // objective_game_player_forced_base_respawn+0xc8
#define ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK  0x1C9FA4 // player_killed_player_perform_respawn_on_kill_check+0x104
#define ADDRESS_PLAYER_KILLED_PLAYER_PERFORM_RESPAWN_ON_KILL_CHECK_HOOK_RETURN 0x1C9FB0 // player_killed_player_perform_respawn_on_kill_check+0x110
#define ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK             0x0FBB7E // game_engine_reset_player_respawn_timers+0x7e
#define ADDRESS_GAME_ENGINE_RESET_PLAYER_RESPAWN_TIMERS_HOOK_RETURN      0x0FBB86 // game_engine_reset_player_respawn_timers+0x86
#define ADDRESS_C_SIMULATION_PLAYER_RESPAWN_REQUEST_EVENT_DEFINITION_APPLY_GAME_EVENT 0x068B40 // c_simulation_player_respawn_request_event_definition::apply_game_event
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1                              0x0BAF22 // player_spawn+0x242
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK1_RETURN                       0x0BAF29 // player_spawn+0x249
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2                              0x0E05A8 // equipment_add+0x78
#define ADDRESS_PLAYER_UPDATE_LOADOUT_HOOK2_RETURN                       0x0E05AF // equipment_add+0x7f
#define ADDRESS_PLAYER_UPDATE_LOADOUT                                    0x0E0660 // player_update_loadout
#define ADDRESS_PLAYER_RESET_HOOK                                        0x0B5259 // player_reset+0x639
#define ADDRESS_PLAYER_RESET_HOOK_RETURN                                 0x0B532D // player_reset+0x70d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK            0x0C9ADD // game_engine_update+0x17d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_NETDEBUG_STATE_HOOK_RETURN     0x0C9AE3 // game_engine_update+0x183
#define ADDRESS_SIMULATION_QUEUE_PLAYER_EVENT_APPLY_SET_ACTIVATION       0x054A70 // simulation_queue_player_event_apply_set_activation
#define ADDRESS_GAME_ENGINE_BOOT_PLAYER_SAFE                             0x054AE0 // game_engine_boot_player_safe
#define ADDRESS_GAME_ENGINE_BOOT_PLAYER                                  0x0CCB20 // game_engine_boot_player
#define ADDRESS_PLAYER_NOTIFY_VEHICLE_EJECTION_FINISHED                  0x0BFB90 // player_notify_vehicle_ejection_finished
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3                          0x0B80FE // players_update_after_game+0x3be
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK3_RETURN                   0x0B8112 // players_update_after_game+0x3d2
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1                          0x0B80A6 // players_update_after_game+0x366
#define ADDRESS_PLAYERS_UPDATE_AFTER_GAME_HOOK1_RETURN                   0x0B80AD // players_update_after_game+0x36d
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1                          0x0F8FCB // game_engine_player_killed+0xdb
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK1_RETURN                   0x0F8FD2 // game_engine_player_killed+0xe2
#define ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1                0x1B0248 // c_game_statborg::record_player_death+0xe8
#define ADDRESS_C_GAME_STATBORG_RECORD_PLAYER_DEATH_HOOK1_RETURN         0x1B024E // c_game_statborg::record_player_death+0xee
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2                          0x0C96E5 // game_engine_update_player+0x1a5
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_HOOK2_RETURN                   0x0C96EF // game_engine_update_player+0x1af
#define ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK                     0x0FA0B9 // game_engine_player_fired_weapon+0x9
#define ADDRESS_GAME_ENGINE_PLAYER_FIRED_WEAPON_HOOK_RETURN              0x0FA1B4 // game_engine_player_fired_weapon+0x104
#define ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK                   0x0F8E00 // game_engine_player_damaged_player+0xe0
#define ADDRESS_GAME_ENGINE_PLAYER_DAMAGED_PLAYER_HOOK_RETURN            0x0F8EEB // game_engine_player_damaged_player+0x1cb
#define ADDRESS_UPDATE_PLAYER_NAVPOINT_DATA                              0x0CC9D0 // update_player_navpoint_data
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3         0x0C9C41 // game_engine_update_after_game_update_state+0x51
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK3_RETURN  0x0C9C7F // game_engine_update_after_game_update_state+0x8f
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_PATCH         0x0C9C7F // game_engine_update_after_game_update_state+0x8f
#define ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK                         0x0FA663 // game_engine_player_rejoined+0x83
#define ADDRESS_GAME_ENGINE_PLAYER_REJOINED_HOOK_RETURN                  0x0FA66A // game_engine_player_rejoined+0x8a
#define ADDRESS_GAME_ENGINE_APPLY_APPEARANCE_TRAITS                      0x11E050 // game_engine_apply_appearance_traits
#define ADDRESS_GAME_ENGINE_APPLY_MOVEMENT_TRAITS                        0x11DF20 // game_engine_apply_movement_traits
#define ADDRESS_GAME_ENGINE_APPLY_SENSORS_TRAITS                         0x11E170 // game_engine_apply_sensors_traits
#define ADDRESS_GAME_ENGINE_APPLY_SHIELD_VITALITY_TRAITS                 0x11DD50 // game_engine_apply_shield_vitality_traits
#define ADDRESS_GAME_ENGINE_APPLY_WEAPONS_TRAITS                         0x11DE00 // game_engine_apply_weapons_traits
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4         0x0C9D51 // game_engine_update_after_game_update_state+0x161
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK4_RETURN  0x0C9D58 // game_engine_update_after_game_update_state+0x168
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2                          0x0F917F // game_engine_player_killed+0x28f
#define ADDRESS_GAME_ENGINE_PLAYER_KILLED_HOOK2_RETURN                   0x0F91A7 // game_engine_player_killed+0x2b7
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK               0x0CB1DD // game_engine_update_player_sitting_out+0x4d
#define ADDRESS_GAME_ENGINE_UPDATE_PLAYER_SITTING_OUT_HOOK_RETURN        0x0CB1E5 // game_engine_update_player_sitting_out+0x55
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1                 0x0B5544 // player_swap+0x204
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK1_RETURN          0x0B554E // player_swap+0x20e
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2                 0x0B558C // player_swap+0x24c
#define ADDRESS_GAME_ENGINE_PLAYER_CHANGED_INDICES_HOOK2_RETURN          0x0B5597 // player_swap+0x257
#define ADDRESS_PLAYER_DELETE_HOOK                                       0x0B5737 // player_delete+0xc7
#define ADDRESS_PLAYER_DELETE_HOOK_RETURN                                0x0B573C // player_delete+0xcc
#define ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK                             0x0FA5D1 // game_engine_player_left+0x101
#define ADDRESS_GAME_ENGINE_PLAYER_LEFT_HOOK_RETURN                      0x0FA5D6 // game_engine_player_left+0x106
#define ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK                          0x11867D // teleporter_teleport_object+0x18d
#define ADDRESS_TELEPORTER_TELEPORT_OBJECT_HOOK_RETURN                   0x118682 // teleporter_teleport_object+0x192

// anvil\hooks\simulation\hooks_simulation.cpp
#define ADDRESS_UPDATE_ESTABLISHING_VIEW                                 0x0370E0 // c_simulation_world::update_establishing_view
#define ADDRESS_INTERNAL_HALT_RENDER_THREAD_AND_LOCK_RESOURCES_CALL      0x0C703E // game_engine_game_starting+0x5e
#define ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY            0x0C7320 // game_engine_game_ending+0x50
#define ADDRESS_GAME_ENGINE_DETACH_FROM_SIMULATION_GRACEFULLY_RETURN     0x0C7353 // game_engine_game_ending+0x83

// anvil\hooks\simulation\hooks_simulation_events.cpp
#define ADDRESS_DAMAGE_SECTION_DEPLETE_HOOK                              0x414EC5 // damage_section_deplete+0x245
#define ADDRESS_DAMAGE_SECTION_DEPLETE_HOOK_RETURN                       0x414ECA // damage_section_deplete+0x24a
#define ADDRESS_DAMAGE_SECTION_RESPOND_TO_DAMAGE_HOOK                    0x414B96 // damage_section_respond_to_damage+0x466
#define ADDRESS_DAMAGE_SECTION_RESPOND_TO_DAMAGE_HOOK_RETURN             0x414B9E // damage_section_respond_to_damage+0x46e
#define ADDRESS_OBJECT_DAMAGE_NEW_HOOK                                   0x40C9C4 // object_damage_new+0x224
#define ADDRESS_OBJECT_DAMAGE_NEW_HOOK_RETURN                            0x40C9C9 // object_damage_new+0x229
#define ADDRESS_OBJECT_CAUSE_DAMAGE_HOOK                                 0x40FB0E // object_cause_damage+0x107e
#define ADDRESS_OBJECT_CAUSE_DAMAGE_HOOK_RETURN                          0x40FB13 // object_cause_damage+0x1083
#define ADDRESS_PROJECTILE_ATTACH_HOOK2                                  0x468045 // projectile_attach+0x3f5
#define ADDRESS_PROJECTILE_ATTACH_HOOK2_RETURN                           0x46804B // projectile_attach+0x3fb
#define ADDRESS_PROJECTILE_DETONATE_EFFECTS_AND_DAMAGE                   0x4667D0 // projectile_detonate_effects_and_damage
#define ADDRESS_PROJECTILE_DETONATE_PATCH                                0x467250 // projectile_detonate+0x8f0
#define ADDRESS_PROJECTILE_COLLISION_END                                 0x465667
#define ADDRESS_PROJECTILE_COLLISION                                     0x463930 // projectile_collision
#define ADDRESS_PROJECTILE_COLLISION_HOOK0                               0x464FD4 // projectile_collision+0x16a4
#define ADDRESS_PROJECTILE_COLLISION_HOOK0_RETURN                        0x464FDC // projectile_collision+0x16ac
#define ADDRESS_PROJECTILE_COLLISION_HOOK1                               0x46528B // projectile_collision+0x195b
#define ADDRESS_PROJECTILE_COLLISION_HOOK1_RETURN                        0x465291 // projectile_collision+0x1961
#define ADDRESS_PROJECTILE_COLLISION_HOOK2                               0x46530B // projectile_collision+0x19db
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_RETURN                        0x465313 // projectile_collision+0x19e3
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_2                             0x4653D3 // projectile_collision+0x1aa3
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_2_RETURN                      0x4653D8 // projectile_collision+0x1aa8
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_3                             0x465454 // projectile_collision+0x1b24
#define ADDRESS_PROJECTILE_COLLISION_HOOK2_3_RETURN                      0x46545D // projectile_collision+0x1b2d
#define ADDRESS_PROJECTILE_COLLISION_HOOK3                               0x465472 // projectile_collision+0x1b42
#define ADDRESS_PROJECTILE_COLLISION_HOOK3_RETURN                        0x46547A // projectile_collision+0x1b4a
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP                       0x465591 // projectile_collision+0x1c61
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_RETURN                0x465596 // projectile_collision+0x1c66
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_2                     0x4655C3 // projectile_collision+0x1c93
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_2_RETURN              0x4655C8 // projectile_collision+0x1c98
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_3                     0x4655F1 // projectile_collision+0x1cc1
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_3_RETURN              0x4655F6 // projectile_collision+0x1cc6
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_4                     0x465657 // projectile_collision+0x1d27
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_4_RETURN              0x46565C // projectile_collision+0x1d2c
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_5                     0x46565F // projectile_collision+0x1d2f
#define ADDRESS_PROJECTILE_COLLISION_STACK_CLEANUP_5_RETURN              0x465666 // projectile_collision+0x1d36
#define ADDRESS_UNIT_ACTION_VEHICLE_BOARD_SUBMIT_HOOK                    0x4479F3 // unit_action_vehicle_board_submit+0x133
#define ADDRESS_UNIT_ACTION_VEHICLE_BOARD_SUBMIT_HOOK_RETURN             0x4479FA // unit_action_vehicle_board_submit+0x13a
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_INTERNAL_HOOK                  0x456CC6 // motor_animation_exit_seat_internal+0x156
#define ADDRESS_MOTOR_ANIMATION_EXIT_SEAT_INTERNAL_HOOK_RETURN           0x456CCF // motor_animation_exit_seat_internal+0x15f
#define ADDRESS_UNIT_RESOLVE_MELEE_ATTACK_HOOK                           0x42C270 // unit_resolve_melee_attack+0x360
#define ADDRESS_UNIT_RESOLVE_MELEE_ATTACK_HOOK_RETURN                    0x42C276 // unit_resolve_melee_attack+0x366
#define ADDRESS_GAME_ENGINE_SEND_EVENT                                   0x11C0C0 // game_engine_send_event
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK2                          0x0FAF89 // game_engine_earn_wp_event+0x119
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK2_RETURN                   0x0FAFC3 // game_engine_earn_wp_event+0x153
#define ADDRESS_GAME_ENGINE_SCORING_UPDATE_LEADERS_INTERNAL_HOOK         0x0E00DF // game_engine_scoring_update_leaders_internal+0x2ff
#define ADDRESS_GAME_ENGINE_SCORING_UPDATE_LEADERS_INTERNAL_HOOK_RETURN  0x0E011A // game_engine_scoring_update_leaders_internal+0x33a
#define ADDRESS_GAME_ENGINE_AWARD_MEDAL_HOOK                             0x0FB1D8 // award_medal+0x178
#define ADDRESS_GAME_ENGINE_AWARD_MEDAL_HOOK_RETURN                      0x0FB209 // award_medal+0x1a9
#define ADDRESS_C_SLAYER_ENGINE_EMIT_GAME_START_EVENT_HOOK               0x22E2D2 // c_slayer_engine::emit_game_start_event+0x62
#define ADDRESS_C_SLAYER_ENGINE_EMIT_GAME_START_EVENT_HOOK_RETURN        0x22E308 // c_slayer_engine::emit_game_start_event+0x98
#define ADDRESS_DISPLAY_TELEPORTER_BLOCKED_MESSAGE_HOOK                  0x11887F // display_teleporter_blocked_message+0x4f
#define ADDRESS_DISPLAY_TELEPORTER_BLOCKED_MESSAGE_HOOK_RETURN           0x1188B4 // display_teleporter_blocked_message+0x84
#define ADDRESS_C_TELEPORTER_AREA_UPDATE_PLAYERS_HOOK                    0x1187EF
#define ADDRESS_C_TELEPORTER_AREA_UPDATE_PLAYERS_HOOK_RETURN             0x118824

// anvil\hooks\simulation\hooks_simulation_globals.cpp
#define ADDRESS_GAME_ENGINE_UPDATE_ROUND_CONDITIONS                      0x0C6C00 // game_engine_update_round_conditions
#define ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK                             0x0C98CB // game_engine_update_time+0x14b
#define ADDRESS_GAME_ENGINE_UPDATE_TIME_HOOK_RETURN                      0x0C98D1 // game_engine_update_time+0x151
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2                      0x0CA265 // game_engine_update_after_game+0xe5
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK2_RETURN               0x0CA26C // game_engine_update_after_game+0xec
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1         0x0C9C2A // game_engine_update_after_game_update_state+0x3a
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK1_RETURN  0x0C9C31 // game_engine_update_after_game_update_state+0x41
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2         0x0C9D9B // game_engine_update_after_game_update_state+0x1ab
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_UPDATE_STATE_HOOK2_RETURN  0x0C9DA2 // game_engine_update_after_game_update_state+0x1b2
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1                    0x0DC9F2 // game_engine_build_initial_teams+0xf2
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK1_RETURN             0x0DC9F9 // game_engine_build_initial_teams+0xf9
#define ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK                0x0DC51D // game_engine_build_valid_team_mapping+0x10d
#define ADDRESS_GAME_ENGINE_BUILD_VALID_TEAM_MAPPING_HOOK_RETURN         0x0DC522 // game_engine_build_valid_team_mapping+0x112
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK                  0x0DC661 // game_engine_recompute_active_teams+0x81
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS_HOOK_RETURN           0x0DC666 // game_engine_recompute_active_teams+0x86
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2                    0x0DCA19 // game_engine_build_initial_teams+0x119
#define ADDRESS_GAME_ENGINE_BUILD_INITIAL_TEAMS_HOOK2_RETURN             0x0DCA1F // game_engine_build_initial_teams+0x11f
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK               0x0DC847 // game_engine_teams_use_one_shared_life+0x57
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE_HOOK_RETURN        0x0DC84C // game_engine_teams_use_one_shared_life+0x5c
#define ADDRESS_C_CTF_ENGINE_GAME_STARTING                               0x22852F // c_ctf_engine::game_starting+0x2f
#define ADDRESS_C_CTF_ENGINE_GAME_STARTING_RETURN                        0x228535 // c_ctf_engine::game_starting+0x35
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1                0x229A03 // c_ctf_engine::get_time_left_in_ticks+0x283
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK1_RETURN         0x229A0A // c_ctf_engine::get_time_left_in_ticks+0x28a
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2                0x229A2A // c_ctf_engine::get_time_left_in_ticks+0x2aa
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK2_RETURN         0x229A31 // c_ctf_engine::get_time_left_in_ticks+0x2b1
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3                0x229A5B // c_ctf_engine::get_time_left_in_ticks+0x2db
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK3_RETURN         0x229A62 // c_ctf_engine::get_time_left_in_ticks+0x2e2
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4                0x229A7B // c_ctf_engine::get_time_left_in_ticks+0x2fb
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK4_RETURN         0x229A82 // c_ctf_engine::get_time_left_in_ticks+0x302
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5                0x229992 // c_ctf_engine::get_time_left_in_ticks+0x212
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK5_RETURN         0x22999A // c_ctf_engine::get_time_left_in_ticks+0x21a
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6                0x229ADB // c_ctf_engine::get_time_left_in_ticks+0x35b
#define ADDRESS_C_CTF_ENGINE_GET_TIME_LEFT_IN_TICKS_HOOK6_RETURN         0x229AE2 // c_ctf_engine::get_time_left_in_ticks+0x362
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK               0x228379 // c_ctf_engine::initialize_for_new_round+0x109
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_FOR_NEW_ROUND_HOOK_RETURN        0x228383 // c_ctf_engine::initialize_for_new_round+0x113
#define ADDRESS_C_CTF_ENGINE_INITIALIZE_OBJECT_DATA                      0x228220 // c_ctf_engine::initialize_object_data

// anvil\hooks\simulation\hooks_statborg.cpp
#define ADDRESS_GAME_ENGINE_PLAYER_ADDED                                 0x0FA2D0 // game_engine_player_added
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK                       0x0CA2F0 // game_engine_update_after_game+0x170
#define ADDRESS_GAME_ENGINE_UPDATE_AFTER_GAME_HOOK_RETURN                0x0CA2F7 // game_engine_update_after_game+0x177
#define ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK                  0x1AF61D // c_game_statborg::adjust_player_stat+0x4d
#define ADDRESS_C_GAME_STATBORG_ADJUST_PLAYER_STAT_HOOK_RETURN           0x1AF624 // c_game_statborg::adjust_player_stat+0x54
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1                  0x0C8AF3 // game_engine_end_round_with_winner+0x143
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK1_RETURN           0x0C8AFA // game_engine_end_round_with_winner+0x14a
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2                  0x0C8C3D // game_engine_end_round_with_winner+0x28d
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK2_RETURN           0x0C8C44 // game_engine_end_round_with_winner+0x294
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK                           0x0FB000 // game_engine_earn_wp_event+0x190
#define ADDRESS_GAME_ENGINE_EARN_WP_EVENT_HOOK_RETURN                    0x0FB007 // game_engine_earn_wp_event+0x197
#define ADDRESS_ADJUST_TEAM_STAT                                         0x1AF710 // c_game_statborg::adjust_team_stat
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3                  0x0C8A5F // game_engine_end_round_with_winner+0xaf
#define ADDRESS_GAME_ENGINE_END_ROUND_WITH_WINNER_HOOK3_RETURN           0x0C8A66 // game_engine_end_round_with_winner+0xb6
#define ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK                  0x1C7FC4 // c_game_engine::recompute_team_score+0x104
#define ADDRESS_C_GAME_ENGINE_RECOMPUTE_TEAM_SCORE_HOOK_RETURN           0x1C7FD2 // c_game_engine::recompute_team_score+0x112
#define ADDRESS_PLAYER_CHANGED_TEAMS_HOOK                                0x0FA956 // game_engine_player_changed_teams+0x56
#define ADDRESS_PLAYER_CHANGED_TEAMS_HOOK_RETURN                         0x0FA95F // game_engine_player_changed_teams+0x5f
#define ADDRESS_GAME_ENGINE_PLAYER_INDICES_SWAPPED                       0x0FA740 // game_engine_player_indices_swapped
#define ADDRESS_STATS_RESET_FOR_ROUND_SWITCH                             0x1AEE00 // c_game_statborg::stats_reset_for_round_switch

// anvil\hooks\simulation\hooks_weapon_updates.cpp
#define ADDRESS_WEAPON_AGE_HOOK                                          0x433CE6 // weapon_age+0xc6
#define ADDRESS_WEAPON_AGE_HOOK_RETURN                                   0x433CF0 // weapon_age+0xd0
#define ADDRESS_WEAPON_BARREL_FIRE_HOOK                                  0x43577A // weapon_barrel_fire+0x35a
#define ADDRESS_WEAPON_BARREL_FIRE_HOOK_RETURN                           0x43577F // weapon_barrel_fire+0x35f
#define ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK                      0x434ECE // weapon_magazine_execute_reload+0x10e
#define ADDRESS_WEAPON_MAGAZINE_EXECUTE_RELOAD_HOOK_RETURN               0x434ED6 // weapon_magazine_execute_reload+0x116
#define ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK                              0x42DBB4 // weapon_magazine_update+0xd4
#define ADDRESS_WEAPON_MAGAZINE_UPDATE_HOOK_RETURN                       0x42DBB9 // weapon_magazine_update+0xd9
#define ADDRESS_WEAPON_REPORT_KILL_HOOK                                  0x4339C5 // weapon_report_kill+0xf5
#define ADDRESS_WEAPON_REPORT_KILL_HOOK_RETURN                           0x4339CA // weapon_report_kill+0xfa
#define ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK                           0x43374B // weapon_set_current_amount+0x10b
#define ADDRESS_WEAPON_SET_CURRENT_AMOUNT_HOOK_RETURN                    0x433750 // weapon_set_current_amount+0x110
#define ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK                             0x433479 // weapon_set_total_rounds+0xb9
#define ADDRESS_WEAPON_SET_TOTAL_ROUNDS_HOOK_RETURN                      0x43347F // weapon_set_total_rounds+0xbf
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1                       0x432147 // weapon_take_inventory_rounds+0xa7
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK1_RETURN                0x43214E // weapon_take_inventory_rounds+0xae
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2                       0x4321E1 // weapon_take_inventory_rounds+0x141
#define ADDRESS_WEAPON_TAKE_INVENTORY_ROUNDS_HOOK2_RETURN                0x4321E8 // weapon_take_inventory_rounds+0x148
#define ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK                               0x42E055 // weapon_trigger_update+0x205
#define ADDRESS_WEAPON_TRIGGER_UPDATE_HOOK_RETURN                        0x42E05A // weapon_trigger_update+0x20a
#define ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK              0x4310CC // weapon_handle_potential_inventory_item+0x3bc
#define ADDRESS_WEAPON_HANDLE_POTENTIAL_INVENTORY_ITEM_HOOK_RETURN       0x4310D2 // weapon_handle_potential_inventory_item+0x3c2
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_END                      0x426DD6
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX                          0x426D10 // unit_inventory_set_weapon_index
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0                    0x426D16 // unit_inventory_set_weapon_index+0x6
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK0_RETURN             0x426D1C // unit_inventory_set_weapon_index+0xc
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1                    0x426D8E // unit_inventory_set_weapon_index+0x7e
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_HOOK1_RETURN             0x426DCF // unit_inventory_set_weapon_index+0xbf
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP            0x426DCF // unit_inventory_set_weapon_index+0xbf
#define ADDRESS_UNIT_INVENTORY_SET_WEAPON_INDEX_STACK_CLEANUP_RETURN     0x426DD5 // unit_inventory_set_weapon_index+0xc5
#define ADDRESS_UNIT_INVENTORY_CYCLE_WEAPON_SET_IDENTIFIER               0x426CC0 // unit_inventory_cycle_weapon_set_identifier
#define ADDRESS_UNIT_DELETE_ALL_WEAPONS_INTERNAL                         0x424E60 // unit_delete_all_weapons_internal
#define ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK                          0x427119 // unit_handle_deleted_object+0xa9
#define ADDRESS_UNIT_HANDLE_DELETED_OBJECT_HOOK_RETURN                   0x427178 // unit_handle_deleted_object+0x108

// anvil\server_tools.cpp
#define ADDRESS_TUTORIAL_START_PATCH                                     0x33AB0D // tutorial_start+0x4d
#define ADDRESS_TUTORIAL_START_PATCH_2                                   0x33AB58 // tutorial_start+0x98
#define ADDRESS_LEVELS_GET_TUTORIAL_MAP_ID_PATCH                         0x0DD176 // levels_get_tutorial_map_id+0xa6

// cache\cache_file_tag_resource_runtime.cpp
#define ADDRESS_TAG_RESOURCES_LOCK_GAME                                  0x07E7E0 // tag_resources_lock_game
#define ADDRESS_TAG_RESOURCES_UNLOCK_GAME                                0x07E830 // tag_resources_unlock_game

// cache\cache_files.cpp
#define ADDRESS_G_CACHE_FILE_GLOBALS                                     0x3EDDCD0 // g_cache_file_globals
#define ADDRESS_CACHE_FILE_GET_GLOBAL_TAG_INDEX                          0x082E30 // cache_file_get_global_tag_index

// cache\cache_files_windows.cpp
#define ADDRESS_CACHE_FILE_TABLE_OF_CONTENTS                             0x3FE1408 // cache_file_table_of_contents
#define ADDRESS_CACHED_MAP_FILE_OPEN_FOR_RUNNING_OFF_DVD                 0x0ED780 // cached_map_file_open_for_running_off_dvd

// cache\physical_memory_map.cpp
#define ADDRESS_PHYSICAL_MEMORY_MALLOC_FIXED                             0x0A0580 // _physical_memory_malloc_fixed

// cache\restricted_memory.cpp
#define ADDRESS_G_RESTRICTED_SECTION                                     0x3F16310 // g_restricted_section
#define ADDRESS_G_RESTRICTED_REGIONS                                     0x3FDCAB8 // g_restricted_regions
#define ADDRESS_C_RESTRICTED_MEMORY_ADD_MEMBER                           0x1690E0 // c_restricted_memory::add_member

// camera\camera_globals.cpp
#define ADDRESS_G_DIRECTOR_CAMERA_SPEED_SCALE                            0xEA3E98 // g_director_camera_speed_scale

// camera\debug_director.cpp
#define ADDRESS_C_DIRECTOR_UPDATE                                        0x0E3110 // c_director::update

// camera\director.cpp
#define ADDRESS_C_DIRECTOR_SET_CAMERA_MODE_INTERNAL                      0x0E3460 // c_director::set_camera_mode_internal
#define ADDRESS_DIRECTOR_SET_MODE                                        0x0E2E80 // director_set_mode

// cseries\async.cpp
#define ADDRESS_ASYNC_YIELD_UNTIL_DONE_FUNCTION                          0x098B80 // async_yield_until_done_function

// cseries\async_helpers.cpp
#define ADDRESS_ASYNC_READ_POSITION                                      0x0C40C0 // async_read_position

// cseries\async_xoverlapped.cpp
#define ADDRESS_OVERLAPPED_UPDATE                                        0x0C5780 // overlapped_update

// cseries\cseries.cpp
#define ADDRESS_G_NORMAL_ALLOCATION                                      0xE9B960 // g_normal_allocation
#define ADDRESS_BIT_VECTOR_COUNT_BITS                                    0x0C39D0 // bit_vector_count_bits
#define ADDRESS_INDEX_FROM_MASK                                          0x0C3C10 // player_index_from_mask

// cseries\cseries_windows_debug_pc.cpp
#define ADDRESS_G_EXCEPTION_TIME                                         0x106DECC // g_exception_time
#define ADDRESS_G_EXCEPTION_CACHING_IN_PROGRESS                          0x40274B8 // g_exception_caching_in_progress
#define ADDRESS_G_EXCEPTION_INFORMATION                                  0x40274BC // g_exception_information
#define ADDRESS_EXCEPTION_CODE_GET_STRING                                0x167E80 // exception_code_get_string
#define ADDRESS_CRASHDUMP_FROM_EXCEPTION                                 0x2B4650 // crashdump_from_exception
#define ADDRESS_BUILD_EXCEPTION_POINTERS                                 0x167BD0 // build_exception_pointers

// cseries\language.cpp
#define ADDRESS_GET_CURRENT_LANGUAGE                                     0x0B0C00 // get_current_language

// cseries\progress.cpp
#define ADDRESS_PROGRESS_GLOBALS                                         0x4EC4720 // progress_globals

// game\game.cpp
#define ADDRESS_G_DISABLE_VIDEO                                          0x104DE54 // g_disable_video
#define ADDRESS_G_DISABLE_AUDIO                                          0x104DD9A // g_disable_audio
#define ADDRESS_GAME_ENGINE_TEAMS_USE_ONE_SHARED_LIFE                    0x0DC7F0 // game_engine_teams_use_one_shared_life

// game\game_allegiance.cpp
#define ADDRESS_GAME_TEAM_IS_ENEMY                                       0x171070 // game_team_is_enemy

// game\game_engine.cpp
#define ADDRESS_K_GAME_ENGINE_END_CONDITIONS                             0xE9C240 // k_game_engine_end_conditions
#define ADDRESS_GAME_ENGINES                                             0xF01EC0 // game_engines
#define ADDRESS_GAME_ENGINE_PLAYER_IS_PLAYING                            0x0C77A0 // game_engine_player_is_playing
#define ADDRESS_GAME_ENGINE_GET_MULTIPLAYER_STRING                       0x0CD1F0 // game_engine_get_multiplayer_string

// game\game_engine_assault.cpp
#define ADDRESS_C_GAME_ENGINE_ASSAULT_VARIANT_CONSTRUCTOR_VFTABLE        0xD714A0 // ?_7c_game_engine_assault_variant
#define ADDRESS_C_GAME_ENGINE_ASSAULT_VARIANT_SET                        0x1A7B50 // c_game_engine_assault_variant::set

// game\game_engine_candy_monitor.cpp
#define ADDRESS_GAME_ENGINE_REGISTER_OBJECT                              0x172600 // game_engine_register_object

// game\game_engine_ctf.cpp
#define ADDRESS_C_GAME_ENGINE_CTF_VARIANT_CONSTRUCTOR_VFTABLE            0xD71470 // ?_7c_game_engine_ctf_variant
#define ADDRESS_C_GAME_ENGINE_CTF_VARIANT_SET                            0x1AE560 // c_game_engine_ctf_variant::set

// game\game_engine_default.cpp
#define ADDRESS_C_GAME_ENGINE_BASE_VARIANT_CONSTRUCTOR_VFTABLE           0xD71410 // ?_7c_game_engine_base_variant
#define ADDRESS_C_GAME_ENGINE_BASE_VARIANT_SET                           0x1705C0 // c_game_engine_base_variant::set_0

// game\game_engine_events.cpp
#define ADDRESS_AUDIENCE_MEMBER_FIND_RESPONSE                            0x0D0E40 // audience_member_find_response

// game\game_engine_infection.cpp
#define ADDRESS_C_GAME_ENGINE_INFECTION_VARIANT_CONSTRUCTOR_VFTABLE      0xD71320 // ?_7c_game_engine_infection_variant
#define ADDRESS_C_GAME_ENGINE_INFECTION_VARIANT_SET                      0x1ADCB0 // c_game_engine_infection_variant::set

// game\game_engine_juggernaut.cpp
#define ADDRESS_C_GAME_ENGINE_JUGGERNAUT_VARIANT_CONSTRUCTOR_VFTABLE     0xD714D0 // ?_7c_game_engine_juggernaut_variant
#define ADDRESS_C_GAME_ENGINE_JUGGERNAUT_VARIANT_SET                     0x1AA590 // c_game_engine_juggernaut_variant::set

// game\game_engine_king.cpp
#define ADDRESS_C_GAME_ENGINE_KING_VARIANT_CONSTRUCTOR_VFTABLE           0xD713B0 // ?_7c_game_engine_king_variant
#define ADDRESS_C_GAME_ENGINE_KING_VARIANT_SET                           0x1AC240 // c_game_engine_king_variant::set

// game\game_engine_oddball.cpp
#define ADDRESS_C_GAME_ENGINE_ODDBALL_VARIANT_CONSTRUCTOR_VFTABLE        0xD71440 // ?_7c_game_engine_oddball_variant
#define ADDRESS_C_GAME_ENGINE_ODDBALL_VARIANT_SET                        0x1A9D90 // c_game_engine_oddball_variant::set

// game\game_engine_sandbox.cpp
#define ADDRESS_C_GAME_ENGINE_SANDBOX_VARIANT_CONSTRUCTOR_VFTABLE        0xD71350 // ?_7c_game_engine_sandbox_variant
#define ADDRESS_C_GAME_ENGINE_SANDBOX_VARIANT_SET                        0x1ABB90 // c_game_engine_sandbox_variant::set

// game\game_engine_scoring.cpp
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_0                           0x3FDC928
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_1                           0x3FDC9A8
#define ADDRESS_SCORING_STATBORG_RESET_VALUE_2                           0x3FDCA28

// game\game_engine_slayer.cpp
#define ADDRESS_C_GAME_ENGINE_SLAYER_VARIANT_CONSTRUCTOR_VFTABLE         0xD71500 // ?_7c_game_engine_slayer_variant
#define ADDRESS_C_GAME_ENGINE_SLAYER_VARIANT_SET                         0x1AC9F0 // c_game_engine_slayer_variant::set

// game\game_engine_team.cpp
#define ADDRESS_GAME_ENGINE_RECOMPUTE_ACTIVE_TEAMS                       0x0DC5E0 // game_engine_recompute_active_teams
#define ADDRESS_GAME_ENGINE_ADJUST_TEAM_SCORE_FOR_COMPOSITION            0x0C9330 // game_engine_adjust_team_score_for_composition
#define ADDRESS_GAME_ENGINE_VARIANT_GET_MAXIMUM_TEAM_COUNT               0x0DC6F0 // game_engine_variant_get_maximum_team_count
#define ADDRESS_GAME_ENGINE_TEAM_INDEX_TO_TEAM_DESIGNATOR                0x0DC3B0 // game_engine_team_index_to_team_designator

// game\game_engine_territories.cpp
#define ADDRESS_C_GAME_ENGINE_TERRITORIES_VARIANT_CONSTRUCTOR_VFTABLE    0xD71380 // ?_7c_game_engine_territories_variant
#define ADDRESS_C_GAME_ENGINE_TERRITORIES_VARIANT_SET                    0x1AB3C0 // c_game_engine_territories_variant::set

// game\game_engine_traits.cpp
#define ADDRESS_GAME_ENGINE_ASSEMBLE_PLAYER_TRAITS                       0x11D7E0 // game_engine_assemble_player_traits

// game\game_engine_util.cpp
#define ADDRESS_GAME_ENGINE_GET_MULTIPLAYER_WEAPON_SELECTION_ABSOLUTE_INDEX 0x11C250 // game_engine_get_multiplayer_weapon_selection_absolute_index
#define ADDRESS_GAME_ENGINE_HANDLE_EVENT                                 0x0D0AD0 // game_engine_handle_event

// game\game_engine_variant.cpp
#define ADDRESS_BUILD_DEFAULT_GAME_VARIANT                               0x0E9BE0 // build_default_game_variant

// game\game_engine_vip.cpp
#define ADDRESS_C_GAME_ENGINE_VIP_VARIANT_CONSTRUCTOR_VFTABLE            0xD713E0 // ?_7c_game_engine_vip_variant
#define ADDRESS_C_GAME_ENGINE_VIP_VARIANT_SET                            0x1A9020 // c_game_engine_vip_variant::set

// game\game_globals.cpp
#define ADDRESS_K_DIFFICULTY_VALUE_INDIRECTION_TABLE                     0xD3D550 // game_difficulty_value_table
#define ADDRESS_GLOBAL_GAME_GLOBALS                                      0x103E788 // global_game_globals

// game\game_results.cpp
#define ADDRESS_G_CURRENT_GAME_RESULTS                                   0x3FC2750 // g_current_game_results
#define ADDRESS_GAME_RESULTS_NOTIFY_PLAYER_INDICES_CHANGED               0x0CDBA0 // game_results_notify_player_indices_changed
#define ADDRESS_GAME_RESULTS_STATISTIC_SET_CALL                          0x0CE0B0 // game_results_statistic_set
#define ADDRESS_GAME_RESULTS_STATISTIC_INCREMENT                         0x0CDFB0 // game_results_statistic_increment

// game\player_appearance.cpp
#define ADDRESS_MODIFIER_GET_NAME                                        0x2E77A0 // modifier_get_name

// game\player_mapping.cpp
#define ADDRESS_PLAYER_MAPPING_GET_NEXT_OUTPUT_USER                      0x0E1F90 // player_mapping_get_next_output_user
#define ADDRESS_PLAYER_MAPPING_SET_INPUT_USER                            0x0E19E0 // player_mapping_set_input_user
#define ADDRESS_PLAYER_MAPPING_SET_INPUT_CONTROLLER                      0x0E1AA0
#define ADDRESS_PLAYER_MAPPING_ATTACH_OUTPUT_USER                        0x0E1CE0

// game\players.cpp
#define ADDRESS_PLAYER_CONTROL_SET_FACING                                0x106780 // player_control_set_facing
#define ADDRESS_PLAYER_IS_LOCAL                                          0x0C1480 // player_is_local
#define ADDRESS_PLAYER_CLEAR_ASSASSINATION_STATE                         0x0BA0F0 // player_clear_assassination_state
#define ADDRESS_PLAYER_SWAP                                              0x0B5340 // player_swap
#define ADDRESS_PLAYER_DELETE                                            0x0B5670 // player_delete
#define ADDRESS_PLAYER_SET_CONFIGURATION                                 0x0B48C0 // player_set_configuration
#define ADDRESS_PLAYERS_REBUILD_USER_MAPPING                             0x0B4660 // players_rebuild_user_mapping

// hf2p\hf2p.cpp
#define ADDRESS_GAME_STARTUP_MAIN                                        0x2B0280 // hf2p_main_initialize
#define ADDRESS_GAME_STARTUP_CLIENT                                      0x2B0560 // game_startup_client
#define ADDRESS_HF2P_SESSION_INVALID                                     0x557EC0 // hf2p_session_invalid

// hf2p\hf2p_session.cpp
#define ADDRESS_HF2P_SETUP_SESSION                                       0x3AAF10 // hf2p_setup_session
#define ADDRESS_HF2P_HANDLE_DISCONNECTION                                0x2F29C0 // hf2p_handle_disconnection
#define ADDRESS_HF2P_JOIN_GAME                                           0x319880 // hf2p_join_game

// hf2p\hq.cpp
#define ADDRESS_HQ_START_TUTORIAL_LEVEL                                  0x33AAC0 // tutorial_start

// hf2p\loadouts.cpp
#define ADDRESS_PLAYER_UPDATE_LOADOUT_INTERNAL                           0x0E05E0 // player_update_loadout_internal
#define ADDRESS_EQUIPMENT_ADD                                            0x0E0530 // equipment_add

// hf2p\podium.cpp
#define ADDRESS_G_PLAYER_PODIUM_COUNT                                    0x4A2973C // g_player_podiums_count
#define ADDRESS_G_PLAYER_PODIUMS                                         0x4A29740 // g_player_podiums
#define ADDRESS_HF2P_PLAYER_PODIUM_INCREMENT_LOOP_COUNT                  0x2E8430 // hf2p_player_podium_increment_loop_count

// hs\hs_function.cpp
#define ADDRESS_HS_FUNCTION_TABLE                                        0xEA4188 // hs_function_table

// hs\hs_runtime.cpp
#define ADDRESS_HS_RETURN                                                0x1210F0 // hs_return
#define ADDRESS_HS_ARGUMENTS_EVALUATE                                    0x121880 // hs_arguments_evaluate

// input\input_windows.cpp
#define ADDRESS_INPUT_GLOBALS                                            0x3EDD060 // input_globals
#define ADDRESS_INPUT_CLEAR_ALL_RUMBLERS                                 0x0817E0 // input_clear_all_rumblers
#define ADDRESS_INPUT_UPDATE                                             0x080C10 // input_update

// interface\interface_constants.cpp
#define ADDRESS_G_ASPECT_RATIO_SCALE                                     0xEB0CAC // g_aspect_ratio_scale
#define ADDRESS_CALCULATE_ASPECT_RATIO_SCALING                           0x3C5E80 // calculate_aspect_ratio_scaling

// interface\user_interface_controller.cpp
#define ADDRESS_USER_INTERFACE_CONTROLLER_GET_SIGNED_IN_CONTROLLER_COUNT 0x3AD970 // user_interface_controller_get_signed_in_controller_count

// interface\user_interface_session.cpp
#define ADDRESS_USER_INTERFACE_SQUAD_SET_GAME_VARIANT                    0x3ABEC0 // user_interface_squad_set_game_variant
#define ADDRESS_USER_INTERFACE_SQUAD_SET_MULTIPLAYER_MAP                 0x3ABC60 // user_interface_squad_set_multiplayer_map
#define ADDRESS_USER_INTERFACE_SET_DESIRED_MULTIPLAYER_MODE              0x3AA7D0 // user_interface_set_desired_multiplayer_mode

// items\projectiles.cpp
#define ADDRESS_PROJECTILE_DETONATE_EFFECTS_AND_DAMAGE_SHARED            0x466620 // projectile_detonate_effects_and_damage_shared

// items\weapons.cpp
#define ADDRESS_WEAPON_DELAY_PREDICTED_STATE                             0x432310 // weapon_delay_predicted_state
#define ADDRESS_WEAPON_GET_OWNER_UNIT_INVENTORY_INDEX                    0x433D90 // weapon_get_owner_unit_inventory_index

// main\console.cpp
#define ADDRESS_NET_SHOW_LATENCY_AND_FRAMERATE_METRICS_ON_CHUD           0x103E768 // g_network_interface_show_latency_and_framerate_metrics_on_chud
#define ADDRESS_NET_FAKE_LATENCY_AND_FRAMERATE_METRICS_ON_CHUD           0x1038283 // g_network_interface_fake_latency_and_framerate_metrics_on_chud

// main\debug_keys.cpp
#define ADDRESS_DISPLAY_FRAMERATE                                        0x103E7C4 // display_framerate

// main\levels.cpp
#define ADDRESS_LEVELS_ADD_CAMPAIGN                                      0x0DE220 // levels_add_campaign
#define ADDRESS_LEVELS_GET_AVAILABLE_MAP_MASK                            0x0DD750 // build_peer_mp_map_mask

// main\main.cpp
#define ADDRESS_G_STATUS_VALUES                                          0x103E7C8 // g_status_values
#define ADDRESS_MAIN_GLOBALS                                             0x3EE1E48 // main_globals
#define ADDRESS_INTERNAL_HALT_RENDER_THREAD_AND_LOCK_RESOURCES           0x094CB0 // _internal_halt_render_thread_and_lock_resources
#define ADDRESS_LAST_RESOURCE_OWNER                                      0x190E460
#define ADDRESS_UNLOCK_RESOURCES_AND_RESUME_RENDER_THREAD                0x094E00 // unlock_resources_and_resume_render_thread

// main\main.h
#define ADDRESS_MAIN_STATUS                                              0x096D20 // main_status

// main\main_game.cpp
#define ADDRESS_MAIN_GAME_GLOBALS                                        0x3F8E588 // main_game_globals

// main\main_render.cpp
#define ADDRESS_GAME_SESSION_ID                                          0x4A2CBD8
#define ADDRESS_G_WATERMARK_SCALES                                       0xD81498 // g_watermark_scales
#define ADDRESS_MAIN_RENDER_PREGAME                                      0x163B00 // main_render_pregame

// main\main_time.cpp
#define ADDRESS_MAIN_TIME_FRAME_RATE_DISPLAY                             0x09FE00 // main_time_frame_rate_display

// math\random_math.cpp
#define ADDRESS_RANDOM_SEED_LOCAL                                        0x4EBF024 // g_nondeterministic_random_seed

// math\real_math.cpp
#define ADDRESS_GENERATE_UP_VECTOR3D                                     0x084960 // generate_up_vector3d

// memory\bitstream.cpp
#define ADDRESS_C_BITSTREAM_WRITE_ACCUMULATOR_TO_MEMORY                  0x0A8110 // c_bitstream::write_accumulator_to_memory
#define ADDRESS_C_BITSTREAM_FINISH_WRITING                               0x0A6030 // c_bitstream::finish_writing
#define ADDRESS_C_BITSTREAM_RESET                                        0x0A7BD0 // c_bitstream::reset
#define ADDRESS_C_BITSTREAM_READ_BOOL                                    0x0A82E0 // c_bitstream::read_bit_internal

// memory\data.cpp
#define ADDRESS_DATA_NEXT_ABSOLUTE_INDEX                                 0x0A8FE0 // data_next_absolute_index
#define ADDRESS_DATUM_DELETE                                             0x0A8F20 // datum_delete
#define ADDRESS_DATA_DELETE_ALL                                          0x0A8B80 // data_delete_all
#define ADDRESS_DATA_INITIALIZE                                          0x0A8920 // data_initialize

// memory\read_write_lock.cpp
#define ADDRESS_C_READ_WRITE_LOCK_SETUP                                  0x1B49B0 // c_read_write_lock::setup
#define ADDRESS_C_READ_WRITE_LOCK_WRITE_LOCK                             0x1B4A50 // c_read_write_lock::write_lock
#define ADDRESS_C_READ_WRITE_LOCK_WRITE_UNLOCK                           0x1B4B30 // c_read_write_lock::write_unlock

// multithreading\synchronization.cpp
#define ADDRESS_G_SYNCH_GLOBALS                                          0x3EE11C4 // g_synch_globals
#define ADDRESS_INTERNAL_SEMAPHORE_RELEASE                               0x094200 // internal_semaphore_release
#define ADDRESS_INTERNAL_SEMAPHORE_TAKE                                  0x0941A0 // internal_semaphore_take
#define ADDRESS_RELEASE_LOCKS_SAFE_FOR_CRASH_RELEASE                     0x094430 // release_locks_safe_for_crash_release

// multithreading\threads.cpp
#define ADDRESS_K_REGISTERED_THREAD_DEFINITIONS                          0xD3D5F8 // k_registered_thread_definitions
#define ADDRESS_G_THREAD_GLOBALS                                         0x1044AD0 // g_thread_globals
#define ADDRESS_G_THREAD_OWNING_DEVICE                                   0x49B1298 // g_thread_owning_device
#define ADDRESS_START_THREAD                                             0x0A5D20 // start_thread

// networking\delivery\network_channel.cpp
#define ADDRESS_C_NETWORK_CHANNEL_OPEN                                   0x00BE20 // c_network_channel::open
#define ADDRESS_C_NETWORK_CHANNEL_SEND_CONNECTION_ESTABLISHED            0x00BF80 // c_network_channel::send_connection_established

// networking\delivery\network_link.cpp
#define ADDRESS_C_NETWORK_LINK_GET_ASSOCIATED_CHANNEL                    0x006590 // c_network_link::get_associated_channel
#define ADDRESS_C_NETWORK_LINK_SEND_OUT_OF_BAND                          0x006800 // c_network_link::send_out_of_band

// networking\logic\life_cycle\life_cycle_handler_end_game_write_stats.cpp
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE_SESSION_START 0x04CB50 // c_life_cycle_state_handler_end_game_write_stats::update_session_start
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_END_GAME_WRITE_STATS_UPDATE_SESSION_END 0x04CBC0 // c_life_cycle_state_handler_end_game_write_stats::update_session_end

// networking\logic\life_cycle\life_cycle_state_handler.cpp
#define ADDRESS_C_LIFE_CYCLE_STATE_HANDLER_ALL_PEERS_HAVE_MAIN_MENU_READY 0x04BFF0 // c_life_cycle_state_handler::all_peers_have_main_menu_ready

// networking\logic\network_join.cpp
#define ADDRESS_G_NETWORK_JOIN_DATA                                      0x1039AF8 // g_network_join_data
#define ADDRESS_NETWORK_JOIN_FLUSH_JOIN_QUEUE                            0x02A640 // network_join_flush_join_queue

// networking\logic\network_life_cycle.cpp
#define ADDRESS_LIFE_CYCLE_GLOBALS                                       0x3EADFA8 // simulation_globals
#define ADDRESS_NETWORK_LIFE_CYCLE_END                                   0x02AC20 // network_life_cycle_end
#define ADDRESS_NETWORK_LIFE_CYCLE_CREATE_LOCAL_SQUAD                    0x02AD00 // network_life_cycle_create_local_squad

// networking\logic\network_session_interface.cpp
#define ADDRESS_SESSION_INTERFACE_GLOBALS                                0x3EAE0C0 // session_interface_globals
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE_SESSION                 0x02F410 // network_session_interface_update_session
#define ADDRESS_NETWORK_SESSION_INTERFACE_GET_LOCAL_USER_IDENTIFIER      0x003D50 // network_session_interface_get_local_user_identifier
#define ADDRESS_NETWORK_SESSION_CALCULATE_PEER_CONNECTIVITY              0x02E500 // network_session_calculate_peer_connectivity
#define ADDRESS_NETWORK_SESSION_INTERFACE_UPDATE                         0x02DC50 // network_session_interface_update

// networking\messages\network_message_gateway.cpp
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_RECEIVE_OUT_OF_BAND_PACKET     0x0231A0 // c_network_message_gateway::receive_out_of_band_packet
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_SEND_MESSAGE_DIRECTED          0x0232C0 // c_network_message_gateway::send_message_directed
#define ADDRESS_C_NETWORK_MESSAGE_GATEWAY_READ_PACKET_HEADER             0x0235C0 // c_network_message_gateway::read_packet_header

// networking\messages\network_message_handler.cpp
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_CONNECT_REFUSE          0x025AC0 // c_network_message_handler::handle_connect_refuse
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_JOIN_REFUSE             0x025660 // c_network_message_handler::handle_join_refuse
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_LEAVE_ACKNOWLEDGE       0x0256E0 // c_network_message_handler::handle_leave_acknowledge
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_VIEW_ESTABLISHMENT      0x0257B0 // c_network_message_handler::handle_view_establishment
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_PLAYER_ACKNOWLEDGE      0x025810 // c_network_message_handler::handle_player_acknowledge
#define ADDRESS_HANDLE_SYNCHRONOUS_UPDATE_CALL                           0x025860 // c_network_message_handler::handle_synchronous_update
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_PLAYBACK_CONTROL 0x0258E0 // c_network_message_handler::handle_synchronous_playback_control
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_ACTIONS     0x025990 // c_network_message_handler::handle_synchronous_actions
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_GAMESTATE   0x025A20 // c_network_message_handler::handle_synchronous_gamestate
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_DISTRIBUTED_GAME_RESULTS 0x025A70 // c_network_message_handler::handle_game_results
#define ADDRESS_C_NETWORK_MESSAGE_HANDLER_HANDLE_SYNCHRONOUS_CLIENT_READY 0x025950 // c_network_message_handler::handle_synchronous_client_ready

// networking\messages\network_message_queue.cpp
#define ADDRESS_C_NETWORK_MESSAGE_QUEUE_SEND_MESSAGE                     0x016BB0 // c_network_message_queue::send_message

// networking\messages\network_message_type_collection.cpp
#define ADDRESS_C_NETWORK_MESSAGE_TYPE_COLLECTION_DECODE_MESSAGE_HEADER  0x038830 // c_network_message_type_collection::decode_message_header

// networking\network_configuration.cpp
#define ADDRESS_G_NETWORK_CONFIGURATION                                  0x10383D0 // g_network_configuration
#define ADDRESS_G_NETWORK_CONFIGURATION_INITIALIZED                      0x1038282 // g_network_configuration_initialized

// networking\network_globals.cpp
#define ADDRESS_NETWORK_GLOBALS                                          0x3E81A44 // network_globals
#define ADDRESS_G_GAME_PORT                                              0xE9B7A0 // g_broadcast_port
#define ADDRESS_G_NETWORK_SESSION_MANAGER                                0x1039AD4 // g_network_session_manager

// networking\network_memory.cpp
#define ADDRESS_NETWORK_SHARED_MEMORY_GLOBALS                            0x1038284 // network_shared_memory_globals
#define ADDRESS_NETWORK_BASE_MEMORY_GLOBALS                              0x394BA58 // network_base_memory_globals

// networking\network_time.cpp
#define ADDRESS_NETWORK_TIME_GLOBALS                                     0x1038344 // network_time_globals

// networking\replication\replication_entity_manager_view.cpp
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_SET_STATE              0x020CA0 // c_replication_entity_manager_view::set_state
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_CLEAR_ENTITY_MASK      0x01F7A0 // c_replication_entity_manager_view::clear_entity_mask
#define ADDRESS_C_REPLICATION_ENTITY_MANAGER_VIEW_SET_ENTITY_MASK        0x01F760 // c_replication_entity_manager_view::set_entity_mask

// networking\session\network_managed_session.cpp
#define ADDRESS_ONLINE_SESSION_MANAGER_GLOBALS                           0x3EAB120 // online_session_manager_globals
#define ADDRESS_MANAGED_SESSION_COMPARE_ID                               0x028B40 // managed_session_compare_id
#define ADDRESS_MANAGED_SESSION_DELETE_SESSION_INTERNAL                  0x028C30 // managed_session_delete_session_internal

// networking\session\network_observer.cpp
#define ADDRESS_C_NETWORK_OBSERVER_HANDLE_CONNECT_REQUEST                0x010E30 // c_network_observer::handle_connect_request
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_INITIATE_CONNECTION  0x00F970 // c_network_observer::observer_channel_initiate_connection
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_SEND_MESSAGE         0x00F440 // c_network_observer::observer_channel_send_message
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_BACKLOGGED           0x00F880 // c_network_observer::observer_channel_backlogged
#define ADDRESS_C_NETWORK_OBSERVER_OBSERVER_CHANNEL_SET_WAITING_ON_BACKLOG 0x00F900 // c_network_observer::observer_channel_set_waiting_on_backlog
#define ADDRESS_C_NETWORK_OBSERVER_QUALITY_STATISTICS_GET_RATINGS        0x00EF30 // c_network_observer::quality_statistics_get_ratings
#define ADDRESS_C_NETWORK_OBSERVER_QUALITY_STATISTICS_REPORT_BADNESS     0x00F160 // c_network_observer::quality_statistics_report_badness

// networking\session\network_session.cpp
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PEER_CONNECT                    0x04B2E0 // c_network_session::handle_peer_connect
#define ADDRESS_C_NETWORK_SESSION_HANDLE_SESSION_DISBAND                 0x04B4D0 // c_network_session::handle_session_disband
#define ADDRESS_C_NETWORK_SESSION_HANDLE_SESSION_BOOT                    0x04B560 // c_network_session::handle_session_boot
#define ADDRESS_C_NETWORK_SESSION_HANDLE_HOST_DECLINE                    0x04B5F0 // c_network_session::handle_host_decline
#define ADDRESS_C_NETWORK_SESSION_CHANNEL_IS_AUTHORITATIVE               0x0227A0 // c_network_session::channel_is_authoritative
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PLAYER_REFUSE                   0x04B7C0 // c_network_session::handle_player_refuse
#define ADDRESS_C_NETWORK_SESSION_HANDLE_PARAMETERS_UPDATE               0x04B3D0 // c_network_session::handle_parameters_update
#define ADDRESS_C_NETWORK_SESSION_DISCONNECT                             0x021CC0 // c_network_session::disconnect
#define ADDRESS_C_NETWORK_SESSION_IS_PEER_JOINING_THIS_SESSION           0x0224F0 // c_network_session::is_peer_joining_this_session
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_CREATING                     0x03E800 // _dynamic_initializer_for__module_base___5
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_JOINING                      0x03E8B0 // c_network_session::idle_peer_joining
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_JOIN_ABORT                   0x03EA00 // c_network_session::idle_peer_join_abort
#define ADDRESS_C_NETWORK_SESSION_IDLE_PEER_LEAVING                      0x03EA60 // c_network_session::idle_peer_leaving
#define ADDRESS_C_NETWORK_SESSION_GET_MAXIMUM_PLAYER_COUNT               0x022E80 // c_network_session::get_maximum_player_count
#define ADDRESS_C_NETWORK_SESSION_CHECK_TO_SEND_TIME_SYNCHRONIZATION     0x022B50 // c_network_session::check_to_send_time_synchronization
#define ADDRESS_C_NETWORK_SESSION_IDLE_OBSERVER_STATE                    0x03EAC0 // c_network_session::idle_observer_state
#define ADDRESS_C_NETWORK_SESSION_PEER_REQUEST_PLAYER_ADD                0x021EF0 // c_network_session::peer_request_player_add
#define ADDRESS_C_NETWORK_SESSION_INITIATE_LEAVE_PROTOCOL                0x021C30 // c_network_session::initiate_leave_protocol

// networking\session\network_session_manager.cpp
#define ADDRESS_C_NETWORK_SESSION_MANAGER_GET_SESSION                    0x027C80 // c_network_session_manager::get_session

// networking\session\network_session_membership.cpp
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_SECURE_ADDRESS 0x0312B0 // c_network_session_membership::get_peer_from_secure_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ADD_PEER                    0x030DE0 // c_network_session_membership::add_peer
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_INCOMING_ADDRESS 0x031230 // c_network_session_membership::get_peer_from_incoming_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_IDLE                        0x0328E0 // c_network_session_membership::idle
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ALL_PEERS_ESTABLISHED       0x020E50 // c_network_session_membership::all_peers_established
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PEER                 0x030E50 // c_network_session_membership::remove_peer
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_FIND_PLAYER_IN_PLAYER_ADD_QUEUE 0x032D90 // c_network_session_membership::find_player_in_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_FROM_PLAYER_ADD_QUEUE 0x032C00 // c_network_session_membership::remove_player_from_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_SET_PLAYER_PROPERTIES       0x031C10 // c_network_session_membership::set_player_properties
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_REMOVE_PLAYER_INTERNAL      0x031B80 // c_network_session_membership::remove_player_internal
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PLAYER_FROM_IDENTIFIER  0x0318B0 // c_network_session_membership::get_player_from_identifier
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_ADD_PLAYER_TO_PLAYER_ADD_QUEUE 0x032B40 // c_network_session_membership::add_player_to_player_add_queue
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_GET_PEER_FROM_OBSERVER_CHANNEL 0x031180 // c_network_session_membership::get_peer_from_observer_channel
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_HOST_EXISTS_AT_INCOMING_ADDRESS 0x031370 // c_network_session_membership::host_exists_at_incoming_address
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_HANDLE_MEMBERSHIP_UPDATE    0x031DD0 // c_network_session_membership::handle_membership_update
#define ADDRESS_C_NETWORK_SESSION_MEMBERSHIP_PEER_PROPERTY_FLAG_TEST     0x032890 // c_network_session_membership::peer_property_flag_test

// networking\session\network_session_parameters.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETERS_CHECK_TO_SEND_UPDATES       0x01A390 // c_network_session_parameters::check_to_send_updates
#define ADDRESS_C_NETWORK_SESSION_PARAMETERS_CHECK_TO_SEND_CHANGE_REQUESTS 0x01A7B0 // c_network_session_parameters::check_to_send_change_requests

// networking\session\network_session_parameters_session.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_SESSION_SIZE_SET_MAX_PLAYER_COUNT 0x02D670 // c_network_session_parameter_session_size::set_max_player_count
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_SESSION_MODE_SET             0x02D820 // c_network_session_parameter_session_mode::set

// networking\session\network_session_parameters_ui.cpp
#define ADDRESS_C_NETWORK_SESSION_PARAMETER_UI_GAME_MODE_REQUEST_CHANGE  0x03B4D0 // c_network_session_parameter_ui_game_mode::request_change

// networking\transport\transport.cpp
#define ADDRESS_TRANSPORT_GLOBALS                                        0x4EBE948 // transport_globals
#define ADDRESS_TRANSPORT_INITIALIZE                                     0x0039C0 // transport_initialize
#define ADDRESS_TRANSPORT_STARTUP                                        0x003A50 // transport_startup

// networking\transport\transport_address.cpp
#define ADDRESS_TRANSPORT_ADDRESS_EQUIVALENT                             0x0077D0 // transport_address_equivalent

// networking\transport\transport_security.cpp
#define ADDRESS_TRANSPORT_SECURITY_GLOBALS                               0x4EBE9D0
#define ADDRESS_G_SESSION_SECURE_ADDRESS                                 0x49C1060 // g_session_secure_address2

// networking\transport\transport_shim.cpp
#define ADDRESS_G_XNET_SHIM_TABLE                                        0x49C0260 // g_transport_address_mapping

// objects\object_scripting.cpp
#define ADDRESS_OBJECT_SCRIPTING_CLEAR_ALL_FUNCTION_VARIABLES            0x475CF0 // object_scripting_clear_all_function_variables

// objects\object_types.cpp
#define ADDRESS_OBJECT_TYPE_DEFINITIONS                                  0xEB2448 // object_type_definitions

// objects\objects.cpp
#define ADDRESS_OBJECT_TRY_AND_GET_AND_VERIFY_TYPE                       0x403000 // object_try_and_get_and_verify_type
#define ADDRESS_OBJECT_WAKE                                              0x3FBE70 // object_wake
#define ADDRESS_OBJECT_SET_REQUIRES_MOTION                               0x403E50 // object_set_requires_motion
#define ADDRESS_OBJECT_NEEDS_RIGID_BODY_UPDATE                           0x3FE620 // object_needs_rigid_body_update
#define ADDRESS_ATTACHMENTS_UPDATE                                       0x409070 // attachments_update
#define ADDRESS_OBJECT_COMPUTE_NODE_MATRICES                             0x4056A0 // object_compute_node_matrices
#define ADDRESS_OBJECT_NEW                                               0x3FCEE0 // object_new
#define ADDRESS_OBJECT_SET_GARBAGE                                       0x403C50 // object_set_garbage
#define ADDRESS_OBJECT_SET_POSITION_INTERNAL                             0x3FBF90 // object_set_position_internal
#define ADDRESS_OBJECT_GET_ORIGIN_INTERPOLATED                           0x401390 // object_get_origin_interpolated
#define ADDRESS_OBJECT_DELETE                                            0x3FE0C0 // object_delete
#define ADDRESS_OBJECT_TRY_AND_GET_MULTIPLAYER                           0x4097C0 // object_try_and_get_multiplayer

// objects\scenery.cpp
#define ADDRESS_SCENERY_ANIMATION_IDLE                                   0x490200 // scenery_animation_idle

// online\online_lsp.cpp
#define ADDRESS_G_ONLINE_LSP_MANAGER                                     0x394B5C0 // g_online_lsp_manager
#define ADDRESS_C_ONLINE_LSP_MANAGER_GO_INTO_CRASH_MODE                  0x005E80 // c_online_lsp_manager::go_into_crash_mode

// physics\collisions.cpp
#define ADDRESS_COLLISION_TEST_VECTOR_TARGET                             0x1A1F80 // collision_test_vector

// physics\collisions.h
#define ADDRESS_COLLISION_TEST_FOR_PROJECTILES_FLAGS                     0x40EAB3C // _collision_test_for_projectiles_flags
#define ADDRESS_COLLISION_TEST_PATHFINDING_FLAGS                         0x40EAAD4 // _collision_test_pathfinding_flags

// physics\havok_component.cpp
#define ADDRESS_G_HAVOK_COMPONENTS                                       0x1046CDC // g_havok_component_data
#define ADDRESS_C_HAVOK_COMPONENT_FORCE_ACTIVATE                         0x127C30 // c_havok_component::force_activate

// rasterizer\dx9\rasterizer_dx9_main.cpp
#define ADDRESS_C_RASTERIZER_SET_SAMPLER_FILTER_MODE_CUSTOM_DEVICE_NO_CACHE 0x25BFF0 // c_rasterizer::set_sampler_filter_mode_custom_device_no_cache

// rasterizer\rasterizer.cpp
#define ADDRESS_RASTERIZER_RENDER_GLOBALS                                0xEAC130 // c_rasterizer::render_globals
#define ADDRESS_RASTERIZER_G_LAST_VIEWPORT                               0x4ECF2B8 // c_rasterizer::g_last_viewport
#define ADDRESS_RASTERIZER_G_CURRENT_VERTEX_DECLARATION                  0x23390EC // c_rasterizer::g_current_vertex_declaration
#define ADDRESS_RASTERIZER_G_DEVICE                                      0x2339330 // c_rasterizer::g_device
#define ADDRESS_RASTERIZER_G_CURRENT_INDEX_BUFFER                        0x23390C8 // c_rasterizer::g_current_index_buffer
#define ADDRESS_RASTERIZER_X_LAST_SAMPLER_FILTER_MODES                   0xEAADE8 // c_rasterizer::x_last_sampler_filter_modes
#define ADDRESS_RASTERIZER_X_LAST_RENDER_STATE_VALUE                     0x23390DC // c_rasterizer::x_last_render_state_value
#define ADDRESS_RASTERIZER_G_CURRENT_ALPHA_BLEND_MODE                    0x23390F4 // c_rasterizer::g_current_alpha_blend_mode
#define ADDRESS_C_RASTERIZER_GET_DISPLAY_PIXEL_BOUNDS                    0x257FE0 // c_rasterizer::get_display_pixel_bounds
#define ADDRESS_C_RASTERIZER_GET_DISPLAY_TITLE_SAFE_PIXEL_BOUNDS         0x258050 // c_rasterizer::get_display_title_safe_pixel_bounds
#define ADDRESS_C_RASTERIZER_RESTORE_LAST_VIEWPORT                       0x2588F0 // c_rasterizer::restore_last_viewport
#define ADDRESS_C_RASTERIZER_SET_DEPTH_STENCIL_SURFACE                   0x25E7B0 // c_rasterizer::set_depth_stencil_surface
#define ADDRESS_C_RASTERIZER_SET_VERTEX_SHADER                           0x25C5B0 // c_rasterizer::set_vertex_shader
#define ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER                            0x25C4E0 // c_rasterizer::set_pixel_shader
#define ADDRESS_C_RASTERIZER_DRAW_PRIMITIVE_UP                           0x260B10 // c_rasterizer::draw_primitive_up
#define ADDRESS_C_RASTERIZER_SET_PIXEL_SHADER_CONSTANT                   0x291410 // c_rasterizer::set_pixel_shader_constant
#define ADDRESS_C_RASTERIZER_SET_Z_BUFFER_MODE                           0x25B090 // c_rasterizer::set_z_buffer_mode
#define ADDRESS_C_RASTERIZER_SET_CULL_MODE                               0x25BE20 // c_rasterizer::set_cull_mode
#define ADDRESS_C_RASTERIZER_SET_ALPHA_BLEND_MODE_CUSTOM_DEVICE_NO_CACHE 0x25AC40 // c_rasterizer::set_alpha_blend_mode_custom_device_no_cache
#define ADDRESS_C_RASTERIZER_BEGIN_FRAME                                 0x2598B0 // c_rasterizer::begin_frame
#define ADDRESS_C_RASTERIZER_SETUP_TARGETS_SIMPLE                        0x25D200 // c_rasterizer::setup_targets_simple
#define ADDRESS_C_RASTERIZER_END_FRAME                                   0x259E40 // c_rasterizer::end_frame

// rasterizer\rasterizer_resource_definitions.cpp
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_LIGHTING_VERTEX_DECLS         0xD50A88 // c_vertex_declaration_table::lighting_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_TRANSFER_VERTEX_DECLS         0xD4FA80 // c_vertex_declaration_table::transfer_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_OTHER_VERTEX_DECLS            0xD50280 // c_vertex_declaration_table::other_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_BASE_VERTEX_DECLS             0xEACD38 // c_vertex_declaration_table::base_vertex_decls
#define ADDRESS_C_VERTEX_DECLARATION_TABLE_M_VERTEX_DECLARATIONS         0x23645B0 // c_vertex_declaration_table::m_vertex_declarations

// render\render.cpp
#define ADDRESS_C_RENDER_GLOBALS_M_FRAME_INDEX                           0xEAC1AC // c_render_globals::m_frame_index

// render\render_cameras.cpp
#define ADDRESS_RENDER_CAMERA_VIEW_TO_SCREEN                             0x2948D0 // render_camera_view_to_screen

// render\views\render_view.cpp
#define ADDRESS_C_VIEW_G_VIEW_STACK_TOP                                  0xEAC158 // c_view::g_view_stack_top
#define ADDRESS_C_VIEW_G_VIEW_STACK                                      0x233FD70 // c_view::g_view_stack
#define ADDRESS_C_PLAYER_VIEW_X_CURRENT_PLAYER_VIEW                      0x233FD80 // c_player_view::x_current_player_view

// scenario\scenario_map_variant.cpp
#define ADDRESS_C_MAP_VARIANT_C_MAP_VARIANT                              0x0AB2F0 // c_map_variant::c_map_variant
#define ADDRESS_C_MAP_VARIANT_CREATE_DEFAULT                             0x0AB380 // c_map_variant::create_default

// shell\shell.cpp
#define ADDRESS_SHELL_APPLICATION_PAUSED                                 0x1038280 // shell_application_paused

// shell\shell_windows.cpp
#define ADDRESS_G_WINDOWS_PARAMS                                         0x10382B0 // window_globals

// simulation\game_interface\simulation_game_events.cpp
#define ADDRESS_SIMULATION_EVENT_GENERATE_FOR_REMOTE_PEERS_CALL          0x07E490 // simulation_event_generate_for_remote_peers
#define ADDRESS_SIMULATION_EVENT_BUILD_ENTITY_REFERENCE_INDICES          0x07E610 // simulation_event_build_entity_reference_indices

// simulation\simulation.cpp
#define ADDRESS_SIMULATION_GLOBALS                                       0x4EBEBA8 // simulation_globals

// simulation\simulation_event_handler.cpp
#define ADDRESS_C_SIMULATION_EVENT_HANDLER_SEND_EVENT                    0x03A900 // c_simulation_event_handler::send_event

// simulation\simulation_queue_entities.cpp
#define ADDRESS_SIMULATION_QUEUE_ENTITY_DELETION_INSERT                  0x055B80 // simulation_queue_entity_deletion_insert

// simulation\simulation_view.cpp
#define ADDRESS_C_SIMULATION_VIEW_SET_VIEW_ESTABLISHMENT                 0x0335B0 // c_simulation_view::set_view_establishment

// simulation\simulation_watcher.cpp
#define ADDRESS_C_SIMULATION_WATCHER_GET_MACHINE_INDEX_BY_IDENTIFIER     0x015620 // c_simulation_watcher::get_machine_index_by_identifier

// tag_files\files.cpp
#define ADDRESS_FILE_REFERENCE_ADD_DIRECTORY                             0x0A5690 // file_reference_add_directory
#define ADDRESS_FILE_REFERENCE_SET_NAME                                  0x0A5710 // file_reference_set_name
#define ADDRESS_FILE_CREATE_PARENT_DIRECTORIES_IF_NOT_PRESENT            0x0A58E0 // file_create_parent_directories_if_not_present
#define ADDRESS_FILE_REFERENCE_GET_NAME                                  0x0A5790 // file_reference_get_name

// tag_files\files_windows.cpp
#define ADDRESS_FILE_WRITE                                               0x0A47D0 // file_write
#define ADDRESS_FILE_GET_EOF                                             0x0A46D0 // file_get_eof
#define ADDRESS_FIND_FILES_NEXT                                          0x0A4B40 // find_files_next
#define ADDRESS_FIND_FILES_END                                           0x0A4AF0 // find_files_end
#define ADDRESS_FILE_DELETE                                              0x0A3FD0 // file_delete

// tag_files\tag_resource_cache_control.cpp
#define ADDRESS_C_TAG_RESOURCE_CACHE_CONTROLLER_ACQUIRE_PAGE_RESERVATION 0x247C40 // c_tag_resource_cache_controller::acquire_page_reservation
#define ADDRESS_C_TAG_RESOURCE_CACHE_CONTROLLER_RELEASE_PAGE_RESERVATION_FORCE 0x247D30 // c_tag_resource_cache_controller::release_page_reservation_force

// text\draw_string.cpp
#define ADDRESS__VFTABLE                                                 0xD80A58 // ?_7c_draw_string
#define ADDRESS_C_DRAW_STRING_DRAW_MORE                                  0x16CBE0 // c_draw_string::draw_more
#define ADDRESS_M_CHARACTER_CACHE_VFTABLE                                0xD89148 // ?_7c_simple_font_draw_string
#define ADDRESS_M_CHARACTER_CACHE                                        0x2AB680 // ?0c_simple_font_draw_string
#define ADDRESS_M_RENDER_DATA_VFTABLE                                    0xD85DC8 // ?_7c_rasterizer_draw_string
#define ADDRESS_DRAW_STRING_GET_GLYPH_SCALING_FOR_DISPLAY_SETTINGS       0x16DC30 // draw_string_get_glyph_scaling_for_display_settings

// text\font_cache.cpp
#define ADDRESS_G_INTERNAL_FONT_CACHE_GLOBALS                            0x4067900 // g_internal_font_cache_globals
#define ADDRESS_M_LOCKED_VFTABLE                                         0xD80A3C // ?_7c_font_cache_mt_safe
#define ADDRESS_FONT_CACHE_NEW                                           0x16A880 // font_cache_new

// text\font_loading.cpp
#define ADDRESS_G_FONT_GLOBALS                                           0x3F863D4 // g_font_globals

// text\font_package_cache.cpp
#define ADDRESS_FONT_PACKAGE_GET                                         0x16A370 // font_package_get
#define ADDRESS_FONT_PACKAGE_GET_CHARACTER                               0x16B9C0 // font_package_get_character
#define ADDRESS_PACKAGE_TABLE_SEARCH_FUNCTION                            0x16B980 // package_table_search_function
#define ADDRESS_FONT_PACKAGE_CACHE_NEW                                   0x16A050 // font_package_cache_new

// units\bipeds.cpp
#define ADDRESS_BIPED_CALCULATE_MELEE_AIMING                             0x4409F0 // biped_calculate_melee_aiming

// units\units.cpp
#define ADDRESS_UNIT_SET_ACTIVELY_CONTROLLED                             0x423010 // unit_set_actively_controlled
#define ADDRESS_UNIT_DROP_ITEM                                           0x426500 // unit_drop_item
#define ADDRESS_UNIT_CONTROL                                             0x41BA10 // unit_control
#define ADDRESS_UNIT_INVENTORY_GET_WEAPON                                0x422E60 // unit_inventory_get_weapon

// visibility\visibility_collection.cpp
#define ADDRESS_G_VISIBILITY_GLOBALS_KEEPER                              0x40BB4A8 // g_visibility_globals_keeper
#define ADDRESS_VISIBILITY_VOLUME_TEST_SPHERE                            0x1CCE30 // visibility_volume_test_sphere

// engine version specific lengths
#define LENGTH_UNIT_HANDLE_EQUIPMENT_ENERGY_COST                         0x169
#define LENGTH_PLAYER_CAN_USE_CONSUMABLE                                 0x62
#define LENGTH_GAME_ENGINE_SHOULD_SPAWN_PLAYER                           0xF0
#define LENGTH_GAME_ENGINE_UPDATE_ROUND_CONDITIONS                       0x111
#define LENGTH_OBJECT_SET_DAMAGE_OWNER                                   0x75
#define LENGTH_HF2P_GAME_UPDATE_NOP                                      65

#endif // ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
