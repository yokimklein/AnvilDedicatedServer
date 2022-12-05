#include "network_observer.h"
#include "..\..\dllmain.h"

char const* k_observer_state_strings[k_observer_state_count]
{
	"none",
	"dead",
	"idle",
	"securing",
	"waiting",
	"ready",
	"connecting",
	"connected",
	"reconnecting",
	"disconnected"
};

char const* k_owner_type_strings[k_network_observer_owner_count]
{
	"squad one",
	"squad two",
	"group",
	"simulation"
};

void c_network_observer::handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message)
{
	typedef void(__thiscall* c_network_observer__handle_connect_request_ptr)(c_network_observer* observer, s_transport_address const* address, s_network_message_connect_request const* message);
	static auto c_network_observer__handle_connect_request = reinterpret_cast<c_network_observer__handle_connect_request_ptr>(module_base + 0x10E30);
	c_network_observer__handle_connect_request(this, address, message);
}

void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner observer_owner, int observer_channel_index)
{
	typedef void(__thiscall* observer_channel_initiate_connection_call_ptr)(c_network_observer* observer, int observer_owner, int observer_channel_index);
	static auto observer_channel_initiate_connection_call = reinterpret_cast<observer_channel_initiate_connection_call_ptr>(module_base + 0xF970);
	return observer_channel_initiate_connection_call(this, observer_owner, observer_channel_index);
}

// FUNC TODO
const char* c_network_observer::get_name(long observer_index)
{
	// get channel from observer index
	// call c_network_channel::get_name
	return "<channel-name>";
}

bool c_network_observer::observer_channel_dead(e_network_observer_owner owner_type, long observer_index)
{
	return this->get_observer(owner_type, observer_index)->state == _observer_state_dead;
}

c_network_observer::s_channel_observer* c_network_observer::get_observer(e_network_observer_owner owner_type, long observer_index)
{
	// TODO ASSERTS
	return &this->m_channel_observers[observer_index];
}

void c_network_observer::observer_channel_send_message(e_network_observer_owner owner_type, long observer_index, bool disconnected, e_network_message_type message_type, long message_size, s_network_message* message)
{
	void(__thiscall* observer_channel_send_message)(c_network_observer* observer, e_network_observer_owner owner_type, long observer_index, bool disconnected, e_network_message_type message_type, long message_size, s_network_message* message) = reinterpret_cast<decltype(observer_channel_send_message)>(module_base + 0xF440);
	observer_channel_send_message(this, owner_type, observer_index, disconnected, message_type, message_size, message);
}

bool c_network_observer::observer_channel_connected(e_network_observer_owner owner_type, long observer_index)
{
	return this->get_observer(owner_type, observer_index)->state == _observer_state_connected;
}

bool c_network_observer::observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	bool(__thiscall* observer_channel_backlogged)(c_network_observer* observer, e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type) = reinterpret_cast<decltype(observer_channel_backlogged)>(module_base + 0xF880);
	return observer_channel_backlogged(this, owner_type, observer_index, message_type);
}

void c_network_observer::observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	void(__thiscall* observer_channel_set_waiting_on_backlog)(c_network_observer* observer, e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type) = reinterpret_cast<decltype(observer_channel_set_waiting_on_backlog)>(module_base + 0xF900);
	observer_channel_set_waiting_on_backlog(this, owner_type, observer_index, message_type);
}

void c_network_observer::quality_statistics_get_ratings(ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating)
{
	void(__thiscall * quality_statistics_get_ratings)(c_network_observer* observer, ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating) = reinterpret_cast<decltype(quality_statistics_get_ratings)>(module_base + 0xEF30);
	quality_statistics_get_ratings(this, connectivity_badness_rating, host_badness_rating, client_badness_rating);
}

// TODO - figure out what this actually is
// TODO TEST!!!
long c_network_observer::observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel* channel)
{
	//auto* channel_observer = this->find_observer_by_channel(channel);
	long next_channel = *((long*)channel + 0x29D) == 0;  // 0xA74
	byte* channel_address = (byte*)channel;
	if (!next_channel && ((unsigned __int8)(1 << (long)owner_type) & channel_address[0xA7D]) != 0)
		return (channel_address - (byte*)this - 56) / 0x10A8;
	return -1;
}