#include "network_message_handler.h"
#include <iostream>
#include <networking\messages\network_message_gateway.h>
#include <networking\messages\network_messages_out_of_band.h>
#include <networking\messages\network_messages_connect.h>
#include <networking\messages\network_messages_session_protocol.h>
#include <networking\messages\network_messages_session_membership.h>
#include <networking\messages\network_messages_session_parameters.h>
#include <networking\messages\network_messages_simulation.h>
#include <networking\messages\network_messages_simulation_synchronous.h>
#include <networking\messages\network_messages_simulation_distributed.h>
#include <networking\messages\network_messages_test.h>
#include <networking\session\network_session_manager.h>
#include <networking\session\network_session.h>
#include <networking\network_utilities.h>
#include <networking\network_time.h>

void c_network_message_handler::handle_ping(transport_address const* outgoing_address, s_network_message_ping const* message)
{
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_ping: ping #%d received from '%s' at local %dms\n",
		message->id,
		transport_address_get_string(outgoing_address),
		network_time_get_exact());

	s_network_message_pong pong =
	{
		.id = message->id,
		.timestamp = message->timestamp,
		.qos_response = message->request_qos
	};
	m_message_gateway->send_message_directed(outgoing_address, _network_message_pong, sizeof(s_network_message_pong), &pong);
}

void c_network_message_handler::handle_pong(transport_address const* outgoing_address, s_network_message_pong const* message)
{
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_pong: ping #%d returned from '%s' at local %dms (latency %dms)\n", 
		message->id,
		transport_address_get_string(outgoing_address),
		network_time_get_exact(),
		network_time_since(message->timestamp));
}

void c_network_message_handler::handle_connect_request(transport_address const* address, s_network_message_connect_request const* message)
{
	m_observer->handle_connect_request(address, message);
}

void c_network_message_handler::handle_connect_refuse(c_network_channel* channel, s_network_message_connect_refuse const* message)
{
	DECLFUNC(0x25AC0, void, __fastcall, c_network_channel*, s_network_message_connect_refuse const*)(channel, message);
}

void c_network_message_handler::handle_connect_establish(c_network_channel* channel, s_network_message_connect_establish const* message)
{
	if (channel->closed())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: ignoring connect establish from '%s'/%d (currently closed)\n",
			channel->get_name(),
			message->remote_identifier);
		return;
	}
	if (channel->get_identifier() != message->identifier)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: ignoring connect establish from '%s'/%d (establishment identifier %d != local identifier %d)\n",
			channel->get_name(),
			message->remote_identifier,
			message->identifier,
			channel->get_identifier());
		return;
	}
	if (channel->established() && channel->get_remote_identifier() != message->remote_identifier)
	{
		long channel_identifier = NONE;
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d but we are already established to %d\n",
			channel->get_name(),
			message->remote_identifier,
			channel->get_remote_identifier());
		if (!channel->m_message_queue.has_channel_been_used())
		{
			channel_identifier = channel->get_identifier();
		}
		transport_address remote_address;
		channel->get_remote_address(&remote_address);
		channel->close(_network_channel_reason_connect_reinitiated);
		channel->open(&remote_address, false, channel_identifier);
	}
	if (channel->closed())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d for local %d but the channel closed before we could establish\n",
			channel->get_name(),
			message->remote_identifier,
			channel->get_identifier());
		return;
	}
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_establish: received establishment from '%s'/%d for local %d\n",
		channel->get_name(),
		message->remote_identifier,
		channel->get_identifier());
	channel->send_connection_established(message->remote_identifier);
}

void c_network_message_handler::handle_connect_closed(c_network_channel* channel, s_network_message_connect_closed const* message)
{
	if (channel->closed())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' ignoring remote closure (already closed)\n",
			channel->get_name());
		return;
	}
	if (channel->get_identifier() != message->remote_channel_identifier)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' ignoring remote closure (closed identifier %d != identifier %d)\n",
			channel->get_name(),
			message->remote_channel_identifier,
			channel->get_identifier());
		return;
	}
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_connect_closed: '%s' remotely closed (reason #%d: '%s')\n",
		channel->get_name(),
		message->closure_reason.get(),
		channel->get_closure_reason_string(message->closure_reason));
	channel->close(_network_channel_reason_remote_closure);
}

