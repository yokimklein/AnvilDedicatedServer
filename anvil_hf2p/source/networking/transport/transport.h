#pragma once

typedef void(__cdecl* transport_startup_function_t)(void*);
typedef void(__cdecl* transport_shutdown_function_t)(void*);
typedef void(__cdecl* transport_reset_function_t)(void*);

struct s_transport_globals
{
	bool initialized;
	bool winsock_initialized;
	bool is_started;
	bool is_wireless;
	long transition_function_count;
	transport_startup_function_t startup_functions[8];
	transport_shutdown_function_t shutdown_functions[8];
	transport_reset_function_t reset_functions[8];
	void* callback_datas[8];
};
static_assert(sizeof(s_transport_globals) == 0x88);

extern s_transport_globals& transport_globals;

void transport_initialize();
void transport_global_update();
bool transport_network_available();
void transport_startup();