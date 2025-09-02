#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\delivery\network_channel.h>

struct s_network_message_connect_request
{
	ulong channel_identifier;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse
{
	ulong remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish
{
	ulong identifier;
	ulong remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed
{
	ulong remote_channel_identifier;
	ulong channel_identifier;
	c_enum<e_network_channel_closure_reason, long, _network_channel_reason_none, k_network_channel_reason_count> closure_reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);