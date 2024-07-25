#pragma once
#include <cseries\cseries.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\delivery\network_channel.h>
#include <networking\network_statistics.h>
#include <networking\logic\network_bandwidth.h>

constexpr long k_network_maximum_observers = 34; // 32 in h3debug, 33 in mcc

enum e_network_observer_owner
{
	_network_observer_owner_squad_one = 0,
	_network_observer_owner_squad_two,
	_network_observer_owner_group,
	_network_observer_owner_simulation,

	k_network_observer_owner_count
};

struct s_observer_configuration
{
	byte unknown_data1[0x124];
	long period_duration_msec; // name taken from c_network_time_statistics.m_period_duration_msec // offset 0x1c in time stats
	c_network_window_statistics window_statistics; // either full statistics or just the first field? may also be m_interval_duration_msec from c_network_time_statistics which would be above
	byte unknown_data2[0x38];
	long packet_timeout; // offset 0x270
	byte unknown_data3[0x1A4];
};
static_assert(sizeof(s_observer_configuration) == 0x418);

struct s_network_message_connect_request;
struct s_transport_address;
class c_network_message_type_collection;
class c_network_message_gateway;
class c_network_message_handler;
class c_network_session;
class c_network_link;
class c_network_observer
{
	public:
		struct s_channel_observer_owner
		{
			c_network_session* owner;
			long managed_session_index;
		};
		static_assert(sizeof(s_channel_observer_owner) == 0x8);
		static_assert(0x0 == OFFSETOF(s_channel_observer_owner, owner));
		static_assert(0x4 == OFFSETOF(s_channel_observer_owner, managed_session_index));

		struct s_channel_observer : c_network_channel
		{
			c_enum<e_observer_state, long, k_observer_state_count> state;
			long m_allocated_timestamp1;
			byte flags;
			byte owner_flags;
			ushort __unknownA7E;
			ulong owner_connection_identifier;
			s_transport_secure_address secure_address;
			ulong __unknown1220_flags;
			ulong __unknown1220_flags_bit;
			s_transport_address secure_connection_address;
			// s_transport_secure_identifier secure_connection_identifier;

			long unknown3; // -1 when allocated
			long unknown4; // -1 when allocated
			long unknown5; // set by a global value that is 1000 by default, likely to be a timeout?
			long unknown6; // set by a global value that is 1000 by default, likely to be a timeout?
			long unknown7; // 0
			long m_packet_timeout;
			byte unknown_data1[0x2];
			long m_allocated_timestamp2;
			long unknown8; // 0
			long unknown9; // 0
			c_network_time_statistics time_statistics[3];
			c_network_window_statistics window_statistics[2];
			byte unknown_data2[44];
			bool stream__active; // stream.active
			bool load_bearing;
			bool unknown_bool2;
			bool unknown_bool3;
			byte unknown_data3[1];
			bool synchronous;
			bool joining;
			byte unknown_data4[245];
		};
		static_assert(sizeof(s_channel_observer) == 0x10A8); // 0x10D8 in ms23

		void handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message);
		void observer_channel_initiate_connection(e_network_observer_owner observer_owner, long observer_channel_index);
		const char* get_name(long observer_index);
		bool observer_channel_dead(e_network_observer_owner owner_type, long observer_index);
		s_channel_observer* get_observer(e_network_observer_owner owner_type, long observer_index);
		void observer_channel_send_message(e_network_observer_owner owner_type, long observer_index, bool disconnected, e_network_message_type message_type, long message_size, s_network_message* message);
		bool observer_channel_connected(e_network_observer_owner owner_type, long observer_index);
		bool observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void quality_statistics_get_ratings(ulong* connectivity_badness_rating, ulong* host_badness_rating, ulong* client_badness_rating);
		long observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel* channel); // owner type might be a byte?
		s_channel_observer* find_observer_by_channel(c_network_channel* channel);
		bool is_bandwidth_stable();

		c_network_link* m_link;
		c_network_message_gateway* m_message_gateway;
		c_network_message_handler* m_message_handler;
		c_network_message_type_collection* m_message_type_collection;
		s_observer_configuration* m_configuration;
		s_channel_observer_owner m_owners[k_network_observer_owner_count];
		byte __data[4]; // c_network_session?
		s_channel_observer m_channel_observers[k_network_maximum_observers];
		byte __data2[0xC0]; // m_quality_statistics?
		bool m_bandwidth_unstable;
		byte __data3[0x168];
};
static_assert(sizeof(c_network_observer) == 0x238B8);
static_assert(0x14 == OFFSETOF(c_network_observer, m_owners));
static_assert(0x38 == OFFSETOF(c_network_observer, m_channel_observers));
static_assert(0x23748 == OFFSETOF(c_network_observer, m_bandwidth_unstable));