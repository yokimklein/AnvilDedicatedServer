#pragma once
#include <cseries\cseries.h>

class c_network_time_statistics
{
private:
	static long const k_network_statistics_interval_count = 20;

public:
	struct s_statistics_interval
	{
		long events;
		long total_values;
	};
	static_assert(sizeof(s_statistics_interval) == 0x8);

	qword m_total_events;
	qword m_total_values;
	ulong m_current_interval_start_timestamp;
	s_statistics_interval m_current_interval;
	long m_period_duration_msec;
	long m_interval_duration_msec;
	real m_period_inverse_seconds;
	long m_next_interval_index;
	s_statistics_interval m_stored_intervals[k_network_statistics_interval_count];
	s_statistics_interval m_stored_total;
};
static_assert(sizeof(c_network_time_statistics) == 0xD8);

class c_network_window_statistics
{
private:
	static long const k_network_statistics_maximum_window_size = 32;

public:

	struct s_statistics_window_entry
	{
		ulong timestamp;
		long value;
	};
	static_assert(sizeof(s_statistics_window_entry) == 0x8);

	long m_window_size;
	long m_window_next_entry;
	s_statistics_window_entry m_window_entries[k_network_statistics_maximum_window_size];
	long m_window_total_values;
	long m_window_aperture_msec;
};
static_assert(sizeof(c_network_window_statistics) == 0x110);