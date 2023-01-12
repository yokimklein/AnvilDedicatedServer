#pragma once
#include "..\delivery\network_connection.h"

enum e_network_read_result;
class c_bitstream;
struct s_replication_outgoing_request;
struct s_replication_incoming_request;
struct c_replication_scheduler_client
{
private:
	virtual bool __thiscall has_data_to_transmit();
	virtual long __thiscall build_outgoing_requests(void const*, long, s_replication_outgoing_request*);
	virtual long __thiscall terminator_required_bits(void) const;
	virtual void __thiscall write_to_packet(long, long, void const*, long, c_bitstream*, long);
	virtual void __thiscall write_terminator_to_packet(long, c_bitstream*);
	virtual e_network_read_result __thiscall read_from_packet(long, c_bitstream*, long, s_replication_incoming_request*, long*);
	virtual void __thiscall process_incoming_request(s_replication_incoming_request*);
	virtual void __thiscall notify_packet_acknowledged(long, bool);
	virtual void __thiscall notify_packet_retired(long, bool, bool);
};

enum e_network_channel_closure_reason : long;
struct c_replication_telemetry_provider;
struct c_replication_scheduler : c_network_channel_client
{
public:
	virtual char const* __thiscall get_client_name() const;
	virtual bool __thiscall connection_lost(e_network_channel_closure_reason*) const;
	virtual bool __thiscall has_data_to_transmit(bool*) const;
	virtual long __thiscall space_required_bits(long, long);
	virtual bool __thiscall write_to_packet(long, c_bitstream*, long, long);
	virtual e_network_read_result __thiscall read_from_packet(long*, c_bitstream*);
	virtual void __thiscall notify_packet_acknowledged(long);
	virtual void __thiscall notify_packet_retired(long, bool);

	long m_view_index;
	c_replication_scheduler_client* m_clients[3];
	long m_client_terminator_bits[3];
	long m_space_required_bits;
	c_replication_telemetry_provider* m_telemetry_provider;
};
static_assert(sizeof(c_replication_scheduler) == 0x2C);