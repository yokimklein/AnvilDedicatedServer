#pragma once
#include <cseries\cseries.h>

struct s_network_bandwidth_persistent_data
{
	long qos_sample_count;
	long qos_samples[8];

	long bandwidth_measurement_count;
	long bandwidth_measurement_successful_bps[8];
	long bandwidth_measurement_unsafe_bps[8];

	// removed since ms23/h3
	//long bandwidth_dispute_count;
};
static_assert(sizeof(s_network_bandwidth_persistent_data) == 0x68);

struct s_network_quality_session_statistics
{
	ulong64 client_badness_history[2];
	ulong64 host_badness_history[2];
};
static_assert(sizeof(s_network_quality_session_statistics) == 0x20); 

struct s_network_quality_statistics
{
	s_network_quality_session_statistics session[2];
	ulong64 connectivity_history[2];
	s_network_bandwidth_persistent_data bandwidth_data;
};
static_assert(sizeof(s_network_quality_statistics) == 0xB8);
