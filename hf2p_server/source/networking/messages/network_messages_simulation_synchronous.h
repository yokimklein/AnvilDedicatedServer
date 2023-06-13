#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>

struct s_network_message_synchronous_update : s_network_message
{
	byte unknown_data[0x1668];
};
static_assert(sizeof(s_network_message_synchronous_update) == 0x1668);

struct s_network_message_synchronous_playback_control : s_network_message
{
	ulong type;
	ulong identifier;
	ulong update_number;
};
static_assert(sizeof(s_network_message_synchronous_playback_control) == 0xC);

struct s_network_message_synchronous_actions : s_network_message
{
	ulong action_number;
	ulong current_update_number;
    ulong user_flags;
	byte unknown_data[0x204]; // array of 4 for each user?
};
static_assert(sizeof(s_network_message_synchronous_actions) == 0x210);

struct s_network_message_synchronous_acknowledge : s_network_message
{
	ulong current_update_number;
};
static_assert(sizeof(s_network_message_synchronous_acknowledge) == 0x4);

struct s_network_message_synchronous_gamestate : s_network_message
{
	ulong unknown1;
	ulong unknown2;
	ulong unknown3;
	byte unknown4[0x4];
};
static_assert(sizeof(s_network_message_synchronous_gamestate) == 0x10); // max size of 0x40000?

struct s_network_message_synchronous_client_ready : s_network_message
{
	// contains no members?
};
//static_assert(sizeof(s_network_message_synchronous_client_ready) == 0x0);