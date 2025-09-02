#pragma once
#include <cseries\cseries.h>
#include <networking\delivery\network_link.h>
#include <networking\messages\network_message_type_collection.h>
#include <networking\messages\network_message_handler.h>
#include <networking\messages\network_message_gateway.h>
#include <networking\session\network_observer.h>
#include <networking\session\network_session_manager.h>
#include <networking\session\network_session.h>
#include <networking\session\network_session_parameter_type_collection.h>
#include <simulation\simulation_world.h>
#include <simulation\simulation_watcher.h>
#include <simulation\simulation_type_collection.h>
#include <networking\network_globals.h>

class c_simulation_distributed_world;
struct s_data_array;
class c_rockall_heap;
struct s_network_shared_memory_globals
{
	long current_configuration;
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
	c_network_session sessions[k_network_maximum_sessions];
	c_network_session_parameter_type_collection sesssion_parameter_types;
	c_simulation_world simulation_world;
	c_simulation_watcher simulation_watcher;
	c_simulation_type_collection simulation_types;
};
static_assert(sizeof(s_network_base_memory_globals) == 0x535F58);
static_assert(0x0 == OFFSETOF(s_network_base_memory_globals, link));
static_assert(0x378 == OFFSETOF(s_network_base_memory_globals, message_types));
static_assert(0x8B0 == OFFSETOF(s_network_base_memory_globals, message_gateway));
static_assert(0xE30 == OFFSETOF(s_network_base_memory_globals, message_handler));
static_assert(0xE48 == OFFSETOF(s_network_base_memory_globals, observer));
static_assert(0x24700 == OFFSETOF(s_network_base_memory_globals, session_manager));
static_assert(0x24710 == OFFSETOF(s_network_base_memory_globals, sessions));
static_assert(0x5288D8 == OFFSETOF(s_network_base_memory_globals, sesssion_parameter_types));
static_assert(0x528D38 == OFFSETOF(s_network_base_memory_globals, simulation_world));
static_assert(0x52A278 == OFFSETOF(s_network_base_memory_globals, simulation_watcher));
static_assert(0x535DD0 == OFFSETOF(s_network_base_memory_globals, simulation_types));

extern s_network_shared_memory_globals& network_shared_memory_globals;
extern s_network_base_memory_globals& network_base_memory_globals;

void* network_heap_allocate_block(long block_size);
void network_heap_free_block(void* block);
char* network_heap_describe(char* string, long length);
