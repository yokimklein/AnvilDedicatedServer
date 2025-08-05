#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\transport\transport_address.h>
#include <memory\bitstream.h>
#include <networking\messages\network_out_of_band_consumer.h>
#include <networking\delivery\network_link.h>

class c_network_link;
class c_network_message_handler;
class c_network_message_type_collection;
class c_network_message_gateway : public c_network_out_of_band_consumer
{
public:
	virtual bool receive_out_of_band_packet(transport_address const* incoming_address, c_bitstream* packet) override;

	// non source original getter
	const c_network_message_type_collection* message_types() const { return m_message_types; };

	bool send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, void const* message);

private:
	bool m_initialized;
	c_network_link* m_link;
	const c_network_message_type_collection* m_message_types;
	c_network_message_handler* m_message_handler;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[k_network_link_maximum_game_data_size];
	byte m_outgoing_packet_storage_end;
	short : 16;
	transport_address m_outgoing_packet_address;
	c_bitstream m_outgoing_packet;
};
static_assert(sizeof(c_network_message_gateway) == 0x580);
