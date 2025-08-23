#include "network_message_gateway.h"

bool c_network_message_gateway::receive_out_of_band_packet(transport_address const* incoming_address, c_bitstream* packet)
{
	return INVOKE_CLASS_MEMBER(0x231A0, c_network_message_gateway, receive_out_of_band_packet, incoming_address, packet);
}

bool c_network_message_gateway::send_message_directed(transport_address const* outgoing_address, e_network_message_type message_type, long message_storage_size, void const* message)
{
	// $TODO: stack overflow crash here inside of c_network_message_gateway::send_all_pending_messages when more than 1 peer disconnects in a single tick
	return INVOKE_CLASS_MEMBER(0x232C0, c_network_message_gateway, send_message_directed, outgoing_address, message_type, message_storage_size, message);
}

void c_network_message_gateway::send_all_pending_messages()
{
	ASSERT(m_initialized);

	if (m_outgoing_packet_pending)
	{
		bool result = true;

		ASSERT(m_outgoing_packet.was_writing());
		ASSERT(!m_outgoing_packet.would_overflow(1));

		m_outgoing_packet.write_bool("has_message", false);
		m_outgoing_packet.finish_writing(NULL);

		if (m_outgoing_packet.begin_consistency_check())
		{
			result = read_packet_header(&m_outgoing_packet);
			if (!result)
			{
				printf("MP/NET/MESSAGE,SND: c_network_message_gateway::send_all_pending_messages: Error. An outgoing message header is corrupt. Addr %s.\n",
					transport_address_get_string(&m_outgoing_packet_address));
			}

			e_network_message_type message_type;
			long message_storage_size = 0;
			// This was made static to avoid a stack overflow crash that would occur when >1 peer disconnects at the same time
			// HO's k_network_message_maximum_size is 4x larger than H3 and 2x as large as MCC
			// In many other places in the code base, this temporary storage was made static, but it seems they missed it here
			static byte message_storage[k_network_message_maximum_size]{};

			while (m_outgoing_packet.read_bool("has_message"))
			{
				result = m_message_types->decode_message(&m_outgoing_packet, &message_type, &message_storage_size, message_storage);
				if (!result)
				{
					printf("MP/NET/MESSAGE,SND: c_network_message_gateway::send_all_pending_messages: Error. An outgoing message payload is corrupt. Type %s. Size %d.\n",
						m_message_types->get_message_type_name(message_type),
						message_storage_size);
					break;
				}

				m_message_types->dispose_message(message_type, message_storage_size, message_storage);
			}

			m_outgoing_packet.finish_reading();
		}

		if (result)
		{
			m_link->send_out_of_band(&m_outgoing_packet, &m_outgoing_packet_address, NULL);
		}

		m_outgoing_packet_pending = false;
	}

	ASSERT(!m_outgoing_packet_pending);
	ASSERT(!m_outgoing_packet.reading() && !m_outgoing_packet.writing());
}

bool __fastcall c_network_message_gateway::read_packet_header(c_bitstream* packet)
{
	ASSERT(packet);
	return INVOKE(0x235C0, c_network_message_gateway::read_packet_header, packet);
}