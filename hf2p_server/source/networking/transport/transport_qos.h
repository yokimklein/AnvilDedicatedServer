#pragma once

struct s_transport_qos_result
{
	long probes_sent;
	long probes_received;
	long ping_msec_minimum;
	long ping_msec_median;
	long bandwidth_upstream_bps;
	long bandwidth_downstream_bps;
	long data_block_size;
	void* data_block;
};
static_assert(sizeof(s_transport_qos_result) == 0x20);