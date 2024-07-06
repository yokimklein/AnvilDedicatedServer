#pragma once
#include <cseries\cseries.h>

enum e_life_cycle_state
{
	_life_cycle_state_none,
	_life_cycle_state_pre_game,
	_life_cycle_state_start_game,
	_life_cycle_state_in_game,
	_life_cycle_state_end_game_write_stats,
	_life_cycle_state_leaving,
	_life_cycle_state_joining,
	//_life_cycle_state_matchmaking_start,
	//_life_cycle_state_matchmaking_find_match_client,
	//_life_cycle_state_matchmaking_find_match,
	//_life_cycle_state_matchmaking_find_and_assemble_match,
	//_life_cycle_state_matchmaking_assemble_match,
	//_life_cycle_state_matchmaking_select_host,
	//_life_cycle_state_matchmaking_arbitration,
	//_life_cycle_state_matchmaking_prepare_map,
	//_life_cycle_state_in_match,
	//_life_cycle_state_end_match_write_stats,
	//_life_cycle_state_post_match,

	k_life_cycle_state_count,
};

const long k_maximum_state_change_entry_data_size = 16;

class c_life_cycle_state_handler;
class c_network_session_manager;
class c_network_session;
class c_network_observer;
class c_life_cycle_state_manager
{
public:
	//void request_state_change(e_life_cycle_state state, long entry_data_size, void* entry_data);
	//void request_leave_sessions(bool disconnect);
	//void set_current_state(e_life_cycle_state state, long entry_data_size, void* entry_data);
	//e_life_cycle_state get_current_state() const;
	//c_life_cycle_state_handler* get_current_state_handler() const;
	//void terminate();
	c_network_session* get_active_squad_session() const;
	c_network_session* get_target_session() const;
	//c_network_session* get_group_session() const;
	c_network_observer* get_observer() const;
	//void register_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);
	//void deregister_state_handler(e_life_cycle_state state, c_life_cycle_state_handler* handler);

private:
	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_current_state;
	c_static_array<c_life_cycle_state_handler*, k_life_cycle_state_count> m_handlers;
	c_network_session_manager* m_session_manager;
	c_network_session* m_active_squad_session;
	c_network_session* m_target_session;
	//c_network_session* m_group_session; // Removed since ms23/h3, matchmaking session
	c_network_observer* m_observer;

	bool m_pending_state_change;
	byte : 8;
	byte : 8;
	byte : 8;
	c_enum<e_life_cycle_state, long, k_life_cycle_state_count> m_pending_state;

	long m_entry_data_size;
	byte m_entry_data[k_maximum_state_change_entry_data_size];

	byte m_unknown1;
	byte m_unknown2[k_maximum_state_change_entry_data_size]; // 2 qwords?
	byte m_unknown3;
	long m_unknown4;
};
static_assert(sizeof(c_life_cycle_state_manager) == 0x64);

