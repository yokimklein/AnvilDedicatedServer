#pragma once
#include <cseries\cseries.h>
#include <replication\replication_encoding.h>
#include <game\game_results.h>
#include <networking\replication\replication_scheduler.h>

long const k_network_maximum_control_streams = 32;

class c_simulation_view_telemetry_provider;
class c_replication_control_view : public c_replication_scheduler_client
{
	bool m_initialized;
	c_simulation_view_telemetry_provider* m_telemetry_provider;

	dword_flags m_motion_available_send;
	dword_flags m_motion_available_receive;
	s_player_motion m_motion[k_network_maximum_control_streams];
	dword m_motion_timestamp[k_network_maximum_control_streams];

	dword m_prediction_available_send;
	dword m_prediction_available_receive;
	s_player_prediction m_prediction[k_network_maximum_control_streams];
};
static_assert(sizeof(c_replication_control_view) == 0x1320);
