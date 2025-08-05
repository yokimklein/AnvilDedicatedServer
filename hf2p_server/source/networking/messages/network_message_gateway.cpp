#include "network_message_gateway.h"

bool c_network_message_gateway::receive_out_of_band_packet(transport_address const* incoming_address, c_bitstream* packet)
{
	return INVOKE_CLASS_MEMBER(0x000231A0, c_network_message_gateway, receive_out_of_band_packet, incoming_address, packet);
}

bool c_network_message_gateway::send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, void const* message)
{
	return INVOKE_CLASS_MEMBER(0x000232C0, c_network_message_gateway, send_message_directed, outgoing_address, message_type, message_storage_size, message);
}
