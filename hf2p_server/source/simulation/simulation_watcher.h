#pragma once
#include <cseries\cseries.h>
#include <game\player_configuration.h>
#include <simulation\simulation_players.h>
#include <networking\delivery\network_channel.h>

class c_simulation_world;
class c_network_observer;
class c_network_session;
class c_simulation_watcher : public c_network_channel_owner
{
public:
	//void describe_status(char* buffer, long buffer_size) const;
	//bool need_to_generate_updates() const;

protected:
	c_simulation_world* m_world;
	c_network_observer* m_observer;
	c_network_session* m_session;
	dword m_session_local_update_number;
	dword m_machine_last_membership_update_number;
	dword m_local_session_membership_update_number;
	dword m_local_machine_valid_mask;
	dword m_local_machine_index;
	s_machine_identifier m_local_machine_identifiers[17];
	bool __unknown134;
	s_player_collection player_collection;
	dword m_machine_valid_mask;
	s_machine_identifier m_machine_identifiers[17];
	bool __unknown16654;
	bool m_game_reverted;
	bool __unknown16656;
	bool __unknown16657;
};
static_assert(sizeof(c_simulation_watcher) == 0xBB58);