#pragma once

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
};
static_assert(sizeof(c_network_channel_client) == 0x4);