void c_network_message_handler::handle_join_request(transport_address const* outgoing_address, s_network_message_join_request const* message)
{
	if (message->protocol != k_network_protocol_version)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: received message with incorrect protocol version [%d!=%d]\n",
			message->protocol,
			k_network_protocol_version);
		return;
	}

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		// if you get this error, it's likely caused by the client trying to connect with the wrong secure address/id, usually because the API server hasn't updated with the new server information
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: can't handle join-request for '%s' from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			transport_address_get_string(outgoing_address));
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_request: failed to handle incoming join request\n");
		return;
	}

	session->handle_join_request(outgoing_address, message);
}

void c_network_message_handler::handle_peer_connect(transport_address const* outgoing_address, s_network_message_peer_connect const* message)
{
	if (message->protocol != k_network_protocol_version)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_connect: received message with incorrect protocol version [%d!=%d]\n",
			message->protocol,
			k_network_protocol_version);
		return;
	}

	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_connect: no session, ignoring peer connect from '%s'\n",
			transport_address_get_string(outgoing_address));
		return;
	}

	session->handle_peer_connect(outgoing_address, message);
}

void c_network_message_handler::handle_join_abort(transport_address const* outgoing_address, s_network_message_join_abort const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->established() || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: no session, ignoring join abort from '%s'\n",
			transport_address_get_string(outgoing_address));
		return;
	}

	e_network_join_refuse_reason refuse_reason = _network_join_refuse_reason_none;
	if (session->join_abort(outgoing_address, message->join_nonce))
	{
		refuse_reason = _network_join_refuse_reason_abort_successful;
	}
	else
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: ignoring unknown abort from '%s'\n",
			transport_address_get_string(outgoing_address));
		refuse_reason = _network_join_refuse_reason_abort_ignored;
	}

	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_join_abort: received abort, sending join-refusal (%s) to '%s'\n",
		network_message_join_refuse_get_reason_string(refuse_reason),
		transport_address_get_string(outgoing_address));
	s_network_message_join_refuse refuse_message =
	{
		.session_id = message->session_id,
		.reason = refuse_reason
	};
	m_message_gateway->send_message_directed(outgoing_address, _network_message_join_refuse, sizeof(s_network_message_join_refuse), &refuse_message);
}

void c_network_message_handler::handle_join_refuse(transport_address const* outgoing_address, s_network_message_join_refuse const* message)
{
	INVOKE_CLASS_MEMBER(0x25660, c_network_message_handler, handle_join_refuse, outgoing_address, message);
}

// this is what clients SHOULD be using when they disconnect, instead they just release the channel and timeout - thanks saber
void c_network_message_handler::handle_leave_session(transport_address const* outgoing_address, s_network_message_leave_session const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_leave_session: ignoring leave-session from '%s' (session not found)\n",
			transport_address_get_string(outgoing_address));
		return;
	}

	if (!session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_leave_session: ignoring leave-session from '%s' (not hosting)\n",
			transport_address_get_string(outgoing_address));
		return;
	}

	if (!session->handle_leave_request(outgoing_address))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_leave_session: can't handle leave-session request (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			transport_address_get_string(outgoing_address));
		return;
	}
}

void c_network_message_handler::handle_leave_acknowledge(transport_address const* outgoing_address, s_network_message_leave_acknowledge const* message)
{
	INVOKE_CLASS_MEMBER(0x256E0, c_network_message_handler, handle_leave_acknowledge, outgoing_address, message);
}

void c_network_message_handler::handle_session_disband(transport_address const* outgoing_address, s_network_message_session_disband const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_disband: ignoring message from '%s' (no %s session)\n",
			transport_address_get_string(outgoing_address),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	if (!session->handle_session_disband(outgoing_address, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_disband: session (%s) failed to handle session-disband from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			transport_address_get_string(outgoing_address));
		return;
	}
}

