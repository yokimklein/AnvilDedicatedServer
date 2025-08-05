#pragma once
#include <cseries\cseries.h>
#include <cseries\async_xoverlapped.h>
#include <networking\online\online_session.h>
#include <networking\online\online_constants.h>

constexpr long k_managed_sessions_max_count = 8;

struct s_online_session;
struct s_transport_secure_identifier;
class c_network_session_membership;

//typedef c_flags<e_managed_session_creation_flags, uns8, k_managed_session_creation_flags_count> c_managed_session_creation_flags;
typedef byte c_managed_session_creation_flags;

enum e_online_managed_session_flags
{
	_online_managed_session_allocated_bit = 0,
	_online_managed_session_master_bit,
	_online_managed_session_pending_retry_bit,
	_online_managed_session_force_create_offline_bit,
	_online_managed_session_created_bit,
	_online_managed_session_creation_failed_bit,
	_online_managed_session_host_migration_in_process_bit,
	_online_managed_session_host_migration_session_created_bit,
	_online_managed_session_host_migration_must_delete_session_bit,
	_online_managed_session_host_migration_using_new_session_bit,
	_online_managed_session_players_add_pending_bit,
	_online_managed_session_players_add_succeeded_bit,
	_online_managed_session_players_add_failed_bit,
	_online_managed_session_game_started_bit,
	_online_managed_session_game_start_failed_bit,
	_online_managed_session_game_ended_bit,
	_online_managed_session_game_end_failed_bit,
	_online_managed_session_locking_flags_bit,
	_online_managed_session_flags_locked_bit,

	k_online_managed_session_flags_count
};
typedef c_flags<e_online_managed_session_flags, ulong, k_online_managed_session_flags_count> c_online_managed_session_flags;

enum e_online_managed_session_operation_flags
{
	_online_managed_session_delete_bit = 0,
	_online_managed_session_free_bit,
	_online_managed_session_create_bit,
	_online_managed_session_delete_host_migration_bit,
	_online_managed_session_create_host_migration_bit,
	_online_managed_session_recreate_players_bit,
	_online_managed_session_players_remove_bit,
	_online_managed_session_modify_session_bit,
	_online_managed_session_players_add_bit,
	_online_managed_session_game_end_bit,
	_online_managed_session_game_start_bit,

	k_online_managed_session_operation_flags_count
};
typedef c_flags<e_online_managed_session_operation_flags, ushort, k_online_managed_session_operation_flags_count> c_online_managed_session_operation_flags;

enum e_managed_session_status_flags
{
	_managed_session_creation_in_progress_bit = 0,
	_managed_session_creation_succeeded_bit,
	_managed_session_creation_failed_bit,
	_managed_session_host_migration_in_progress_bit,
	_managed_session_host_migration_session_created_bit,
	_managed_session_players_add_in_progress_bit,
	_managed_session_players_add_succeeded_bit,
	_managed_session_players_add_failed_bit,
	_managed_session_game_start_in_progress_bit,
	_managed_session_game_started_bit,
	_managed_session_game_start_failed_bit,
	_managed_session_game_end_in_progress_bit,
	_managed_session_game_ended_bit,
	_managed_session_game_end_failed_bit,

	k_managed_session_status_flags_count,
};
typedef c_flags_no_init<e_managed_session_status_flags, word, k_managed_session_status_flags_count> c_managed_session_status_flags;

class c_managed_session_overlapped_task : public c_overlapped_task
{
public:
	//void __thiscall complete_();
	//void __thiscall failure_(dword calling_result, dword overlapped_error, dword overlapped_extended_error);
	//void __thiscall success_(dword return_result);

public:
	virtual ~c_managed_session_overlapped_task();
	virtual const char* get_context_string() const override;
	virtual dword start(void* overlapped) override;
	virtual void success(dword return_result) override;
	virtual void failure(dword calling_result, dword overlapped_error, dword overlapped_extended_error) override;
	virtual void complete() override;

public:
	using t_completion_routine = void __cdecl(long, bool, dword);
	
