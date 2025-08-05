#pragma once
#include <cseries\cseries.h>
#include <networking\transport\transport_security.h>
#include <simulation\simulation.h>

enum e_online_session_player_flags
{
	_online_session_player_valid_bit = 0,
	_online_session_player_occupies_private_slot_bit,
	//_online_session_player_is_online_enabled_bit,

	k_online_session_player_flags_count
};

struct s_online_session_player
{
	c_flags_no_init<e_online_session_player_flags, word, k_online_session_player_flags_count> flags;
	qword xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);
static_assert(0x00 == OFFSETOF(s_online_session_player, flags));
static_assert(0x08 == OFFSETOF(s_online_session_player, xuid));

struct s_online_session
{
	ulong controller;
	ushort public_slots_flags; // e_online_session_flags
	ushort private_slots_flags; // e_online_session_flags
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description description;
	qword nonce;
	void* handle;
	s_online_session_player players[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_online_session) == 0x150);