void c_network_message_handler::handle_session_boot(transport_address const* outgoing_address, s_network_message_session_boot const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_boot: ignoring session-boot from '%s' (no %s session)\n",
			transport_address_get_string(outgoing_address),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}
	if (!session->handle_session_boot(outgoing_address, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_session_boot: session (%s) failed to handle session-boot from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			transport_address_get_string(outgoing_address));
		return;
	}
}

void c_network_message_handler::handle_host_decline(c_network_channel* channel, s_network_message_host_decline const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_host_decline: channel '%s' ignoring host-decline (%s) (can not find session)\n",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	if (!session->handle_host_decline(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_host_decline: session failed to handle host-decline (%s)\n",
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}
}

void c_network_message_handler::handle_peer_establish(c_network_channel* channel, s_network_message_peer_establish const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->handle_peer_establish(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_establish: channel '%s' failed to handle peer-establish (%s)\n",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
	
		s_network_message_host_decline decline_message =
		{
			.session_id = message->session_id
		};
		channel->send_message(_network_message_host_decline, sizeof(s_network_message_host_decline), &decline_message);
	}
}

void c_network_message_handler::handle_time_synchronize(transport_address const* outgoing_address, s_network_message_time_synchronize const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_time_synchronize: session doesn't exist to handle time-synchronize (%s %d) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			message->synchronization_stage,
			transport_address_get_string(outgoing_address));
		return;
	}

	if (!session->handle_time_synchronize(outgoing_address, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_time_synchronize: session failed to handle time-synchronize (%s %d) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			message->synchronization_stage,
			transport_address_get_string(outgoing_address));
		return;
	}
}

