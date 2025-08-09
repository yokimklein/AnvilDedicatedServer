#pragma once
#include <cseries\cseries.h>

enum e_bitstream_state
{
	_bitstream_state_none = 0,

	_bitstream_state_write,
	_bitstream_state_write_finished,

	_bitstream_state_read,
	_bitstream_state_read_consistency_check,
	_bitstream_state_read_finished,

	k_bitstream_state_count
};

class c_bitstream
{
public:
	bool was_writing() const;
	bool would_overflow(long size_in_bits) const;
	void write_accumulator_to_memory(qword value, long size_in_bits);
	void write_bit_internal(bool value);
	void write_bool(const char* debug_string, bool value);
	void finish_writing(long* bits_wasted);
	void reset(long state);
	bool begin_consistency_check();
	bool read_bool(const char* debug_string);
	void finish_reading();
	bool reading() const;
	bool writing() const;

private:
	static long const k_bitstream_maximum_position_stack_size = 4;

protected:
	struct s_bitstream_stack_entry
	{
		long current_memory_bit_position;
		long current_stream_bit_position;
		qword accumulator;
		long accumulator_bit_count;
		byte* next_data;
	};

	byte* m_data;
	byte* m_data_max;
	long m_data_size_bytes;
	long m_data_size_alignment;
	c_enum<e_bitstream_state, long, _bitstream_state_none, k_bitstream_state_count> m_state;
	bool m_data_error_detected;
	s_bitstream_stack_entry m_bitstream_data;
	long m_position_stack_depth;

	// why is this between `m_position_stack_depth` and `m_position_stack`, is `m_position_stack_depth` actually a qword?
	dword __unknown34; // long m_potential_bit_position?

	s_bitstream_stack_entry m_position_stack[k_bitstream_maximum_position_stack_size];
	long m_number_of_bits_rewound;
	long m_number_of_position_resets;
};
static_assert(sizeof(c_bitstream) == 0xA0);
