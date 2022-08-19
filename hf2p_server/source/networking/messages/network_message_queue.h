#pragma once
#include <windows.h>
#include "..\..\memory\sliding_window.h"
#include "network_message_type_collection.h"
#include "..\delivery\network_connection.h"

class c_network_message_queue : c_network_channel_client
{
	public:
		bool has_channel_been_used();
		void send_message(e_network_message_type message_type, long message_size, s_network_message* message);
	
		struct s_outgoing_fragment_record
		{
			byte flags;
			byte fragment_size_bytes;
			uint16_t fragment_size_bits;
			int32_t packet_sequence_number;
			s_outgoing_fragment_record* fragment;
		};

		struct s_incoming_fragment_record
		{
			byte flags;
			byte fragment_size_bytes;
			uint16_t fragment_size_bits;
			s_incoming_fragment_record* fragment;
			byte __data[32];
		};

		byte __data8[4];
		const char* m_name;
		c_network_message_type_collection* m_message_types;
		c_sliding_window m_outgoing_window;
		s_outgoing_fragment_record* m_outgoing_fragments[2];
		c_sliding_window m_incoming_window;
		s_incoming_fragment_record* m_incoming_fragments[2];
		int32_t m_outgoing_payload_bytes;
		int32_t m_incoming_payload_bytes;
};
static_assert(sizeof(c_network_message_queue) == 0x64);