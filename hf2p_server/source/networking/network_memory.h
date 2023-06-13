#pragma once
#include "..\cseries\cseries.h"
#include "delivery\network_link.h"
#include "messages\network_message_type_collection.h"
#include "messages\network_message_handler.h"
#include "messages\network_message_gateway.h"
#include "session\network_observer.h"
#include "session\network_session_manager.h"
#include "session\network_session.h"
#include "session\network_session_parameter_type_collection.h"
#include "..\simulation\simulation_world.h"
#include "..\simulation\simulation_watcher.h"
#include "..\simulation\simulation_type_collection.h"

class c_simulation_distributed_world;
struct s_data_array;
class c_rockall_heap;
struct s_network_shared_memory_globals
{
	bool initialized;
	long allocation_size;
	void* allocation_low_address;
	void* allocation_high_address;
	bool distributed_simulation_available;
	c_simulation_distributed_world* simulation_distributed_world;
	s_data_array* simulation_view_data_array;
	s_data_array* simulation_distributed_view_data_array;
	long heap_size;
	c_rockall_heap* heap;
	void* heap_buffer;
	dword : 32;

	// not in release builds
	//s_network_memory_status total_status;
	//s_network_memory_status block_status[21];
};
static_assert(sizeof(s_network_shared_memory_globals) == 0x30);

struct s_network_base_memory_globals
{
	c_network_link link;
	c_network_message_type_collection message_types;
	dword : 32;
	c_network_message_gateway message_gateway;
	c_network_message_handler message_handler;
	c_network_observer observer;
	c_network_session_manager session_manager;
	dword : 32;
	c_network_session sessions[3];
	c_network_session_parameter_type_collection sesssion_parameter_types;
	c_simulation_world simulation_world;
	c_simulation_watcher simulation_watcher;
	c_simulation_type_collection simulation_types;
};
static_assert(sizeof(s_network_base_memory_globals) == 0x540A60); // TODO: confirm size

void* network_heap_allocate_block(long block_size);
void network_heap_free_block(void* block);
char* network_heap_describe(char* string, long length);

static s_network_shared_memory_globals* network_shared_memory_globals = (s_network_shared_memory_globals*)base_address(0x1038284);