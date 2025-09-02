#pragma once
#include <cseries\cseries.h>
#include <memory\sliding_window.h>
#include <networking\network_external.h>

enum e_network_message_type;
class c_network_message_type_collection;
class c_network_message_queue : public c_network_channel_client
{
public:
	void send_message(e_network_message_type message_type, long raw_message_size, void const* raw_message_payload);
	bool has_channel_been_used() const;
	
	struct s_outgoing_message_description
	{
		e_network_message_type message_type;
		word message_number;
		word message_raw_size_bytes;
		word message_encoded_size_bits;
		dword timestamp;
	};
	static_assert(sizeof(s_outgoing_message_description) == 0x10);

	struct s_outgoing_fragment_record
	{
		byte flags;
		byte fragment_size_bytes;
		word fragment_size_bits;
		long packet_sequence_number;
		s_outgoing_fragment_record* next_fragment;
		s_outgoing_message_description message_description;
		__pragma(warning(disable : 4200)) byte payload[];
	};
	static_assert(sizeof(s_outgoing_fragment_record) == 0x1C);

	struct s_incoming_fragment_record
	{
		byte flags;
		byte fragment_size_bytes;
		word fragment_size_bits;
		long sequence_number;
		s_incoming_fragment_record* next_fragment;
		__pragma(warning(disable : 4200)) byte payload[];
	};
	static_assert(sizeof(s_incoming_fragment_record) == 0xC);

	bool m_allocated;
	bool m_connection_lost;
	bool m_first_fragment_reserved;
	bool m_second_fragment_sent;
	bool m_establishment_received;
	char const* m_channel_name;

	c_network_message_type_collection const* m_message_types;

	c_sliding_window m_outgoing_window;
	s_outgoing_fragment_record* m_outgoing_fragment_list_head;
	s_outgoing_fragment_record* m_outgoing_fragment_list_tail;

	c_sliding_window m_incoming_window;
	s_incoming_fragment_record* m_incoming_fragment_list_head;
	s_incoming_fragment_record* m_incoming_fragment_list_tail;

	long m_outgoing_payload_bytes;
	long m_incoming_payload_bytes;
};
static_assert(sizeof(c_network_message_queue) == 0x64);