	//void filter_local_users(int32 player_count, const uns64* players, const bool* online_enabled, const bool* private_slots);
	//void process_add_players(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, const uns64* player_xuids, const bool* online_enabled, const bool* private_slots, int32 player_count);
	//bool process_add_players_immediately(s_online_session* session, const uns64* player_xuids, const bool* online_enabled, const bool* private_slots, int32 player_count);
	//void process_create(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, uns16 mask);
	//void process_delete(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session);
	//void process_game_end(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session);
	//void process_modify(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, s_online_session* desired_session, s_online_session* actual_session);
	//static bool __cdecl process_modify_immediately(s_online_session* desired_session, s_online_session* actual_session);
	//void process_game_start(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session);
	//void process_remove_players(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, const uns64* player_xuids, const bool* online_enabled, int32 player_count);
	//bool process_remove_players_immediately(s_online_session* session, const uns64* player_xuids, const bool* online_enabled, int32 player_count);
	//void process_session_host_migrate(int32 managed_session_index, t_completion_routine* completion_routine, s_online_session* session, bool is_host, s_transport_session_description* host_migration_description);

	enum e_overlapped_task
	{
		_process_create = 0,
		_process_delete,
		_process_session_host_migrate,
		_process_modify,
		_process_add_players,
		_process_remove_players,
		_process_start,
		_process_end
	};

	e_overlapped_task m_context;
	s_online_session* m_session;
	s_online_session* m_desired_session;
	s_online_session* m_actual_session;
	long m_managed_session_index;
	t_completion_routine* m_completion_routine;
	bool m_result;
	dword m_return_result;
	word_flags m_mask;
	bool m_is_host;
	s_transport_session_description* m_host_migration_description;
	long m_player_count;
	qword m_player_xuids[k_network_maximum_players_per_session];
	dword_flags m_private_slots[k_network_maximum_players_per_session];
};
static_assert(sizeof(c_managed_session_overlapped_task) == 0x100);

struct s_online_context
{
	e_online_context_id id;
	long value;
};
static_assert(sizeof(s_online_context) == 0x8);

struct s_online_managed_session
{
	enum e_network_session_class session_class;
	enum e_transport_platform platform;
	c_online_managed_session_flags flags;
	c_online_managed_session_operation_flags current_operation_flags;
	c_online_managed_session_operation_flags pending_operation_flags;
	c_managed_session_overlapped_task overlapped_task;
	ulong time_of_last_failure;
	bool not_connected_to_live;
	bool has_no_valid_address;
	bool has_conflicting_session_id;
	byte : 8;
	s_online_session desired_online_session_state;
	s_online_session transitory_online_session_state;
	s_online_session actual_online_session_state;
	qword xuids[k_network_maximum_players_per_session];
	byte_flags xuid_flags[k_network_maximum_players_per_session];
	byte_flags xuid_next_flags[k_network_maximum_players_per_session];
	long session_player_operation_count;
	//bool we_are_the_new_host;
	//bool we_were_the_host;
//#pragma pack(push,1)
	//s_transport_session_description host_migration_description;
//#pragma pack(pop)
	//byte pad3[2];
	s_online_context creation_contexts[4];
	long context_count;
};
static_assert(sizeof(s_online_managed_session) == 0x5D0); // 0x608 in ms23

struct s_online_session_manager_globals
{
	c_online_managed_session_operation_flags current_operation_flags;
	s_online_managed_session managed_sessions[8];
};
static_assert(sizeof(s_online_session_manager_globals) == 0x2E88);

extern s_online_session_manager_globals& online_session_manager_globals;

bool managed_session_get_security_information(long index, s_transport_session_description* out_secure_host_description, e_transport_platform* out_transport_platform);
const char* managed_session_get_id_string(long index);
bool managed_session_get_id(long index, s_transport_secure_identifier* out_secure_identifier);
void managed_session_modify_slot_counts(long index, long private_slot_count, long public_slot_count, bool friends_only, long peer_count);
c_managed_session_status_flags managed_session_get_status(long index);
bool managed_session_is_master_session(long index);
void managed_session_reset_session(long index, bool recreating_session);
void managed_session_remove_players(long index, qword const* xuids, long xuid_count);
void remove_from_player_list(s_online_session_player* players, long player_count, qword const* xuids, long xuid_count);
void managed_session_reset_players_add_status(long index);
void managed_session_add_players(long index, qword const* xuids, bool const* xuids_left_game, long xuid_count);
void add_to_player_list(s_online_session_player* players, long player_count, qword const* xuids, bool const* xuids_left_game, long xuid_count);
bool __fastcall managed_session_compare_id(long index, s_transport_secure_identifier const* secure_id);
void __fastcall managed_session_delete_session_internal(long index, s_online_managed_session* managed_session);
s_online_managed_session* managed_session_get(long index);
