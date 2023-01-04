#include "data.h"
#include "..\dllmain.h"
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

/*
long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits)
{
	return INVOKE(0x0, data_allocation_size, maximum_count, size, alignment_bits); // 0x0055AAB0 in ms23
}

void __cdecl data_connect(s_data_array* data, long count, void* datums)
{
	INVOKE(0x0, data_connect, data, count, datums); // 0x0055AAE0 in ms23
}

void __cdecl data_delete_all(s_data_array* data)
{
	INVOKE(0x0, data_delete_all, data); // 0x0055ABF0 in ms23
}

void __cdecl data_disconnect(s_data_array* data)
{
	INVOKE(0x0, data_disconnect, data); // 0x0055ACA0 in ms23
}

void __cdecl data_dispose(s_data_array* data)
{
	INVOKE(0x0, data_dispose, data); // 0x0055ACC0 in ms23
}

void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	INVOKE(0x0, data_initialize, data, name, maximum_count, size, alignment_bits, allocation); // 0x0055ACF0 in ms23
}

void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector)
{
	INVOKE(0x0, data_initialize_disconnected, data, name, maximum_count, size, alignment_bits, allocation, in_use_bit_vector); // 0x0055ADA0 in ms23
}
*/
void data_iterator_begin(s_data_iterator* iterator, s_data_array* data)
{
	INVOKE(0x0, data_iterator_begin, iterator, data); // 0x0055AE10 in ms23
}

void* data_iterator_next(s_data_iterator* iterator)
{
	return INVOKE(0x0, data_iterator_next, iterator); // 0x0055AE30 in ms23
}
/*
void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value)
{
	return INVOKE(0x0, data_iterator_next_with_word_flags, iterator, flag_offset, flag_mask, flag_value); // 0x0055AE80 in ms23
}

// sub_55AEE0, called by unreferenced `c_content_catalogue` function

void __cdecl data_make_invalid(s_data_array* data)
{
	INVOKE(0x0, data_make_invalid, data); // 0x0055AF70 in ms23
}

void __cdecl data_make_valid(s_data_array* data)
{
	INVOKE(0x0, data_make_valid, data); // 0x0055AF80 in ms23
}

s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0, data_new, name, maximum_count, size, alignment_bits, allocation); // 0x0055AFA0 in ms23
}

s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	return INVOKE(0x0, data_new_disconnected, name, maximum_count, size, alignment_bits, allocation); // 0x0055B010 in ms23
}

long __cdecl data_next_absolute_index(s_data_array const* data, long index)
{
	return INVOKE(0x0, data_next_absolute_index, data, index); // 0x0055B060 in ms23
}

// sub_55B0B0, called by `data_iterator_next_with_word_flags`

long __cdecl data_next_index(s_data_array const* data, long index)
{
	return INVOKE(0x0, data_next_index, data, index); // 0x0055B130 in ms23
}

long __cdecl data_previous_index(s_data_array* data, long index)
{
	return INVOKE(0x0, data_previous_index, data, index); // 0x0055B170 in ms23
}

void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data)
{
	INVOKE(0x0, data_set_new_base_address, out_data, data); // 0x0055B1D0 in ms23
}

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long index)
{
	return INVOKE(0x0, datum_absolute_index_to_index, data, index); // 0x0055B280 in ms23
}

bool __cdecl datum_available_at_index(s_data_array const* data, long index)
{
	return INVOKE(0x0, datum_available_at_index, data, index); // 0x0055B2B0 in ms23
}
*/
void __cdecl datum_delete(s_data_array* data, long index)
{
	INVOKE(module_base + 0xA8F20, datum_delete, data, index); // 0x0055B2E0 in ms23
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

	//FUNCTION_DEF(0xA8C20, datum_index, __thiscall, datum_new_call, s_data_array* data);
	//return datum_new_call(data); // 0x0055B410 in ms23
}
/*
long __cdecl datum_new_at_absolute_index(s_data_array* data, long index)
{
	return INVOKE(0x0, datum_new_at_absolute_index, data, index); // 0x0055B4D0 in ms23
}

long __cdecl datum_new_at_index(s_data_array* data, long index)
{
	return INVOKE(0x0, datum_new_at_index, data, index); // 0x0055B550 in ms23
}

long __cdecl datum_new_in_range(s_data_array* data, long begin_index, long end_index, bool initialize)
{
	return INVOKE(0x0, datum_new_in_range, data, begin_index, end_index, initialize); // 0x0055B5D0 in ms23
}

void* __cdecl datum_try_and_get(s_data_array const* data, long index)
{
	return INVOKE(0x0, datum_try_and_get, data, index); // 0x0055B6D0 in ms23
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long index)
{
	return INVOKE(0x0, datum_try_and_get_absolute, data, index); // 0x0055B710 in ms23
}

void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index)
{
	return INVOKE(0x0, datum_try_and_get_unsafe, data, index); // 0x0055B740 in ms23
}
*/
void* datum_get(s_data_array const* data, datum_index index)
{
	return (void*)&data->data[data->size * (word)index];
}