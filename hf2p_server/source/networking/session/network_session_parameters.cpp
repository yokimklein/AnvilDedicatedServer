#include "network_session_parameters.h"
#include "..\..\dllmain.h"
#include "network_session.h"

void c_network_session_parameters::check_to_send_updates()
{
	void(__thiscall * check_to_send_updates)(c_network_session_parameters* parameters) = reinterpret_cast<decltype(check_to_send_updates)>(module_base + 0x1A390);
	check_to_send_updates(this);
}

void c_network_session_parameters::check_to_send_change_requests()
{
	void(__thiscall * check_to_send_change_requests)(c_network_session_parameters* parameters) = reinterpret_cast<decltype(check_to_send_change_requests)>(module_base + 0x1A7B0);
	check_to_send_change_requests(this);
}

void c_network_session_parameters::update()
{
	auto session = this->session;
	if (session->established())
	{
		if (session->is_host())
			this->check_to_send_updates();
		else
			this->check_to_send_change_requests();
	}
}