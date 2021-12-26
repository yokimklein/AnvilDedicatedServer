#pragma once
#include "..\transport\transport_address.h"

class c_network_channel;
class c_network_link
{
public:
	c_network_channel* get_associated_channel(s_transport_address const* address);

	// based off h3debug - confirm for ms29
	bool m_initialized;
	// 11 pointers after this point?
	// 1: channel identifier


	// 4: struct transport_endpoint (_network_packet_mode_channel)
	// 5: struct transport_endpoint (_network_packet_mode_out_of_band) k_network_packet_mode_count = 2
	

	// 8: c_network_time_statistics
	
	// 12: non pointer class field?
	// 
	// 62: c_network_time_statistics

	// 116: c_network_time_statistics

	// 170: c_network_time_statistics
	
	// 172: incoming c_network_simulation_queue
	// 
	// 224: char*
	// 
	// 240: c_network_simulation_state*
	// 
	// 380: outgoing c_network_simulation_queue

	//s_transport_address m_address; // urgent we get this one sorted
};
//static_assert(sizeof(c_network_link) == 0x0);