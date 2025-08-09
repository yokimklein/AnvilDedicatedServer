#pragma once
#include <cseries\cseries.h>
#include <memory\bitstream.h>
#include <networking\logic\network_join.h>

constexpr ulong k_network_message_maximum_size = 0x40000;

enum e_network_message_type
{
	_network_message_ping,
	_network_message_pong,
	//_network_message_broadcast_search, // removed in ms29
	//_network_message_broadcast_reply, // removed in ms29
	_network_message_connect_request,
	_network_message_connect_refuse,
	_network_message_connect_establish,
	_network_message_connect_closed,
	_network_message_join_request,
	_network_message_peer_connect,
	_network_message_join_abort,
	_network_message_join_refuse,
	_network_message_leave_session,
	_network_message_leave_acknowledge,
	_network_message_session_disband,
	_network_message_session_boot,
	_network_message_host_decline,
	_network_message_peer_establish,
	_network_message_time_synchronize,
	_network_message_membership_update,
	_network_message_peer_properties,
	_network_message_delegate_leadership,
	_network_message_boot_machine,
	_network_message_player_add,
	_network_message_player_refuse,
	_network_message_player_remove,
	_network_message_player_properties,
	_network_message_parameters_update,
	_network_message_parameters_request,
	_network_message_view_establishment,
	_network_message_player_acknowledge,
	_network_message_synchronous_update,
	_network_message_synchronous_playback_control,
	_network_message_synchronous_actions,
	_network_message_synchronous_acknowledge,
	_network_message_synchronous_gamestate,
	_network_message_game_results,
	_network_message_synchronous_client_ready,
	_network_message_test,

	k_network_message_type_count
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
	const char* get_message_type_name(e_network_message_type message_type) const;
	void register_message_type(
		e_network_message_type message_type,
		char const* message_type_name,
		long flags,
		long message_size,
		long message_size_maximum,
		encode_t* encode_function,
		decode_t* decode_function,
		compare_t* compare_function,
		dispose_t* dispose_function);

	void encode_message_header(c_bitstream* stream, e_network_message_type message_type, long message_storage_size);
	bool decode_message_header(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size) const;
	bool __cdecl decode_message(c_bitstream* packet, e_network_message_type* message_type, long* message_storage_size, void* message_storage) const;
	void __cdecl dispose_message(e_network_message_type message_type, long message_storage_size, void* message_storage) const;

protected:
	s_network_message_type m_message_types[k_network_message_type_count];
};
static_assert(sizeof(c_network_message_type_collection) == 0x534);

const char* network_message_join_refuse_get_reason_string(e_network_join_refuse_reason refuse_reason);