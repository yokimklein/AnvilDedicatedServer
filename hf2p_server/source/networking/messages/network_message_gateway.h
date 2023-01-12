#pragma once
#include "..\..\cseries\cseries.h"
#include "network_message_type_collection.h"
#include "..\transport\transport_address.h"
#include "..\..\memory\bitstream.h"
#include "network_messages_out_of_band.h"

class c_network_link;
class c_network_message_handler;
class c_network_message_type_collection;
class c_network_message_gateway : c_network_out_of_band_consumer
{
public:
	virtual bool __cdecl receive_out_of_band_packet(s_transport_address const* address, c_bitstream* packet);
	c_network_link* get_network_link();
	bool send_message_directed(s_transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, s_network_message const* message);

	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_type_collection;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x4B0];
	byte m_outgoing_packet_storage_end;
	short : 16;
	s_transport_address m_outgoing_packet_address;
	long : 32;
	c_bitstream m_outgoing_packet;
};
static_assert(sizeof(c_network_message_gateway) == 0x580);