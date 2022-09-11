#pragma once
#include "..\..\cseries\cseries.h"
#include "..\transport\transport_address.h"
#include "..\..\simulation\simulation.h"
#include "..\..\dllmain.h"

enum e_online_context_id : ulong
{
	_online_context_id_game_mode = 0,
	_online_context_id_game_type,

	k_online_context_id_count
};

struct s_online_session_player
{
	byte flags;
	byte __align1[7];
	qword xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);

struct s_online_session
{
	ulong controller;
	ushort public_slots_flags;
	ushort private_slots_flags;
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description description;
	qword nonce;
	HANDLE handle;
	s_online_session_player players[k_network_maximum_players_per_session];
};
static_assert(sizeof(s_online_session) == 0x150);

struct c_overlapped_task
{
	struct c_overlapped_task_vtbl* __vftable /*VFT*/;
	ushort __unknown8;
	ushort m_context;
	const char* m_file;
	ulong m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);

struct c_managed_session_overlapped_task : c_overlapped_task
{
	ulong context;
	char __data14[236];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);

struct s_online_context
{
	e_online_context_id id;
	ulong value;
};
static_assert(sizeof(s_online_context) == 0x8);

struct c_managed_session // 0x5D0
{
	ulong session_class;
	e_transport_platform platform; // size=0x4
	ulong flags; // e_online_managed_session_flags
	ushort current_operation_flags;
	ushort pending_operation_flags;
	c_managed_session_overlapped_task overlapped_task;
	ulong creation_time;
	char __data114[4];
	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;
	qword xuids[k_network_maximum_players_per_session];
	byte_flags xuid_flags[k_network_maximum_players_per_session];
	byte_flags xuid_next_flags[k_network_maximum_players_per_session];
	long session_player_operation_count;
	s_online_context creation_contexts[4];
	ulong creation_context_count;
};
static_assert(sizeof(c_managed_session) == 0x5D0);

struct s_online_session_manager_globals
{
	ushort current_operation_flags;
	char __data2[6];
	c_managed_session managed_sessions[8];
};
static_assert(sizeof(s_online_session_manager_globals) == 0x2E88);

static s_online_session_manager_globals* online_session_manager_globals = reinterpret_cast<s_online_session_manager_globals*>(module_base + 0x3EAB120);

bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform);
const char* managed_session_get_id_string(long managed_session_index);
bool managed_session_get_id(long index, s_transport_secure_identifier* secure_id);
void managed_session_modify_slot_counts(long managed_session_index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count);
short* managed_session_get_status(short* managed_session_status, long managed_session_index);
bool managed_session_is_master_session(long managed_session_index);
void managed_session_reset_session(long managed_session_index, bool use_session_time);
void managed_session_remove_players(long managed_session_index, qword* xuids, long xuid_count);
void remove_from_player_list(s_online_session_player* players, long player_count, qword* xuids, long xuid_count);
void managed_session_reset_players_add_status(long managed_session_index);
void managed_session_add_players(long managed_session_index, qword* xuids, bool* player_bools, long player_count);
void managed_session_add_players_internal(s_online_session_player* players, long player_count, qword* player_xuids, bool* player_bools, long xuid_count);
bool managed_session_compare_id(long managed_session_index, s_transport_secure_identifier const* secure_id);