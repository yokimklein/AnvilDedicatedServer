#pragma once
#include <cstdint>
#include <windows.h>
#include "network_session_parameters_base.h"
#include "..\..\game\game_options.h"
#include "..\..\simulation\simulation.h"

struct s_saved_film_description
{
	byte data[796];
};

struct c_generic_network_session_parameter_saved_film_description : c_network_session_parameter_base
{
	s_saved_film_description m_data;
	s_saved_film_description m_requested_data;
};

struct s_network_session_parameter_saved_film_game_options
{
	long playback_length_in_ticks;
	long playback_start_tick;
	s_game_options game_options;
};

struct c_network_session_parameter_saved_film_game_options : c_network_session_parameter_base
{
	s_network_session_parameter_saved_film_game_options m_data;
	s_network_session_parameter_saved_film_game_options m_requested_data;
	long m_peer_consumed_chunks[k_network_maximum_machines_per_session];
	long m_next_request_chunk;
	long m_expected_update_chunk;
	long m_expected_change_chunk;
	char chunked_update_data[0x8800];
	long chunked_update_data_size;
	long chunked_update_data_checksum;
	long chunked_update_data_chunk_count;
	char chunked_change_data[0x8800];
	long chunked_change_data_size;
	long chunked_change_data_checksum;
	long chunked_change_data_chunk_count;
	long m_max_chunk_count;
	long : 32;
};