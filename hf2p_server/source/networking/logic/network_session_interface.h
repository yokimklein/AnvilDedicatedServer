#pragma once
#include <text\unicode.h>
#include <cseries\cseries.h>
#include <game\player_configuration.h>
#include <game\player_mapping.h>
#include <networking\session\network_session_membership.h>
#include <networking\session\network_session_parameters_saved_film_game_options.h>

#pragma pack(push, 1)
struct s_network_session_interface_user
{
    long state;
    s_player_identifier identifier;
    long controller_index;
    s_player_configuration player_data;
    c_enum<e_output_user_index, long, _output_user_index0, k_number_of_output_users> output_user_index;
    c_static_string<64> override_hopper_directory;
    long player_voice_settings;
    byte __data78[0x20];
};
static_assert(sizeof(s_network_session_interface_user) == 0xBE8);

class c_network_session_manager;
struct s_network_session_interface_globals
{
    bool initialized;
    byte : 8;
    c_static_wchar_string<16> machine_name; // 0x3EAE0C2
    c_static_wchar_string<32> session_name; // 0x3EAE0E2
    bool has_live_connection_info; // May no longer exist and just be padding
    byte : 8;
    //s_transport_qos_result qos_result;
    //long bandwidth_bps;
    //long max_machine_count;
    dword peer_status_flags;
    //short ready_hopper_identifier;
    //byte : 8;
    //byte : 8;
    long game_start_error;
    //bool was_guide_opened_during_a_multiplayer_session;
    //byte : 8;
    //byte : 8;
    //byte : 8;
    long map_id;
    c_enum<e_network_session_map_status, long, _network_session_map_status_none, k_network_session_map_status_count> current_map;
    long current_map_progress_percentage;
    s_network_session_interface_user users[k_number_of_output_users];
    qword game_instance;
    long scenario_type;
    c_static_string<128> scenario_path;
    short campaign_insertion_point;
    short map_status;
    real map_progress;
    s_saved_film_description local_specific_film;
    dword local_specific_film_time;
    c_static_array<long, 3> session_connection_identifiers;
    c_static_array<long, 3> session_update_times;
    c_static_array<long, 3> session_connection_update_numbers;
    bool session_variant_has_teams[3];
    bool session_variant_has_sve_teams[3];
    bool session_variant_observers_allowed[3];
    byte __data5EF1[3];
    int session_variant_maximum_team_count[3];
    c_network_session_manager* session_manager;
    byte : 8;
    byte : 8;
    byte : 8;
    byte : 8;
};
static_assert(sizeof(s_network_session_interface_globals) == 0x3410);
static_assert(0x02 == OFFSETOF(s_network_session_interface_globals, machine_name));
static_assert(0x22 == OFFSETOF(s_network_session_interface_globals, session_name));
static_assert(0x64 == OFFSETOF(s_network_session_interface_globals, peer_status_flags)); // 0x3EAE124
static_assert(0x68 == OFFSETOF(s_network_session_interface_globals, game_start_error)); // 0x3EAE128
static_assert(0x6C == OFFSETOF(s_network_session_interface_globals, map_id));
static_assert(0x70 == OFFSETOF(s_network_session_interface_globals, current_map)); // 0x3EAE130
static_assert(0x74 == OFFSETOF(s_network_session_interface_globals, current_map_progress_percentage)); // 0x3EAE134
static_assert(0x3018 == OFFSETOF(s_network_session_interface_globals, game_instance)); // 0x3EB10D8
#pragma pack(pop)

extern s_network_session_interface_globals& session_interface_globals; // 0x3EAE0C0

class c_network_session;
struct s_network_session_peer;
struct s_player_identifier;
struct s_network_session_peer_connectivity;

void __fastcall network_session_update_peer_properties(c_network_session* session, s_network_session_peer* peer);
void network_session_check_properties(c_network_session* session);
void network_session_update_team_indices(c_network_session* session, bool variant_has_teams, bool previous_update_variant_had_teams, bool observer_allowed, bool sve_teams, long maximum_team_count);
void __fastcall network_session_interface_update_session(c_network_session* session);
bool __fastcall network_session_interface_get_local_user_identifier(s_player_identifier* player_identifier);
void __fastcall network_session_calculate_peer_connectivity(c_network_session* session, s_network_session_peer_connectivity* peer_connectivity);
void network_session_interface_set_local_name(wchar_t const* machine_name, wchar_t const* session_name);
bool network_session_interface_get_squad_session(c_network_session** out_session);
void __cdecl network_session_interface_update();