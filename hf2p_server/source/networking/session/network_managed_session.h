#pragma once
#include "..\..\cseries\cseries.h"
#include "..\transport\transport_security.h"
#include "..\..\simulation\simulation.h"
#include "..\..\dllmain.h"
#include "network_session_parameters_session.h"

enum e_online_context_id : ulong
{
	_online_context_id_game_mode = 0,
	_online_context_id_game_type,

	k_online_context_id_count
};

enum e_online_managed_session_flags : ulong
{
	_online_managed_session_allocated_bit, // allocated datum for managed session
	_online_managed_session_master_session_bit, // is master session
	_online_managed_session_recreating_bit,
	_online_managed_session_offline_bit, // mark session to be created offline
	_online_managed_session_created_bit,
	_online_managed_session_unknown_flag5_bit,
	_online_managed_session_host_migration_in_process_bit,
	_online_managed_session_host_migration_session_created_bit,
	_online_managed_session_unknown_flag8_bit,
	_online_managed_session_unknown_flag9_bit,
	_online_managed_session_players_add_pending_bit,
	_online_managed_session_unknown_flag11_bit,
	_online_managed_session_unknown_flag12_bit,
	_online_managed_session_game_start_succeeded_bit, // game start completed successfully
	_online_managed_session_game_start_failed_bit, // game start failed
	_online_managed_session_game_end_succeeded_bit, // game end completed successfully
	_online_managed_session_game_end_failed_bit, // game end failed
	_online_managed_session_lock_session_bit, // lock session // if (flags.test(17)) flags.set(18, true)
	_online_managed_session_session_is_locked_bit, // session is locked

	k_online_managed_session_flags
};

enum e_online_managed_session_operation_flags : ushort
{
	_online_managed_session_delete_bit,
	_online_managed_session_free_bit,
	_online_managed_session_create_bit,
	_online_managed_session_delete_host_migration_bit,
	_online_managed_session_create_host_migration_bit,
	_online_managed_session_unknown_operation5_bit,
	_online_managed_session_players_remove_bit,
	_online_managed_session_modify_session_bit,
	_online_managed_session_players_add_bit,
	_online_managed_session_game_end_bit,
	_online_managed_session_game_start_bit,
	_online_managed_session_unknown_bit11,

	k_online_managed_session_operation_flags
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
	ushort public_slots_flags; // e_online_session_flags
	ushort private_slots_flags; // e_online_session_flags
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

struct c_managed_session
{
	e_network_session_class session_class;
	e_transport_platform platform;
	c_flags<e_online_managed_session_flags, ulong, k_online_managed_session_flags> flags;
	c_flags<e_online_managed_session_operation_flags, ushort, k_online_managed_session_operation_flags> current_operation_flags;
	c_flags<e_online_managed_session_operation_flags, ushort, k_online_managed_session_operation_flags> pending_operation_flags;
	c_managed_session_overlapped_task overlapped_task;
	ulong creation_time;
	bool creation_failed_not_live;
	bool creation_failed_invalid_ip;
	bool creation_failed_session_id_clash; // another managed session already has this session's ID
	byte pad[1];
	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;
	qword xuids[k_network_maximum_players_per_session];
	byte_flags xuid_flags[k_network_maximum_players_per_session];
	byte_flags xuid_next_flags[k_network_maximum_players_per_session];
	long session_player_operation_count;
	//bool unknown1;
	//byte pad2[1];
	//s_transport_session_description migration_description;
	//byte pad3[2];
	s_online_context creation_contexts[4];
	ulong creation_context_count;
	//int unknown2;
};
static_assert(sizeof(c_managed_session) == 0x5D0); // 0x608 in ms23

struct s_online_session_manager_globals
{
	e_online_managed_session_operation_flags current_operation_flags;
	char __data2[6];
	c_managed_session managed_sessions[8];
};
static_assert(sizeof(s_online_session_manager_globals) == 0x2E88);

static s_online_session_manager_globals* online_session_manager_globals = reinterpret_cast<s_online_session_manager_globals*>(module_base + 0x3EAB120);

bool managed_session_get_security_information(long managed_session_index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform);
const char* managed_session_get_id_string(long managed_session_index);
bool managed_session_get_id(long managed_session_index, s_transport_secure_identifier* secure_id);
void managed_session_modify_slot_counts(long managed_session_index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count);
short* managed_session_get_status(short* managed_session_status, long managed_session_index);
bool managed_session_is_master_session(long managed_session_index);
void managed_session_reset_session(long managed_session_index, bool recreating_session);
void managed_session_remove_players(long managed_session_index, qword* xuids, long xuid_count);
void remove_from_player_list(s_online_session_player* players, long player_count, qword* xuids, long xuid_count);
void managed_session_reset_players_add_status(long managed_session_index);
void managed_session_add_players(long managed_session_index, qword* xuids, bool* player_bools, long player_count);
void managed_session_add_players_internal(s_online_session_player* players, long player_count, qword* xuids, bool* player_bools, long xuid_count);
bool managed_session_compare_id(long managed_session_index, s_transport_secure_identifier const* secure_id);