#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/memory/data.hpp

enum e_data_array_flags
{
	_data_array_can_disconnect_bit,
	_data_array_disconnected_bit,
	_data_array_unknown2_bit,
	_data_array_invalidate_bit,
	_data_array_unknown4_bit,
	_data_array_unknown5_bit,
	_data_array_unknown6_bit,
	_data_array_unknown7_bit,

	k_number_of_data_array_flags
};

enum e_data_alignment
{
	_data_alignment_8bit,
	_data_alignment_16bit,
	_data_alignment_32bit,
	_data_alignment_64bit,

	k_number_of_data_alignments
};

// 32-bit data array index handle
typedef unsigned long datum_index;
static_assert(sizeof(datum_index) == 0x4);

enum : datum_index
{
	_datum_index_none = 0xFFFFFFFF
};

#define DATUM_INDEX_TO_ABSOLUTE_INDEX(VALUE) ((VALUE) & 0xFFFF)
#define DATUM_INDEX_TO_IDENTIFIER(VALUE)  ((VALUE) >> 16)
#define DATUM_IS_FREE(VALUE) (VALUE->identifier == 0)

struct s_datum_header
{
	word identifier;
};
static_assert(sizeof(s_datum_header) == 0x2);

struct s_data_array
{
	string name;
	long maximum_count;
	long size;
	byte alignment_bits; // e_data_alignment
	bool valid;

	// bit 0, _data_array_can_disconnect_bit
	// bit 1, _data_array_disconnected_bit
	word flags;

	tag signature;

	// c_allocation_interface
	void* allocator;

	long next_index;
	long first_unallocated;
	long actual_count;

	// e_datum_salt
	// salt_type == 0
	word next_identifier;

	// salt_type == 1
	word isolated_next_identifier;

	char* data;
	dword* in_use_bit_vector;
	long offset_to_data;
	long offset_to_bit_vector;

	long get_index(long index) const;
	long get_allocation_size() const;
	s_datum_header* get_datum(const datum_index index) const;
};
static_assert(sizeof(s_data_array) == 0x54);

struct s_data_iterator
{
	s_data_array const* data;
	datum_index index;
	long absolute_index;
};
static_assert(sizeof(s_data_iterator) == 0xC);

template<typename t_datum_type>
class c_data_iterator;

struct c_allocation_base;

void __cdecl data_iterator_begin(s_data_iterator* iterator, s_data_array const* data);
void* __cdecl data_iterator_next(s_data_iterator* data);
long __fastcall data_next_absolute_index(s_data_array const* data, long absolute_index);
void __fastcall datum_delete(s_data_array* data, datum_index index);
void datum_initialize(s_data_array* data, s_datum_header* header);
datum_index datum_new(s_data_array* data);
void* datum_get(s_data_array* data, datum_index index);
//extern long __cdecl data_allocation_size(long maximum_count, long size, long alignment_bits);
//extern void __cdecl data_connect(s_data_array* data, long count, void* datums);
//extern void __cdecl data_delete_all(s_data_array* data);
//extern void __cdecl data_disconnect(s_data_array* data);
//extern void __cdecl data_dispose(s_data_array* data);
//extern void __cdecl data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector);
//extern void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value);
//extern void __cdecl data_make_invalid(s_data_array* data);
//extern void __cdecl data_make_valid(s_data_array* data);
//extern s_data_array* __cdecl data_new(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern long __cdecl data_next_index(s_data_array const* data, long index);
//extern long __cdecl data_previous_index(s_data_array* data, long index);
//extern void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data);
long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index);
//extern bool __cdecl datum_available_at_index(s_data_array const* data, long index);
//extern long __cdecl datum_new_at_absolute_index(s_data_array* data, long index);
//extern long __cdecl datum_new_at_index(s_data_array* data, long index);
//extern long __cdecl datum_new_in_range(s_data_array* data, long begin_index, long end_index, bool initialize);
extern void* __cdecl datum_try_and_get(s_data_array const* data, long index);
void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long absolute_index);
//extern void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index);

template <typename t_datum_type>
struct c_smart_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

	//s_data_array*& get_restricted_data_array_address()
	//{
	//}

	t_datum_type& operator[](datum_index index) const
	{
		t_datum_type* data = (t_datum_type*)datum_get(m_data, index);
		return *data;
	}

	void operator=(s_data_array* rhs)
	{
		ASSERT(!rhs || sizeof(t_datum_type) == rhs->size);

		m_data = rhs;
	}

	s_data_array* operator*() const
	{
		return m_data;
	}

	s_data_array* operator->() const
	{
		return m_data;
	}

	//operator struct s_data_array*() const
	//{
	//}

	s_data_array* m_data;
};
static_assert(sizeof(c_smart_data_array<s_datum_header>) == sizeof(s_data_array*));

typedef c_smart_data_array<s_datum_header> data_array_base;

enum class data_address_type : long
{
	memory,
	definition,
	resource
};
static_assert(sizeof(data_address_type) == 0x4);

union data_address
{
	dword value;
	struct
	{
		data_address_type type : 3;
		long offset : 29;
	};
};
static_assert(sizeof(data_address) == 0x4);

template <typename t_datum_type>
struct c_typed_data_array
{
	string name;
	long maximum_count;
	long size;
	byte alignment_bits;
	bool valid;

	// bit 0, _data_array_can_disconnect_bit
	// bit 1, _data_array_disconnected_bit
	word flags;

	tag signature;

	// c_allocation_interface
	void* allocator;

	long next_index;
	long first_unallocated;
	long actual_count;

	// e_datum_salt
	// salt_type == 0
	word next_identifier;

	// salt_type == 1
	word isolated_next_identifier;

	t_datum_type* data;
	dword* in_use_bit_vector;
	long offset_to_data;
	long offset_to_bit_vector;
};
static_assert(sizeof(c_typed_data_array<void>) == sizeof(s_data_array));

template<typename t_datum_type>
class c_data_iterator
{
	//static_assert(std::is_same<t_datum_type, void>::value || std::is_base_of<s_datum_header, t_datum_type>::value);

public:
	c_data_iterator() :
		m_datum(),
		m_iterator()
	{
	}

	void begin(s_data_array* data)
	{
		data_iterator_begin(&m_iterator, data);
	}

	void begin(s_data_array const* data)
	{
		data_iterator_begin(&m_iterator, data);
	}

	bool next()
	{
		m_datum = (t_datum_type*)data_iterator_next(&m_iterator);
		return m_datum != nullptr;
	}

	long get_index() const
	{
		return m_iterator.index;
	}

	short get_absolute_index() const
	{
		return static_cast<short>(m_iterator.absolute_index);
	}

	t_datum_type* get_datum() const
	{
		return m_datum;
	}

	//protected:
	t_datum_type* m_datum;
	s_data_iterator m_iterator;
};