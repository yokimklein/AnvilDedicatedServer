#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>

struct s_network_message_ping
{
	word id;
	dword timestamp;
	bool request_qos;
};
static_assert(sizeof(s_network_message_ping) == 0xC);

struct s_network_message_pong
{
	word id;
	dword timestamp;
	dword qos_response;
};
static_assert(sizeof(s_network_message_pong) == 0xC);
