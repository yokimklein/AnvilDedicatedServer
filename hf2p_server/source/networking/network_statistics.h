#pragma once
#include <cseries\cseries.h>
#include <memory\bitstream.h>

class c_network_time_statistics
{
private:
	static long const k_network_statistics_interval_count = 20;

public:
	struct s_interval // #TODO: find a better name?
	{
		// names based on `m_stored_total` asserts
		// the struct for `m_intervals` could be different?

		long events;
		long total_values;
	};
	static_assert(sizeof(s_interval) == 0x8);

	qword m_total_events;
	qword m_total_values;

	long __time10;
	s_interval __unknown14;

	long m_period_duration_msec;
	long m_interval_duration_msec;
	real m_period_duration;
	long m_next_interval_index;
	s_interval m_intervals[k_network_statistics_interval_count]; // #TODO: find a better name?
	s_interval m_stored_total;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);

class c_network_window_statistics
{
	byte data[0x110];
};
static_assert(sizeof(c_network_window_statistics) == 0x110);