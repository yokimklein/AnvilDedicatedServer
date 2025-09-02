#include "network_observer.h"
#include <stdio.h>

char const* k_observer_state_strings[]
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
static_assert(NUMBEROF(k_observer_state_strings) == k_observer_state_count);

char const* k_owner_type_strings[]
{
	"squad one",
	"squad two",
	"group",
	"simulation"
};
static_assert(NUMBEROF(k_owner_type_strings) == k_network_observer_owner_count);

void c_network_observer::handle_connect_request(transport_address const* incoming_address, s_network_message_connect_request const* connect_request)
{
	INVOKE_CLASS_MEMBER(0x10E30, c_network_observer, handle_connect_request, incoming_address, connect_request);
}

void c_network_observer::observer_channel_initiate_connection(e_network_observer_owner observer_owner, long observer_channel_index)
{
	INVOKE_CLASS_MEMBER(0xF970, c_network_observer, observer_channel_initiate_connection, observer_owner, observer_channel_index);
}

const char* c_network_observer::get_name(long observer_index)
{
	s_channel_observer* observer = &m_channel_observers[observer_index];
	ASSERT(observer_index >= 0 && observer_index < k_network_maximum_observers);
	ASSERT(observer->state != _observer_state_none);
	ASSERT(observer->channel.allocated());
	return observer->channel.get_name();
}

bool c_network_observer::observer_channel_dead(e_network_observer_owner owner_type, long observer_index)
{
	return get_observer(owner_type, observer_index)->state == _observer_state_dead;
}

c_network_observer::s_channel_observer* c_network_observer::get_observer(e_network_observer_owner owner_type, long observer_index)
{
	s_channel_observer* observer = &m_channel_observers[observer_index];
	ASSERT(observer_index >= 0 && observer_index < k_network_maximum_observers);
	ASSERT(owner_type >= 0 && owner_type < k_network_observer_owner_count);
	ASSERT(observer->state > _observer_state_none && observer->state < k_observer_state_count);
	ASSERT(TEST_BIT(observer->owner_flags, owner_type));
	return observer;
}

void c_network_observer::observer_channel_send_message(e_network_observer_owner owner_type, long observer_channel_index, bool out_of_band, e_network_message_type message_type, long message_size, void const* message)
{
	INVOKE_CLASS_MEMBER(0xF440, c_network_observer, observer_channel_send_message, owner_type, observer_channel_index, out_of_band, message_type, message_size, message);
}

bool c_network_observer::observer_channel_connected(e_network_observer_owner owner_type, long observer_index)
{
	return get_observer(owner_type, observer_index)->state == _observer_state_connected;
}

bool c_network_observer::observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	return INVOKE_CLASS_MEMBER(0xF880, c_network_observer, observer_channel_backlogged, owner_type, observer_index, message_type);
}

void c_network_observer::observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type)
{
	INVOKE_CLASS_MEMBER(0xF900, c_network_observer, observer_channel_set_waiting_on_backlog, owner_type, observer_index, message_type);
}

void c_network_observer::quality_statistics_get_ratings(ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating)
{
	INVOKE_CLASS_MEMBER(0xEF30, c_network_observer, quality_statistics_get_ratings, connectivity_badness_rating, host_badness_rating, client_badness_rating);
}

long c_network_observer::observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel const* channel) const
{
	ASSERT(owner_type >= 0 && owner_type < k_network_observer_owner_count);
	ASSERT(channel != NULL);

	s_channel_observer const* observer = find_observer_by_channel(channel);
	ASSERT(observer != NULL);
	
	if (observer->state == _observer_state_none || !TEST_BIT(observer->owner_flags, owner_type))
	{
		return NONE;
	}
	
	return observer - m_channel_observers;
}

c_network_observer::s_channel_observer const* c_network_observer::find_observer_by_channel(c_network_channel const* channel) const
{
	s_channel_observer const* observer = (c_network_observer::s_channel_observer*)channel;
	ASSERT(observer >= &m_channel_observers[0] && observer < &m_channel_observers[k_network_maximum_observers]);
	ASSERT(((byte *)observer - (byte *)&m_channel_observers[0]) % sizeof(s_channel_observer) == 0);
	return observer;
}

bool c_network_observer::is_bandwidth_stable()
{
	return m_expansion_state == 0;
}

void c_network_observer::quality_statistics_notify_peer_left_gracefully(e_network_observer_owner owner, long observer_index)
{
	s_channel_observer const* observer = get_observer(owner, observer_index);
	if (observer->stream.active && observer->stream.is_simulation)
	{
		printf("MP/NET/OBSERVER,QUALITY: c_network_observer::quality_statistics_notify_peer_left_gracefully: [%s] peer left gracefully, reporting as 'good'\n", get_name(observer_index));
		// originally the channel index was calculated like this:
		// sizeof(s_channel_observer) * channel_index / sizeof(s_channel_observer)
		// I have no idea why they did this because this is entirely pointless
		quality_statistics_report_badness(observer_index, false/*, "peer left gracefully"*/);
	}
}

void c_network_observer::quality_statistics_report_badness(long observer_index, bool bad_not_good)
{
	INVOKE_CLASS_MEMBER(0xF160, c_network_observer, quality_statistics_report_badness, observer_index, bad_not_good);
}
