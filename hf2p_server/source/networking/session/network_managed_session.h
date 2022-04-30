#pragma once
#include "..\transport\transport_address.h"
#include "..\..\simulation\simulation.h"
#include "..\..\dllmain.h"

enum e_online_context_id : uint32_t
{
	_online_context_id_game_mode = 0,
	_online_context_id_game_type,

	k_online_context_id_count
};

struct s_online_session_player
{
	byte flags;
	byte __align1[7];
	int64_t xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);

struct s_online_session
{
	uint32_t controller;
	uint16_t public_slots_flags;
	uint16_t private_slots_flags;
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description description;
	int64_t nonce;
	HANDLE handle;
	s_online_session_player players[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_online_session) == 0x150);

struct c_overlapped_task
{
	struct c_overlapped_task_vtbl* __vftable /*VFT*/;
	uint16_t __unknown8;
	uint16_t m_context;
	const char* m_file;
	uint32_t m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);

struct c_managed_session_overlapped_task : c_overlapped_task
{
	uint32_t context;
	char __data14[236];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);

struct s_online_context
{
	e_online_context_id id;
	uint32_t value;
};
static_assert(sizeof(s_online_context) == 0x8);

struct c_managed_session // 0x5D0
{
	uint32_t session_class;
	e_transport_platform platform; // size=0x4
	uint32_t flags; // e_online_managed_session_flags
	uint16_t current_operation_flags;
	uint16_t pending_operation_flags;
	c_managed_session_overlapped_task overlapped_task;
	uint32_t creation_time;
	char __data114[4];
	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;
	int64_t xuids[16];
	byte xuid_flags[16];
	byte xuid_next_flags[16];
	long session_player_operation_count;
	s_online_context creation_contexts[4];
	uint32_t creation_context_count;
};
static_assert(sizeof(c_managed_session) == 0x5D0);

struct s_online_session_manager_globals
{
	uint16_t current_operation_flags;
	char __data2[6];
	c_managed_session managed_sessions[8];
};
static_assert(sizeof(s_online_session_manager_globals) == 0x2E88);

static s_online_session_manager_globals* online_session_manager_globals = reinterpret_cast<s_online_session_manager_globals*>(module_base + 0x3EAB120);

bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform);
const char* managed_session_get_id_string(long managed_session_index);
bool managed_session_get_id(long index, s_transport_secure_identifier* secure_id);
void managed_session_modify_slot_counts(long managed_session_index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count);