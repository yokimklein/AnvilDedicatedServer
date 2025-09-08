#pragma once
#include <cseries\cseries.h>

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/memory/data.hpp

enum e_data_array_flags
{
	_data_array_can_disconnect_bit,
	_data_array_disconnected_bit,
	_data_array_protection_bit,
	_data_array_invalidate_bit, // _data_array_verify_data_pattern_bit
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

#define DATUM_IS_FREE(VALUE) (VALUE->identifier == 0)
#define DATUM_INDEX_TO_ABSOLUTE_INDEX(VALUE) ((VALUE) & 0x3FF)
#define DATUM_INDEX_TO_IDENTIFIER(VALUE) ((VALUE) >> 16)
#define BUILD_DATUM_INDEX(IDENTIFIER, ABSOLUTE_INDEX) ((IDENTIFIER << 16) | ABSOLUTE_INDEX)
#define DATUM_GET(DATA, TYPE, INDEX) ((TYPE*)datum_get(DATA, INDEX))
#define DATUM_TRY_AND_GET(DATA, TYPE, INDEX) ((TYPE*)datum_try_and_get(DATA, INDEX))
#define DATUM_GET_ABSOLUTE(DATA, TYPE, INDEX) ((TYPE*)datum_get_absolute(DATA, INDEX))
#define DATUM_TRY_AND_GET_ABSOLUTE(DATA, TYPE, INDEX) ((TYPE*)datum_try_and_get_absolute(DATA, INDEX))

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
	word flags;
	tag signature;
	c_allocation_base* allocation;
	long first_possibly_free_absolute_index;
	long count;
	long actual_count;
	word next_identifier;
	word isolated_next_identifier;
	char* data;
	dword* in_use_bit_vector;
	dword offset_to_data;
	dword offset_to_bit_vector;
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

extern void __cdecl data_iterator_begin(s_data_iterator* iterator, s_data_array const* data);
extern void* __cdecl data_iterator_next(s_data_iterator* data);
extern long __fastcall data_next_absolute_index(s_data_array const* data, long absolute_index);
extern void __fastcall datum_delete(s_data_array* data, datum_index index);
extern void datum_initialize(s_data_array* data, s_datum_header* header);
extern datum_index datum_new(s_data_array* data);
extern void* datum_get(s_data_array* data, datum_index index);
extern long data_allocation_size(long maximum_count, long size, long alignment_bits);
//extern void __cdecl data_connect(s_data_array* data, long count, void* datums);
extern void __fastcall data_delete_all(s_data_array* data);
//extern void __cdecl data_disconnect(s_data_array* data);
extern void __cdecl data_dispose(s_data_array* data);
extern void __fastcall data_initialize(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern void __cdecl data_initialize_disconnected(s_data_array* data, char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation, dword* in_use_bit_vector);
//extern void* __cdecl data_iterator_next_with_word_flags(s_data_iterator* iterator, long flag_offset, word flag_mask, word flag_value);
extern void data_make_invalid(s_data_array* data);
extern void data_make_valid(s_data_array* data);
extern s_data_array* __cdecl data_new(const char* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern s_data_array* __cdecl data_new_disconnected(char const* name, long maximum_count, long size, long alignment_bits, c_allocation_base* allocation);
//extern long __cdecl data_next_index(s_data_array const* data, long index);
//extern long __cdecl data_previous_index(s_data_array* data, long index);
//extern void __cdecl data_set_new_base_address(s_data_array** out_data, s_data_array* data);
extern long __cdecl datum_absolute_index_to_index(s_data_array const* data, long absolute_index);
//extern bool __cdecl datum_available_at_index(s_data_array const* data, long index);
//extern long __cdecl datum_new_at_absolute_index(s_data_array* data, long index);
//extern long __cdecl datum_new_at_index(s_data_array* data, long index);
//extern long __cdecl datum_new_in_range(s_data_array* data, long begin_index, long end_index, bool initialize);
extern void* __cdecl datum_try_and_get(s_data_array const* data, long index);
extern void* __cdecl datum_try_and_get_absolute(s_data_array const* data, long absolute_index);
//extern void* __cdecl datum_try_and_get_unsafe(s_data_array const* data, long index);
extern bool data_is_full(const s_data_array* data);

template <typename t_datum_type>
class c_smart_data_array
{
	static_assert(__is_base_of(s_datum_header, t_datum_type));

public:
	s_data_array*& get_restricted_data_array_address()
	{
		return this;
	}

	operator s_data_array* () const
	{
		return m_data_array;
	}

	s_data_array* operator->() const
	{
		return m_data_array;
	}

	void operator=(s_data_array* rhs)
	{
		ASSERT(!rhs || sizeof(t_datum_type) == rhs->size);

		m_data_array = rhs;
	}

	t_datum_type* operator&()
	{
		return m_type_access->data;
	}

	struct s_typed_access
	{
		byte unused[OFFSETOF(s_data_array, data)];
		t_datum_type* data;
	};

	union
	{
		s_data_array* m_data_array;
		s_typed_access* m_type_access;
	};
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