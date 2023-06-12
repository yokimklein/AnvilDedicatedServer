#include "data.h"
#include "assert.h"

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/memory/data.cpp

long s_data_array::get_index(long index) const
{
	if ((index < 0) || (index >= first_unallocated))
		return -1;

	while (!((1 << (index & 31)) & in_use_bit_vector[index >> 5]))
	{
		if (++index >= first_unallocated)
			return -1;
	}

	return index;
}

long s_data_array::get_allocation_size() const
{
	long padding = flags ? ((1 << flags) - 1) : 0;

	return padding + size * maximum_count + 4 * (((maximum_count + 31) >> 5) + 21);
}

s_datum_header* s_data_array::get_datum(const datum_index index) const
{
	if (index == -1 || DATUM_INDEX_TO_ABSOLUTE_INDEX(index) < (dword)first_unallocated)
		return nullptr;

	s_datum_header* datum = (s_datum_header*)&data[index * size];

	if (!datum->identifier || datum->identifier != DATUM_INDEX_TO_IDENTIFIER(index))
		return nullptr;

	return datum;
}

long data_next_absolute_index(s_data_array const* data, long index)
{
	FUNCTION_DEF(0xA8FE0, long, __fastcall, data_next_absolute_index_call, s_data_array const* data, long index);
	return data_next_absolute_index_call(data, index);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array* data)
{
	iterator->data = data;
	iterator->index = -1;
	iterator->absolute_index = -1;
}

void* data_iterator_next(s_data_iterator* iterator)
{
	char* result;
	const s_data_array* data = iterator->data;
	long index = data_next_absolute_index(iterator->data, iterator->absolute_index + 1);
	if (index == -1)
	{
		iterator->absolute_index = data->maximum_count;
		iterator->index = -1;
		result = nullptr;
	}
	else
	{
		result = &data->data[index * data->size];
		iterator->absolute_index = index;
		iterator->index = index | (*result << 16);
	}
	return result;
}

void __cdecl datum_delete(s_data_array* data, long index)
{
	FUNCTION_DEF(0xA8F20, void, __fastcall, datum_delete_call, s_data_array * data, long index);
	datum_delete_call(data, index);
}

void datum_initialize(s_data_array* data, s_datum_header* header)
{
	csmemset(header, 0, data->size);
	assert(data->next_identifier != 0);
	if (data->next_identifier == 0xFFFF)
		data->next_identifier = 0x8000;
	header->identifier = data->next_identifier++;
}

datum_index datum_new(s_data_array* data)
{
	long index_iterator = -1;
	long out_index = -1;
	assert(!TEST_BIT(data->flags, _data_array_disconnected_bit));
	assert(data->offset_to_data != NULL);
	assert(data->data != NULL);
	//data_verify(data); // TODO
	assert(data->valid);
	for (long i = data->next_index; i < data->first_unallocated; ++i)
	{
		if ((data->in_use_bit_vector[i >> 5] & (1 << (i & 0x1F))) == 0)
		{
			index_iterator = i;
			break;
		}
	}
	if (index_iterator == -1 && data->first_unallocated < data->maximum_count)
		index_iterator = data->first_unallocated;
	if (index_iterator != -1)
	{
		s_datum_header* header = (s_datum_header*)&data->data[index_iterator * data->size];
		data->in_use_bit_vector[index_iterator >> 5] |= 1 << (index_iterator & 0x1F);
		++data->actual_count;
		data->next_index = index_iterator + 1;
		if (data->first_unallocated <= index_iterator)
		{
			data->first_unallocated = index_iterator + 1;
			//sub_8292E418(data); // TODO
		}
		assert(DATUM_IS_FREE(header));
		datum_initialize(data, header);
		out_index = index_iterator | ((word)header->identifier << 16);
	}
	if (out_index != -1)
		datum_get(data, out_index);
	return out_index;
}

void* datum_get(s_data_array const* data, datum_index index)
{
	return (void*)&data->data[data->size * (word)index];
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long absolute_index)
{
	if (absolute_index >= 0 && absolute_index < data->first_unallocated)
	{
		if (*&data->data[absolute_index * data->size])
			return &data->data[absolute_index * data->size];
	}
	return nullptr;
}

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index)
{
	long index = -1;
	if (absolute_index != -1)
		index = absolute_index | (*&data->data[absolute_index * data->size] << 16);
	return index;
}