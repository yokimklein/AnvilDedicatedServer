#pragma once
#include "..\..\cseries\cseries.h"
#include "..\..\memory\bitstream.h"
#include "..\logic\network_join.h"

enum e_network_message_type : long
{
	_network_message_type_ping,
	_network_message_type_pong,
	//_network_message_type_broadcast_search, // removed in ms29
	//_network_message_type_broadcast_reply, // removed in ms29
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
	_network_message_type_game_results,
	_network_message_type_synchronous_client_ready,
	_network_message_type_test,

	k_network_message_type_count
};

struct s_network_message
{
};

class c_network_message_type_collection
{
	using encode_t = void __cdecl(c_bitstream*, long, void const*);
	using decode_t = bool __cdecl(c_bitstream*, long, void*);
	using compare_t = bool __cdecl(long, void*, void*);
	using dispose_t = void __cdecl(long, void*);

	struct s_network_message_type
	{
		bool initialized;
		const char* message_type_name;
		dword_flags flags;
		long message_size;
		long message_size_maximum;
		encode_t* encode_function;
		decode_t* decode_function;
		compare_t* compare_function;
		dispose_t* dispose_function;
	};
	static_assert(sizeof(s_network_message_type) == 0x24);

public:
	const char* get_message_type_name(e_network_message_type message_type);
	void register_message_type(
		e_network_message_type message_type,
		char const* message_type_name,
		long flags,
		long message_size,
		long message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function
	);
protected:
	s_network_message_type m_message_types[k_network_message_type_count];
};

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason);