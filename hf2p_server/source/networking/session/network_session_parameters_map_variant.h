#pragma once
#include <windows.h>
#include "network_session_parameters_base.h"
#include "..\..\scenario\scenario_map_variant.h"
#include "..\..\simulation\simulation.h"

struct s_network_session_parameter_map_variant
{
	long : 32;
	long : 32;
	c_map_variant map_variant;
};

struct c_network_session_parameter_map_variant : c_network_session_parameter_base
{
	s_network_session_parameter_map_variant m_data;
	s_network_session_parameter_map_variant m_requested_data;
	int m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	int m_next_request_chunk;
	int m_expected_update_chunk;
	int m_expected_change_chunk;
	char chunked_update_data[28672];
	int chunked_update_data_size;
	int chunked_update_data_checksum;
	int chunked_update_data_chunk_count;
	char chunked_change_data[28672];
	int chunked_change_data_size;
	int chunked_change_data_checksum;
	int chunked_change_data_chunk_count;
	int m_max_chunk_count;
	long : 32;
};