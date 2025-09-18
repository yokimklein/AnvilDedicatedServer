#include "bitstream.h"

bool c_bitstream::was_writing() const
{
	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_write, _bitstream_state_write_finished);
}

bool c_bitstream::would_overflow(long size_in_bits) const
{
	ASSERT(reading() || writing());
	return size_in_bits + m_bitstream_data.current_stream_bit_position > CHAR_BITS * m_data_size_bytes;
}

void c_bitstream::write_accumulator_to_memory(qword value, long size_in_bits)
{
	DECLFUNC(0xA8110, void, __thiscall, c_bitstream*, qword, long)(this, value, size_in_bits);

	//int32 shift_bits = QWORD_BITS - m_bitstream_data.accumulator_bit_count;
	//int32 accumulator_bit_count = size_in_bits - shift_bits;
	//
	//m_bitstream_data.current_stream_bit_position += size_in_bits;
	//m_bitstream_data.accumulator = value;
	//m_bitstream_data.accumulator_bit_count = accumulator_bit_count;
	//
	//uns64 accumulator = value;
	//if (accumulator_bit_count < QWORD_BITS)
	//{
	//	uns64 shifted_accumulator = left_shift_fast<uns64>(m_bitstream_data.accumulator, shift_bits);
	//	accumulator = shifted_accumulator | (value >> accumulator_bit_count);
	//}
	//
	//c_bitstream::encode_qword_to_memory(accumulator, QWORD_BITS);
}

void c_bitstream::write_bit_internal(bool value)
{
	ASSERT(writing());
	
	if (m_bitstream_data.accumulator_bit_count >= QWORD_BITS)
	{
		qword bit = value ? 1 : 0;
		write_accumulator_to_memory(bit, 1);
	}
	else
	{
		m_bitstream_data.current_stream_bit_position++;
		m_bitstream_data.accumulator_bit_count += 1;
		m_bitstream_data.accumulator = (m_bitstream_data.accumulator << 1) | (value ? 1 : 0);
	}
}

void c_bitstream::write_bool(const char* debug_string, bool value)
{
	ASSERT(writing());
	c_bitstream::write_bit_internal(value);
}

void c_bitstream::finish_writing(long* bits_wasted)
{
	//VASSERT(!overflowed(), c_string_builder("bitstream overflowed (%d bits > %d max-size), cannot be written successfully",
	//	m_bitstream_data.current_stream_bit_position, CHAR_BITS * m_data_size_bytes).get_string());
	DECLFUNC(0xA6030, void, __thiscall, c_bitstream*, long*)(this, bits_wasted);
}

void c_bitstream::reset(long state)
{
	DECLFUNC(0xA7BD0, void, __thiscall, c_bitstream*, long)(this, state);
}

bool c_bitstream::begin_consistency_check()
{
	reset(_bitstream_state_read_consistency_check);
	return true;
}

bool c_bitstream::read_bool(const char* debug_string)
{
	ASSERT(reading());

	return DECLFUNC(0xA82E0, bool, __thiscall, c_bitstream*)(this);
}

void c_bitstream::finish_reading()
{
	ASSERT(reading());

	//VASSERT(!overflowed(), c_string_builder("finish_reading: bitstream read off the end of the stream (%d bits > %d max-size)",
	//	m_bitstream_data.current_memory_bit_position, CHAR_BITS * m_data_size_bytes).get_string());

	m_state = _bitstream_state_read_finished;
}

bool c_bitstream::reading() const
{
	return IN_RANGE_INCLUSIVE(m_state, _bitstream_state_read, _bitstream_state_read_consistency_check);
}

bool c_bitstream::writing() const
{
	return m_state == _bitstream_state_write;
}
