#include "simulation_watcher.h"
#include <networking\session\network_session.h>

long c_simulation_watcher::get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier)
{
	assert(remote_machine_identifier);
	return DECLFUNC(0x15620, long, __thiscall, c_simulation_watcher*, s_machine_identifier const*)(this, remote_machine_identifier);
}

bool c_simulation_watcher::boot_machine(s_machine_identifier const* machine_identifier, e_network_session_boot_reason reason)
{
	long peer_index = this->m_session->find_peer_by_machine_identifier(machine_identifier);
	if (peer_index == -1)
		return false;
	else
		return this->m_session->host_boot_machine(peer_index, reason);
}