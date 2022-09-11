#pragma once
#include "network_session_parameters_base.h"
#include "..\..\simulation\simulation.h"
#include "..\..\saved_games\saved_game_files.h"

struct s_network_session_parameter_map_variant
{
	long : 32;
	long : 32;
	c_map_variant map_variant;
};

class c_network_session_parameter_map_variant : public c_network_session_parameter_base
{
	s_network_session_parameter_map_variant m_data;
	s_network_session_parameter_map_variant m_requested_data;
	long m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	long m_next_request_chunk;
	long m_expected_update_chunk;
	long m_expected_change_chunk;
	char chunked_update_data[0x7000];
	long chunked_update_data_size;
	long chunked_update_data_checksum;
	long chunked_update_data_chunk_count;
	char chunked_change_data[0x7000];
	long chunked_change_data_size;
	long chunked_change_data_checksum;
	long chunked_change_data_chunk_count;
	long m_max_chunk_count;
	long : 32;
};