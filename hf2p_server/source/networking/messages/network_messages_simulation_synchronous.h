#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>

enum e_network_synchronous_playback_control
{
	_network_synchronous_playback_control_revert = 0,
	_network_synchronous_playback_control_end_playback,

	k_network_synchronous_playback_control_count
};

enum e_synchronous_gamestate_message_type
{
	_synchronous_gamestate_message_initiate_join = 0,
	_synchronous_gamestate_message_gamestate_chunk,
	_synchronous_gamestate_message_checksums,

	k_number_of_synchronous_gamestate_message_types
};

struct s_network_message_synchronous_update
{
	struct simulation_update update;
	s_simulation_update_metadata metadata;
};
static_assert(sizeof(s_network_message_synchronous_update) == 0x1668);

struct s_network_message_synchronous_playback_control
{
	e_network_synchronous_playback_control type;
	long identifier;
	long update_number;
};
static_assert(sizeof(s_network_message_synchronous_playback_control) == 0xC);

struct s_network_message_synchronous_actions
{
	long action_number;
	long current_update_number;
	dword valid_user_mask;
	player_action user_actions[4];
};
static_assert(sizeof(s_network_message_synchronous_actions) == 0x210);

struct s_network_message_synchronous_acknowledge
{
	long current_update_number;
};
static_assert(sizeof(s_network_message_synchronous_acknowledge) == 0x4);

struct s_network_message_synchronous_gamestate
{
	c_enum<e_synchronous_gamestate_message_type, byte, _synchronous_gamestate_message_initiate_join, k_number_of_synchronous_gamestate_message_types> message_type;
	union
	{
		long chunk_offset;
		long next_update_number;
		dword compressed_checksum;
	};
	long chunk_size;
	dword decompressed_checksum;
};
static_assert(sizeof(s_network_message_synchronous_gamestate) == 0x10);
