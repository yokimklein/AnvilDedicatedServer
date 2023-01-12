#pragma once
#include "..\..\cseries\cseries.h"
#include "..\..\memory\sliding_window.h"

enum e_network_read_result;
enum e_network_channel_closure_reason;
class c_bitstream;
class c_network_channel_client
{
public:
	virtual char const* get_client_name() const;
	virtual bool connection_lost(e_network_channel_closure_reason*) const;
	virtual bool has_data_to_transmit(bool*) const;
	virtual long space_required_bits(long, long);
	virtual bool write_to_packet(long, c_bitstream*, long, long);
	virtual e_network_read_result read_from_packet(long*, c_bitstream*);
	virtual void notify_packet_acknowledged(long);
	virtual void notify_packet_retired(long, bool);

	byte m_allocated;
	byte m_read_simulation_data_expected;
	byte __unknown6;
	byte m_channel_has_been_used;
};
static_assert(sizeof(c_network_channel_client) == 0x8);

class c_network_connection : c_network_channel_client
{
	struct s_connection_incoming_packet
	{
		word_flags flags;
	};
	static_assert(sizeof(struct s_connection_incoming_packet) == 0x2);

	struct s_connection_outgoing_packet
	{
		long timestamp;
		long size_on_wire;
		long round_trip_msec;
		short unretired_window_size;
		word_flags flags;
	};
	static_assert(sizeof(struct s_connection_outgoing_packet) == 0x10);

	const char* n_name;
	c_sliding_window m_incoming_window;
	c_sliding_window m_outgoing_window;
	long m_incoming_window_size_bytes;
	long m_outgoing_window_size_bytes;
	s_connection_incoming_packet m_incoming_packets[0x80];
	s_connection_outgoing_packet m_outgoing_packets[0x80];
	byte __data94C[0x10];
	long m_packet_timeout;
};
static_assert(sizeof(c_network_connection) == 0x960);