void c_network_message_handler::handle_membership_update(c_network_channel* channel, s_network_message_membership_update const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->channel_is_authoritative(channel))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_membership_update: channel '%s' ignoring membership-update (channel is not authoritative)\n",
			channel->get_name());
		return;
	}

	if (!session->handle_membership_update(message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_membership_update: failed to handle authoritative membership-update (%s update %d) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			message->update_number,
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_peer_properties(c_network_channel* channel, s_network_message_peer_properties const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_properties: channel '%s' ignoring peer-properties (%s) (not host)\n",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	if (!session->handle_peer_properties(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_peer_properties: session failed to handle peer-properties (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_delegate_leadership(c_network_channel* channel, s_network_message_delegate_leadership const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_delegate_leadership: channel '%s' ignoring delegate-leadership (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	// TODO: implement this
	if (false/*!session->handle_delegate_leadership(channel, message)*/)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_delegate_leadership: session failed to handle delegate-leadership (%s) from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_boot_machine(c_network_channel* channel, s_network_message_boot_machine const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_boot_machine: channel '%s' ignoring boot-machine (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	// TODO: implement this
	if (false/*!session->handle_boot_machine(channel, message)*/)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_boot_machine: session failed to handle boot-machine (%s) from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_player_add(c_network_channel* channel, s_network_message_player_add const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_add: channel '%s' ignoring player-add (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	// TODO: implement this
	if (false/*!session->handle_player_add(channel, message)*/)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_add: session failed to handle player-add (%s) from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_player_refuse(c_network_channel* channel, s_network_message_player_refuse const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->channel_is_authoritative(channel))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_refuse: channel '%s' ignoring player-refuse (%s) (channel not authoritative)\n",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	if (!session->handle_player_refuse(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_refuse: session failed to handle player-refuse (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_player_remove(c_network_channel* channel, s_network_message_player_remove const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_remove: channel '%s' ignoring player-remove (%s) (not host)",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	// TODO: implement this
	if (false/*!session->handle_player_remove(channel, message)*/)
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_remove: session failed to handle player-remove (%s) from '%s'",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_player_properties(c_network_channel* channel, s_network_message_player_properties const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_properties: channel '%s' ignoring player-properties (%s) (not host)\n",
			channel->get_name(),
			transport_secure_identifier_get_string(&message->session_id));
		return;
	}

	if (!session->handle_player_properties(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_player_properties: session failed to handle player-properties (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_parameters_update(c_network_channel* channel, s_network_message_parameters_update const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->channel_is_authoritative(channel))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_update: channel '%s' ignoring parameters-update (channel is not authoritative)\n",
			channel->get_name());
		return;
	}

	if (!session->handle_parameters_update(message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_update: session failed to handle authoritative parameters-update (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_parameters_request(c_network_channel* channel, s_network_message_parameters_request const* message)
{
	c_network_session* session = m_session_manager->get_session(&message->session_id);
	if (!session || !session->is_host())
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_request: channel '%s' ignoring parameters-request (we are not the host)\n",
			channel->get_name());
		return;
	}

	if (!session->handle_parameters_request(channel, message))
	{
		printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_parameters_request: session failed to handle parameters-request (%s) from '%s'\n",
			transport_secure_identifier_get_string(&message->session_id),
			channel->get_name());
		return;
	}
}

void c_network_message_handler::handle_view_establishment(c_network_channel* channel, s_network_message_view_establishment const* message)
{
	// non original but useful log
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_view_establishment: establishment %d received\n", message->establishment_mode);

	DECLFUNC(0x257B0, void, __thiscall, c_network_channel*)(channel/*this, channel, message*/);
}

void c_network_message_handler::handle_player_acknowledge(c_network_channel* channel, s_network_message_player_acknowledge const* message)
{
	DECLFUNC(0x25810, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_synchronous_update(c_network_channel* channel, s_network_message_synchronous_update const* message)
{
	static void* handle_synchronous_update_call = (void*)BASE_ADDRESS(0x25860);
	__asm
	{
		mov ecx, channel
		call handle_synchronous_update_call
	}
}

void c_network_message_handler::handle_synchronous_playback_control(c_network_channel* channel, s_network_message_synchronous_playback_control const* message)
{
	DECLFUNC(0x258E0, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_synchronous_actions(c_network_channel* channel, s_network_message_synchronous_actions const* message)
{
	DECLFUNC(0x25990, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_synchronous_acknowledge(c_network_channel* channel, s_network_message_synchronous_acknowledge const* message)
{
	DECLFUNC(0x25810, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_synchronous_gamestate(long size, void const* message)
{
	INVOKE(0x25A20, c_network_message_handler::handle_synchronous_gamestate, size, message);
}

void c_network_message_handler::handle_distributed_game_results(c_network_channel* channel, s_network_message_distributed_game_results const* message)
{
	DECLFUNC(0x25A70, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_synchronous_client_ready(c_network_channel* channel)
{
	DECLFUNC(0x25950, void, __thiscall, c_network_channel*)(channel);
}

void c_network_message_handler::handle_test(c_network_channel* channel, s_network_message_test const* message)
{
	//network_test_messages_handle_message(channel, message->in_sequence, message->id);
}

void c_network_message_handler::handle_channel_message(c_network_channel* channel, e_network_message_type message_type, long message_storage_size, void const* message)
{
#define LOG_RECEIVED_OVER_CLOSED_CHANNEL() \
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over CLOSED channel '%s'\n", \
	message_type, \
	channel->m_type_collection->get_message_type_name(message_type), \
	channel->get_name())

#define LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL() \
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received over a non-connected channel '%s', discarding\n", \
	message_type, \
	channel->m_type_collection->get_message_type_name(message_type), \
	channel->get_name())

	ASSERT(m_initialized);

	network_message_converter_t converter = { .message_storage = message };

	// non-original log but its useful to know when channel messages arrive
	transport_address remote_address;
	channel->get_remote_address(&remote_address);
	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: %d/%s received channel message from '%s'\n",
		message_type,
		m_message_types->get_message_type_name(message_type),
		transport_address_get_string(&remote_address));

	switch (message_type)
	{
		case _network_message_connect_establish:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));

			handle_connect_establish(channel, converter.message_connect_establish);
			break;
		}
		case _network_message_leave_session:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_leave_session));

			if (channel->connected() && channel->get_remote_address(&remote_address))
			{
				handle_leave_session(&remote_address, converter.message_leave_session);
			}
			else
			{
				LOG_RECEIVED_OVER_CLOSED_CHANNEL();
			}
			break;
		}
		case _network_message_session_disband:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_session_disband));

			if (channel->connected() && channel->get_remote_address(&remote_address))
			{
				handle_session_disband(&remote_address, converter.message_session_disband);
			}
			else
			{
				LOG_RECEIVED_OVER_CLOSED_CHANNEL();
			}
			break;
		}
		case _network_message_session_boot:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_session_boot));

			if (channel->connected() && channel->get_remote_address(&remote_address))
			{
				handle_session_boot(&remote_address, converter.message_session_boot);
			}
			else
			{
				LOG_RECEIVED_OVER_CLOSED_CHANNEL();
			}
			break;
		}
		case _network_message_host_decline:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_host_decline));

			if (channel->connected())
			{
				handle_host_decline(channel, converter.message_host_decline);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_peer_establish:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_peer_establish));

			if (channel->connected())
			{
				handle_peer_establish(channel, converter.message_peer_establish);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_membership_update:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_membership_update));

			if (channel->connected())
			{
				handle_membership_update(channel, converter.message_membership_update);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_peer_properties:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_peer_properties));

			if (channel->connected())
			{
				handle_peer_properties(channel, converter.message_peer_properties);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_delegate_leadership:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_delegate_leadership));

			if (channel->connected())
			{
				handle_delegate_leadership(channel, converter.message_delegate_leadership);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_boot_machine:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_boot_machine));

			if (channel->connected())
			{
				handle_boot_machine(channel, converter.message_boot_machine);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_player_add:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_player_add));

			if (channel->connected())
			{
				handle_player_add(channel, converter.message_player_add);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_player_refuse:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_player_refuse));

			if (channel->connected())
			{
				handle_player_refuse(channel, converter.message_player_refuse);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_player_remove:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_player_remove));

			if (channel->connected())
			{
				handle_player_remove(channel, converter.message_player_remove);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_player_properties:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_player_properties));

			if (channel->connected())
			{
				handle_player_properties(channel, converter.message_player_properties);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_parameters_update:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_parameters_update));

			if (channel->connected())
			{
				handle_parameters_update(channel, converter.message_parameters_update);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_parameters_request:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_parameters_request));

			if (channel->connected())
			{
				handle_parameters_request(channel, converter.message_parameters_request);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_view_establishment:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_view_establishment));

			if (channel->connected())
			{
				handle_view_establishment(channel, converter.message_view_establishment);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_player_acknowledge:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_player_acknowledge));

			if (channel->connected())
			{
				handle_player_acknowledge(channel, converter.message_player_acknowledge);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_update:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_synchronous_update));

			if (channel->connected())
			{
				handle_synchronous_update(channel, converter.message_synchronous_update);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_playback_control:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_synchronous_playback_control));

			if (channel->connected())
			{
				handle_synchronous_playback_control(channel, converter.message_synchronous_playback_control);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_actions:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_synchronous_actions));

			if (channel->connected())
			{
				handle_synchronous_actions(channel, converter.message_synchronous_actions);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_acknowledge:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_synchronous_acknowledge));

			if (channel->connected())
			{
				handle_synchronous_acknowledge(channel, converter.message_synchronous_acknowledge);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_gamestate:
		{
			ASSERT(message_storage_size >= sizeof(s_network_message_synchronous_gamestate));

			if (channel->connected())
			{
				handle_synchronous_gamestate(message_storage_size - sizeof(s_network_message_synchronous_gamestate), (byte*)converter.message_synchronous_gamestate + sizeof(s_network_message_synchronous_gamestate));
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_game_results:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_distributed_game_results));

			if (channel->connected())
			{
				handle_distributed_game_results(channel, converter.message_distributed_game_results);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		case _network_message_synchronous_client_ready:
		{
			handle_synchronous_client_ready(channel);
			break;
		}
		case _network_message_test:
		{
			ASSERT(message_storage_size == sizeof(s_network_message_test));

			if (channel->connected())
			{
				handle_test(channel, converter.message_test);
			}
			else
			{
				LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL();
			}
			break;
		}
		default:
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_channel_message: type %d/%s not allowed over channel '%s', discarding",
				message_type,
				m_message_types->get_message_type_name(message_type),
				channel->get_name());
			break;
		}
	}

