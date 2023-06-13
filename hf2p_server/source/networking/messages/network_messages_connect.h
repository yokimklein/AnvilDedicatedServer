#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>

struct s_network_message_connect_request : s_network_message
{
	ulong identifier;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse : s_network_message
{
	ulong remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish : s_network_message
{
	ulong identifier;
	ulong remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed : s_network_message
{
	ulong identifier;
	ulong remote_identifier;
	enum e_network_channel_closure_reason closure_reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);