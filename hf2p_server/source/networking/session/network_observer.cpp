#include "network_observer.h"
#include "assert.h"

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
	DECLFUNC(0x10E30, void, __thiscall, c_network_observer*, s_transport_address const*, s_network_message_connect_request const*)(this, address, message);
}

void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner observer_owner, long observer_channel_index)
{
	DECLFUNC(0xF970, void, __thiscall, c_network_observer*, e_network_observer_owner, long)(this, observer_owner, observer_channel_index);
}

const char* c_network_observer::get_name(long observer_index)
{
	s_channel_observer* observer = &this->m_channel_observers[observer_index];
	assert(observer_index >= 0 && observer_index < k_network_maximum_observers);
	assert(observer->state != _observer_state_none);
	assert(observer->allocated());
	return observer->get_name();
}

bool c_network_observer::observer_channel_dead(e_network_observer_owner owner_type, long observer_index)
{
	return this->get_observer(owner_type, observer_index)->state == _observer_state_dead;
}

c_network_observer::s_channel_observer* c_network_observer::get_observer(e_network_observer_owner owner_type, long observer_index)
{
	s_channel_observer* observer = &this->m_channel_observers[observer_index];
	assert(observer_index >= 0 && observer_index < k_network_maximum_observers);
	assert(owner_type >= 0 && owner_type < k_network_observer_owner_count);
	assert(observer->state > _observer_state_none && observer->state < k_observer_state_count);
	assert(TEST_BIT(observer->owner_flags, owner_type));
	return observer;
}

void c_network_observer::observer_channel_send_message(e_network_observer_owner owner_type, long observer_index, bool disconnected, e_network_message_type message_type, long message_size, s_network_message* message)
{
	DECLFUNC(0xF440, void, __thiscall, c_network_observer*, e_network_observer_owner, long, bool, e_network_message_type, long, s_network_message*)(this, owner_type, observer_index, disconnected, message_type, message_size, message);
}

bool c_network_observer::observer_channel_connected(e_network_observer_owner owner_type, long observer_index)
{
	return this->get_observer(owner_type, observer_index)->state == _observer_state_connected;
}

bool c_network_observer::observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	return DECLFUNC(0xF880, bool, __thiscall, c_network_observer*, e_network_observer_owner, long, e_network_message_type)(this, owner_type, observer_index, message_type);
}

void c_network_observer::observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	DECLFUNC(0xF900, void, __thiscall, c_network_observer*, e_network_observer_owner, long, e_network_message_type)(this, owner_type, observer_index, message_type);
}

void c_network_observer::quality_statistics_get_ratings(ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating)
{
	DECLFUNC(0xEF30, void, __thiscall, c_network_observer*, ulong*, ulong*, ulong*)(this, connectivity_badness_rating, host_badness_rating, client_badness_rating);
}

long c_network_observer::observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel* channel)
{
	assert(owner_type >= 0 && owner_type < k_network_observer_owner_count);
	assert(channel != NULL);
	s_channel_observer* observer = this->find_observer_by_channel(channel);
	assert(observer != NULL);
	
	if (observer->state != _observer_state_none && TEST_BIT(observer->owner_flags, owner_type))
		return observer - m_channel_observers;
	else
		return NONE;
}

c_network_observer::s_channel_observer* c_network_observer::find_observer_by_channel(c_network_channel* channel)
{
	s_channel_observer* observer = (c_network_observer::s_channel_observer*)channel;
	assert(observer >= &m_channel_observers[0] && observer < &m_channel_observers[k_network_maximum_observers]);
	assert(((byte*)observer - (byte*)&m_channel_observers[0]) % sizeof(s_channel_observer) == 0);
	return observer;
}

bool c_network_observer::is_bandwidth_stable()
{
	return this->m_bandwidth_unstable == false;
}