#pragma once
#include "..\transport\transport_address.h"
#include "..\delivery\network_link.h"
#include "network_message_type_collection.h"
#include <libloaderapi.h>

static uintptr_t module_base = (uintptr_t)GetModuleHandle(NULL);

class c_network_message_gateway // : c_network_channel_owner?
{
public:
	// send_message_directed

	c_network_link* get_network_link();

private:
	long : 32; // c_network_channel_owner vftable pointer?
	bool m_initialized;
	c_network_link* m_link; // c_network_link
	c_network_message_type_collection* m_message_type_collection;
	long : 32;
	bool m_outgoing_packet_pending;
	byte m_outgoing_packet_storage[0x600];
	s_transport_address m_transport_address;
	long m_outgoing_packet; // c_bitstream
};
//static_assert(sizeof(c_network_message_gateway) == 0x62C + sizeof(c_bistream));

typedef int(__thiscall* c_network_message_gateway__send_message_directed_ptr)(c_network_message_gateway* network_message_gateway, s_transport_address outgoing_address, e_network_message_type network_message_type, long message_storage_size, s_network_message* message);
static auto c_network_message_gateway__send_message_directed = reinterpret_cast<c_network_message_gateway__send_message_directed_ptr>(module_base + 0x232C0);