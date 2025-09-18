#include "data.h"

long __fastcall data_next_absolute_index(s_data_array const* data, long absolute_index)
{
	return INVOKE(0xA8FE0, data_next_absolute_index, data, absolute_index);
}

void data_iterator_begin(s_data_iterator* iterator, s_data_array const* data)
{
	iterator->data = data;
	iterator->index = NONE;
	iterator->absolute_index = NONE;
}

void* data_iterator_next(s_data_iterator* data)
{
	void* result;
	const s_data_array* data_array = data->data;
	long index = data_next_absolute_index(data->data, data->absolute_index + 1);
	if (index == NONE)
	{
		data->absolute_index = data_array->maximum_count;
		data->index = NONE;
		result = nullptr;
	}
	else
	{
		result = &data_array->data[index * data_array->size];
		data->absolute_index = index;
		data->index = BUILD_DATUM_INDEX(*(short*)result, index);
	}
	return result;
}

void __fastcall datum_delete(s_data_array* data, datum_index index)
{
	INVOKE(0xA8F20, datum_delete, data, index);
}

void datum_initialize(s_data_array* data, s_datum_header* header)
{
	csmemset(header, 0, data->size);
	ASSERT(data->next_identifier != 0);
	if (data->next_identifier == NONE)
	{
		data->next_identifier = 0x8000;
	}
	header->identifier = data->next_identifier++;
}

datum_index datum_new(s_data_array* data)
{
	long index_iterator = NONE;
	long out_index = NONE;
	ASSERT(!TEST_BIT(data->flags, _data_array_disconnected_bit));
	ASSERT(data->offset_to_data != NULL);
	ASSERT(data->data != NULL);
	//data_verify(data); // $TODO
	ASSERT(data->valid);
	for (long i = data->first_possibly_free_absolute_index; i < data->count; ++i)
	{
		if (!BIT_VECTOR_TEST_FLAG(data->in_use_bit_vector, i))
		{
			index_iterator = i;
			break;
		}
	}
	if (index_iterator == NONE && data->count < data->maximum_count)
	{
		index_iterator = data->count;
	}
	if (index_iterator != NONE)
	{
		s_datum_header* header = (s_datum_header*)&data->data[index_iterator * data->size];
		BIT_VECTOR_OR_FLAG(data->in_use_bit_vector, index_iterator);
		++data->actual_count;
		data->first_possibly_free_absolute_index = index_iterator + 1;
		if (data->count <= index_iterator)
		{
			data->count = index_iterator + 1;
			//sub_8292E418(data); // TODO
		}
		ASSERT(DATUM_IS_FREE(header));
		datum_initialize(data, header);
		out_index = BUILD_DATUM_INDEX(header->identifier, index_iterator);
	}
	if (out_index != NONE)
	{
		datum_get(data, out_index);
	}
	return out_index;
}

void* datum_get(s_data_array* data, datum_index index)
{
	return (void*)&data->data[data->size * (word)index];
}

void* __cdecl datum_try_and_get(s_data_array const* data, long index)
{
	if (!data)
	{
		return NULL;
	}

	void* result = NULL;

	ASSERT(data);
	ASSERT(data->valid);

	word identifier = DATUM_INDEX_TO_IDENTIFIER(index);
	word absolute_index = DATUM_INDEX_TO_ABSOLUTE_INDEX(index);

	if (index != NONE || absolute_index != NONE)
	{
		VASSERT(identifier, c_string_builder("tried to access %s using datum_try_and_get() with an absolute index #%d",
			data->name,
			absolute_index).get_string());
		
		VASSERT(VALID_INDEX(absolute_index, data->maximum_count), c_string_builder("tried to access %s using datum_try_and_get() with an index 0x%08X outside maximum range [0, %d)",
			data->name,
			index,
			data->maximum_count).get_string());

		if (absolute_index < data->count)
		{
			void** data_ptr = (void**)offset_pointer(data, OFFSETOF(s_data_array, data));
			s_datum_header* header = (s_datum_header*)offset_pointer(*data_ptr, absolute_index * data->size);

			if (header->identifier)
			{
				if (header->identifier == identifier)
				{
					result = header;
				}
			}
		}
	}

	ASSERT(result == align_pointer(result, data->alignment_bits));
	return result;
}

void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long absolute_index)
{
	if (absolute_index >= 0 && absolute_index < data->count)
	{
		if (*&data->data[absolute_index * data->size])
		{
			return &data->data[absolute_index * data->size];
		}
	}
	return nullptr;
}

bool data_is_full(const s_data_array* data)
{
	ASSERT(data);
	ASSERT(data->valid);

	return data->maximum_count == data->count;
}

long data_allocation_size(long maximum_count, long size, long alignment_bits)
{
	long alignment = 0;
	if (alignment_bits)
	{
		alignment = MASK(alignment_bits);
	}
	
	return maximum_count * size + sizeof(s_data_array) + alignment + BIT_VECTOR_SIZE_IN_BYTES(maximum_count);
}

void __fastcall data_delete_all(s_data_array* data)
{
	INVOKE(0xA8B80, data_delete_all, data);
}

void __cdecl data_dispose(s_data_array* data)
{
	c_allocation_base* allocation = data->allocation;
	ASSERT(allocation != NULL);
	
	csmemset(data, 0, sizeof(s_data_array));
	
	if (allocation)
	{
		allocation->deallocate(data);
	}
}

#pragma runtime_checks("", off)
void __fastcall data_initialize(s_data_array* data, const char* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	INVOKE(0xA8920, data_initialize, data, name, maximum_count, size, alignment_bits, allocation);
	__asm add esp, 0x10; // Fix usercall & cleanup stack
}
#pragma runtime_checks("", restore)

void data_make_invalid(s_data_array* data)
{
	//data_verify(data); // $TODO
	data->valid = false;
}

void data_make_valid(s_data_array* data)
{
	data->valid = true;
	data_delete_all(data);
	//data_verify(data); // $TODO
}

s_data_array* __cdecl data_new(const char* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation)
{
	s_data_array* data = static_cast<s_data_array*>(allocation->allocate(data_allocation_size(maximum_count, size, alignment_bits), name));
	if (data)
	{
		data_initialize(data, name, maximum_count, size, alignment_bits, allocation);
		data->flags |= FLAG(_data_array_protection_bit);
	}
	return data;
}

long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index)
{
	long index = NONE;
	if (absolute_index != NONE)
	{
		s_datum_header* header = (s_datum_header*)&data->data[absolute_index * data->size];
		index = BUILD_DATUM_INDEX(header->identifier, absolute_index);
	}
	return index;
}