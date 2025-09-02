#pragma once
#include <cseries\cseries.h>
#include <game\player_configuration.h>
#include <simulation\simulation_players.h>
#include <networking\delivery\network_channel.h>
#include <networking\session\network_observer.h>

class c_simulation_world;
class c_network_observer;
class c_network_session;
enum e_network_session_boot_reason;
class c_simulation_watcher : public c_network_channel_owner
{
public:
	//void describe_status(char* buffer, long buffer_size) const;
	//bool need_to_generate_updates() const;
	long get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier);
	bool boot_machine(s_machine_identifier const* machine_identifier, e_network_session_boot_reason reason);

protected:
	c_simulation_world* m_world;
	c_network_observer* m_observer;
	c_network_session* m_session;
	dword m_session_local_update_number;
	dword m_machine_last_membership_update_number;
	dword m_local_session_membership_update_number;
	dword m_local_machine_valid_mask;
	dword m_local_machine_index;
	s_machine_identifier m_machine_identifiers[17];
	bool m_machine_update_pending;
	s_player_collection m_player_collection;
	dword m_player_collection_machine_valid_mask;
	s_machine_identifier m_player_collection_machine_identifiers[17];
	bool m_changes_pending_acknowledgement;
	bool m_game_revert_occured;
	bool __unknown16656;
	bool __unknown16657;
};
static_assert(sizeof(c_simulation_watcher) == 0xBB58);