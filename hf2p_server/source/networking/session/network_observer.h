#pragma once
#include "..\delivery\network_channel.h"
#include "..\delivery\network_link.h"
#include "..\messages\network_message_type_collection.h"

constexpr long k_network_maximum_observers = 34; // 32 in h3debug, 33 in mcc?

enum e_observer_state : long
{
	_observer_state_none,
	_observer_state_connected = 7,

	k_observer_state_count
};

enum e_network_observer_owner : long
{
	k_network_observer_owner_count = 4
};

class c_network_session;
class c_network_observer
{
	public:
		char handle_connect_request(s_transport_address const* address, s_network_message_connect_request const* message);

		//struct s_channel_observer
		//{
		//	// starts with c_network_channel?
		//	c_network_channel channel;
		//	byte __data[0x62C];
		//};
		//static_assert(sizeof(s_channel_observer) == 0x10A8);

		c_network_link* m_link;
		c_network_message_gateway* m_message_gateway;
		c_network_message_handler* m_message_handler;
		c_network_message_type_collection* m_message_type_collection;
		void* m_unknown;
		c_network_session* m_session;
		long m_unknown2; // e_network_observer_owner? managed_session_index?
		void* m_unknown3;
		void* m_unknown4; // null?
		void* m_unknown5;
		void* m_unknown6; // null?
		long* m_unknown7;
		void* m_unknown8; // null?

		//s_channel_observer channels[k_network_maximum_observers];
		//byte __data[0x62C];
};
//static_assert(sizeof(c_network_observer) == 0x23C7C); // TODO - verify if this size is accurate 

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