#pragma once
#include <cseries\cseries.h>
#include <memory\sliding_window.h>
#include <networking\network_external.h>

class c_network_connection : public c_network_channel_client
{
public:
	struct s_connection_incoming_packet
	{
		word_flags flags;
	};
	static_assert(sizeof(s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		dword timestamp;
		long size_on_wire;
		dword round_trip_msec;
		short unretired_window_size;
		word_flags flags;
	};
	static_assert(sizeof(s_connection_outgoing_packet) == 0x10);

private:
	bool m_allocated;
	bool m_connection_lost;
	char const* m_channel_name;
	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;
	long m_incoming_window_size_bytes;
	long m_outgoing_window_size_bytes;
	c_static_array<s_connection_incoming_packet, 128> m_incoming_packets;
	c_static_array<s_connection_outgoing_packet, 128> m_outgoing_packets;
	long m_highest_outgoing_acknowledged_sequence_number;
	long m_highest_incoming_acknowledged_sequence_number;
	long m_highest_incoming_known_retired_sequence_number;
	bool m_incoming_window_fully_retired;
	bool m_outgoing_window_fully_retired;
	long m_current_timeout_msec;
};
static_assert(sizeof(c_network_connection) == 0x960);