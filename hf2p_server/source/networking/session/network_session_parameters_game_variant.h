#pragma once
#include "network_session_parameters_base.h"
#include "..\..\game\game_options.h"
#include "..\..\simulation\simulation.h"

struct s_network_session_parameter_game_variant
{
	long : 32;
	c_game_variant game_variant;
};
static_assert(sizeof(s_network_session_parameter_game_variant) == 0x268);

struct c_network_session_parameter_game_variant : c_network_session_parameter_base
{
	s_network_session_parameter_game_variant m_data;
	s_network_session_parameter_game_variant m_requested_data;
	long m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	long m_next_request_chunk;
	long m_expected_update_chunk;
	long m_expected_change_chunk;
	char chunked_update_data[0x1800];
	long chunked_update_data_size;
	long chunked_update_data_checksum;
	long chunked_update_data_chunk_count;
	char chunked_change_data[0x1800];
	long chunked_change_data_size;
	long chunked_change_data_checksum;
	long chunked_change_data_chunk_count;
	long m_max_chunk_count;
	long : 32;
};
static_assert(sizeof(c_network_session_parameter_game_variant) == 0x3570);