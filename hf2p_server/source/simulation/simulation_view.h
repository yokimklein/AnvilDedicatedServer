#pragma once
#include "..\cseries\cseries.h"
#include "..\game\players.h"
#include "..\networking\delivery\network_channel.h"

enum e_simulation_view_establishment_mode : long
{
	_simulation_view_establishment_mode_none = -1,
	_simulation_view_establishment_mode_detached,
	_simulation_view_establishment_mode_connect,
	_simulation_view_establishment_mode_established,
	_simulation_view_establishment_mode_ready,
	_simulation_view_establishment_mode_joining,
	_simulation_view_establishment_mode_active,

	k_simulation_view_establishment_mode_count
};

struct c_simulation_distributed_view
{
	byte __data[0x22948];
};
static_assert(sizeof(c_simulation_distributed_view) == 0x22948);

class c_simulation_world;
class c_network_observer;
class c_simulation_view
{
public:
	void set_view_establishment(e_simulation_view_establishment_mode establishment_mode, long establishment_identifier);

	long unknown1;
	long m_view_type;
	long m_view_datum_index;
	c_simulation_distributed_view* m_distributed_view;
	c_simulation_world* m_world;
	long m_world_view_index;
	s_machine_identifier m_remote_machine_identifier;
	long m_remote_machine_index;
	c_network_observer* m_observer;
	long m_observer_channel_index;
	long m_view_death_reason;
	long m_view_establishment_mode;
	long m_view_establishment_identifier;
	long m_valid_view_establishment_identifier;
	long unknown2;
	c_network_channel* m_channel;
	long m_channel_connection_identifier;
	c_network_channel_simulation_interface m_simulation_interface;
	byte __data84[0x3C]; // signature data?
};
static_assert(sizeof(c_simulation_view) == 0xC0);