#undef LOG_RECEIVED_OVER_CLOSED_CHANNEL
#undef LOG_RECEIVED_OVER_NON_CONNECTED_CHANNEL
}

void c_network_message_handler::handle_out_of_band_message(transport_address const* address, e_network_message_type message_type, long message_storage_size, void const* message)
{
	ASSERT(m_initialized);

	printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s received out-of-band from '%s'\n",
		message_type,
		m_message_types->get_message_type_name(message_type),
		transport_address_get_string(address));

	network_message_converter_t converter = { .message_storage = message };

	switch (message_type)
	{
		case _network_message_ping:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_ping));
			handle_ping(address, converter.message_ping);
			break;
		}
		case _network_message_pong:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_pong));
			handle_pong(address, converter.message_pong);
			break;
		}
		case _network_message_connect_request:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_connect_request));
			handle_connect_request(address, converter.message_connect_request);
			break;
		}
		case _network_message_connect_refuse:
		{
			c_network_channel* associated_channel = c_network_link::get_associated_channel(address);
			if (associated_channel)
			{
				//ASSERT(message_storage_size == sizeof(s_network_message_connect_refuse));
				handle_connect_refuse(associated_channel, converter.message_connect_refuse);
			}
			break;
		}
		case _network_message_connect_establish:
		{
			c_network_channel* associated_channel = c_network_link::get_associated_channel(address);
			if (associated_channel)
			{
				//ASSERT(message_storage_size == sizeof(s_network_message_connect_establish));
				handle_connect_establish(associated_channel, converter.message_connect_establish);
			}
			break;
		}
		case _network_message_connect_closed:
		{
			c_network_channel* associated_channel = m_link->get_associated_channel(address);
			if (associated_channel)
			{
				//ASSERT(message_storage_size == sizeof(s_network_message_connect_closed));
				handle_connect_closed(associated_channel, converter.message_connect_closed);
			}
			break;
		}
		case _network_message_join_request:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_join_request));
			handle_join_request(address, converter.message_join_request);
			break;
		}
		case _network_message_peer_connect:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_peer_connect));
			handle_peer_connect(address, converter.message_peer_connect);
			break;
		}
		case _network_message_join_abort:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_join_abort));
			handle_join_abort(address, converter.message_join_abort);
			break;
		}
		case _network_message_join_refuse:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_join_refuse));
			handle_join_refuse(address, converter.message_join_refuse);
			break;
		}
		case _network_message_leave_session:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_leave_session));
			handle_leave_session(address, converter.message_leave_session);
			break;
		}
		case _network_message_leave_acknowledge:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_leave_acknowledge));
			handle_leave_acknowledge(address, converter.message_leave_acknowledge);
			break;
		}
		case _network_message_session_disband:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_session_disband));
			handle_session_disband(address, converter.message_session_disband);
			break;
		}
		case _network_message_session_boot:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_session_boot));
			handle_session_boot(address, converter.message_session_boot);
			break;
		}
		case _network_message_time_synchronize:
		{
			//ASSERT(message_storage_size == sizeof(s_network_message_time_synchronize));
			handle_time_synchronize(address, converter.message_time_synchronize);
			break;
		}
		default:
		{
			printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: c_network_message_handler::handle_out_of_band_message: %d/%s from '%s' cannot be handled out-of-band, discarding\n",
				message_type,
				m_message_types->get_message_type_name(message_type),
				transport_address_get_string(address));
			break;
		}
	}
}