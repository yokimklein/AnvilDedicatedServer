#pragma once
#include "..\transport\transport_address.h"

enum e_network_message_type
{
	_network_message_type_ping,
	_network_message_type_pong,
	//_network_message_type_broadcast_search, // saber removed most matchmaking parts of networking
	//_network_message_type_broadcast_reply,
	_network_message_type_connect_request,
	_network_message_type_connect_refuse,
	_network_message_type_connect_establish,
	_network_message_type_connect_closed,
	_network_message_type_join_request,
	_network_message_type_peer_connect,
	_network_message_type_join_abort,
	_network_message_type_join_refuse,
	_network_message_type_leave_session,
	_network_message_type_leave_acknowledge,
	_network_message_type_session_disband,
	_network_message_type_session_boot,
	_network_message_type_host_decline,
	_network_message_type_peer_establish,
	_network_message_type_time_synchronize,
	_network_message_type_membership_update,
	_network_message_type_peer_properties,
	_network_message_type_delegate_leadership,
	_network_message_type_boot_machine,
	_network_message_type_player_add,
	_network_message_type_player_refuse,
	_network_message_type_player_remove,
	_network_message_type_player_properties,
	_network_message_type_parameters_update,
	_network_message_type_parameters_request,
	_network_message_type_view_establishment,
	_network_message_type_player_acknowledge,
	_network_message_type_synchronous_update,
	_network_message_type_synchronous_playback_control,
	_network_message_type_synchronous_actions,
	_network_message_type_synchronous_acknowledge,
	_network_message_type_synchronous_gamestate,
	_network_message_type_synchronous_client_ready,
	_network_message_type_game_results,
	_network_message_type_test,

	k_network_message_type_count
};

struct s_network_message
{
	// there's a good chance these names are wrong
	long channel_identifier; // transport_secure_identifier?
	long next_update_number; // time sent?
	long chunk_size;
	long incremental_update_number; // enum? -1 == complete, else incremental
	void* baseline_checksum;
};

struct s_network_message_ping : s_network_message
{

};

struct s_network_message_pong : s_network_message
{

};
/*
struct s_network_message_broadcast_search : s_network_message
{

};

struct s_network_message_broadcast_reply : s_network_message
{

};
*/
struct s_network_message_connect_request : s_network_message
{

};

struct s_network_message_connect_refuse : s_network_message
{

};

struct s_network_message_connect_establish : s_network_message
{

};

struct s_network_message_connect_closed : s_network_message
{

};

struct s_network_message_join_request : s_network_message
{

};

struct s_network_message_peer_connect : s_network_message
{

};

struct s_network_message_join_abort : s_network_message
{

};

struct s_network_message_join_refuse : s_network_message
{

};

struct s_network_message_leave_session : s_network_message
{

};

struct s_network_message_leave_acknowledge : s_network_message
{

};

struct s_network_message_session_disband : s_network_message
{

};

struct s_network_message_session_boot : s_network_message
{

};

struct s_network_message_time_synchronize : s_network_message
{

};

class c_network_message_type_collection
{
public:
	// char handle_ping(c_network_message_handler* message_handler, s_transport_address outgoing_address, s_network_message_ping* message_ping);

private:
	
};