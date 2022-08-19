#pragma once
#include <windows.h>
#include "..\messages\network_message_type_collection.h"
#include "..\delivery\network_channel.h"
#include "..\network_statistics.h"

constexpr long k_network_maximum_observers = 34; // 32 in h3debug, 33 in mcc?

enum e_observer_state : long
{
	_observer_state_none = 0,
	_observer_state_dead,
	_observer_state_idle,
	_observer_state_securing,
	_observer_state_waiting,
	_observer_state_ready,
	_observer_state_connecting,
	_observer_state_connected,
	_observer_state_reconnecting,
	_observer_state_disconnected,

	k_observer_state_count
};

static const char* k_observer_state_strings[k_observer_state_count]
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

enum e_network_observer_owner : long
{
	_network_observer_owner_squad_one = 0,
	_network_observer_owner_squad_two,
	_network_observer_owner_group,
	_network_observer_owner_simulation,

	k_network_observer_owner_count
};

static const char* k_owner_type_strings[k_network_observer_owner_count]
{
	"squad one",
	"squad two",
	"group",
	"simulation"
};

struct s_observer_configuration
{

};

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
			e_network_observer_owner owner;
			long managed_session_index;
		};
		static_assert(sizeof(s_channel_observer_owner) == 0x8);

		struct s_channel_observer
		{
			c_network_channel channel;
			e_observer_state state;
			uint32_t unknown1;
			byte flags;
			byte owner_flags;
			uint16_t __unknownA7E;
			uint32_t owner_connection_identifier;
			s_transport_secure_address secure_address;
			uint32_t __unknown1220_flags;
			uint32_t __unknown1220_flags_bit;
			s_transport_address secure_connection_address;
			byte unknown_data1[26];
			uint32_t time_unknown;
			uint32_t unknown2;
			uint32_t unknown3;
			c_network_time_statistics time_statistics[3];
			c_network_window_statistics window_statistics[2];
			byte unknown_data2[294];
		};
		static_assert(sizeof(s_channel_observer) == 0x10A8); // 0x10D8 in ms23

		void handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message);
		void observer_channel_initiate_connection(e_network_observer_owner observer_owner, int observer_channel_index);
		const char* get_name(long observer_index);
		bool observer_channel_dead(e_network_observer_owner owner_type, long observer_index);
		s_channel_observer* get_observer(e_network_observer_owner owner_type, long observer_index);
		void observer_channel_send_message(e_network_observer_owner owner_type, long observer_index, bool disconnected, e_network_message_type message_type, long message_size, s_network_message* message);
		bool observer_channel_connected(e_network_observer_owner owner_type, long observer_index);
		bool observer_channel_backlogged(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void observer_channel_set_waiting_on_backlog(e_network_observer_owner owner_type, long observer_index, e_network_message_type message_type);
		void quality_statistics_get_ratings(uint32_t* connectivity_badness_rating, uint32_t* host_badness_rating, uint32_t* client_badness_rating);
		long observer_channel_find_by_network_channel(e_network_observer_owner owner_type, c_network_channel* channel); // owner type might be a byte?

		c_network_link* m_link;
		c_network_message_gateway* m_message_gateway;
		c_network_message_handler* m_message_handler;
		c_network_message_type_collection* m_message_type_collection;
		s_observer_configuration* m_configuration;
		c_network_session* m_session;
		s_channel_observer_owner m_owners[k_network_observer_owner_count];
		s_channel_observer m_channel_observers[k_network_maximum_observers]; // offset 0x38
		byte unknown_data[0x238];
};
static_assert(sizeof(c_network_observer) == 0x238C0); // TODO - verify size
/*

class c_network_observer : c_network_channel
{
	struct s_channel_observer_owner
	{
		e_network_observer_owner owner;
		long managed_session_index;
	};

	struct s_channel_observer
	{
		// c_network_channel*?
		// 14 = state
		// 1152 byte security_flags[k_network_maximum_observers]
		// 1550 byte stream[0xD8]; byte array? struct is size 0xD8 (contains active, (+2 bytes is_simulation)
		// 1604 bool badness_reported_good_stream
		// size 0x1920 in h3debug

		// size 1078 in ms23
		// 0 void*
		// 76 c_network_time_statistics?
		// 234 bool?
		// 256 long?
		// 508 c_network_time_statistics*
		// 685 owner_flags
		// 686 
	};

	// ms23 (using offsets of 4 bytes)
	// 687: c_network_channel m_channel_observers[k_network_maximum_observers];

	// h3debug:
	// c_network_link* m_link;
	// c_network_message_gateway* m_message_gateway;
	// c_network_message_handler* m_message_handler;
	// c_network_message_type_collection* m_message_type_collection;
	// void* m_unknown;
	// s_network_observer_owner m_owners[k_network_observer_owner_count];
	// s_channel_observer m_channel_observers[k_network_maximum_observers]; // starts at byte offset 52
	// byte rest_of_struct[0x258];
	// 51456 - value after observers array
	// 51470 - bool quality_statistics_set
	// 51472 s_network_quality_statistics (sizeof = 192)
	// 51520 bool is_bandwidth_stable;
	// 51523/3? bool online_network_environment? probably the same bool as above
	// 51524 = 0x100000? estimated bandwidth?
	// 51525 - long bandwidth_successful_bps
	// 51526 - long bandwidth_unsafe_bps
	// 51528 - 0
	// 51530 - long bandwidth something bps?
	// 51535 - something +312 h3debug in c_network_session
	// 51536 - 1024
	// 51537 - 
	// 51538 - c_network_time_statistics*
	// 51592
	// 51593
	// 51594 - float
	// 51595 - float
	// 51598 - network_time_get()
	// 51599 - network_time_get()
	// 51602.5? - bool
	// 51606 - network_time_get()
};

*/