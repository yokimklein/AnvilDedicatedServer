#include "simulation_watcher.h"

long c_simulation_watcher::get_machine_index_by_identifier(s_machine_identifier const* remote_machine_identifier)
{
	assert(remote_machine_identifier);
	return DECLFUNC(0x15620, long, __thiscall, c_simulation_watcher*, s_machine_identifier const*)(this, remote_machine_identifier);
}