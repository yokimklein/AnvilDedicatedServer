#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/cseries.hpp

#include "cseries\cseries_windows.h"
#include "cseries\cseries_console.h"
#include "cseries\cseries_windows_debug_pc.h"

#include <math\integer_math.h>
#include <math\real_math.h>

#include <stdarg.h>
#include <stdio.h>

#include <memory\member_to_static.h>

extern inline size_t base_address_impl(size_t address);

template<typename k_return_type = size_t>
k_return_type base_address(size_t address = 0)
{
	return (k_return_type)(base_address_impl(address));
}

//#define BASE_ADDRESS(ADDR) (module_base + ADDR)
// module_base isn't instatiated in time for reference macros to use it, so we're calling GetModuleHandle directly for now
#define BASE_ADDRESS_REFERENCE(ADDR)((size_t)GetModuleHandle(NULL) + ADDR)

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define DECLFUNC(ADDR, R, CC, ...) base_address<R(CC*)(__VA_ARGS__)>(ADDR)
#define INVOKE(ADDR, TYPE, ...) base_address<decltype(TYPE)*>(ADDR)(__VA_ARGS__)
#define INVOKE_CLASS_MEMBER(ADDRESS, CLASS, NAME, ...) (this->*static_to_member_t<decltype(&CLASS##::##NAME)>{ .address = base_address(ADDRESS) }.function)(__VA_ARGS__)

#define OFFSETOF(s,m) __builtin_offsetof(s,m)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
#define IN_RANGE(value, begin, end) ((value) > (begin) && (value) < (end))
#define IN_RANGE_INCLUSIVE(value, begin, end) (((value) >= (begin)) && ((value) <= (end)))
#define VALID_INDEX(index, count) ((index) >= 0 && (index) < count)
#define VALID_COUNT(index, count) ((index) >= 0 && (index) <= (count))

// referenced
#define BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT) (((BIT_COUNT) + (LONG_BITS - 1)) >> 5)
#define BIT_VECTOR_SIZE_IN_BYTES(BIT_COUNT) (4 * BIT_VECTOR_SIZE_IN_LONGS(BIT_COUNT))
#define BIT_VECTOR_TEST_FLAG(BIT_VECTOR, BIT) ((BIT_VECTOR[BIT >> 5] & (1 << (BIT & (LONG_BITS - 1)))) != 0)

// not referenced
#define BIT_VECTOR_OR_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] |= (1 << (BIT & (LONG_BITS - 1))))
#define BIT_VECTOR_AND_FLAG(BIT_VECTOR, BIT) (BIT_VECTOR[BIT >> 5] &= ~(1 << (BIT & (LONG_BITS - 1))))

#define REFERENCE_DECLARE(address, type, name) type& name = *base_address<type*>(address)
#define REFERENCE_DECLARE_ARRAY(address, type, name, count) type(&name)[count] = *base_address<type(*)[count]>(address)
#define REFERENCE_DECLARE_2D_ARRAY(address, type, name, count0, count1) type(&name)[count0][count1] = *base_address<type(*)[count0][count1]>(address)
#define REFERENCE_DECLARE_3D_ARRAY(address, type, name, count0, count1, count3) type(&name)[count0][count1][count3] = *base_address<type(*)[count0][count1][count3]>(address)
#define REFERENCE_DECLARE_STATIC_ARRAY(address, type, count, name) c_static_array<type, count> &name = *base_address<c_static_array<type, count>*>(address)

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(x, low) ((x) < (low) ? (x) : (low))
#define MAX(x, high) ((x) > (high) ? (x) : (high))
#define PIN(x, low, high) MIN(MAX((x), (low)), (high))
#define CLAMP(x, low, high) ((x) <= (low) ? (low) : (x) >= (high) ? (high) : (x))
#define CLAMP_LOWER(x, low, high) ((x) >= (high) - (low) ? (x) - (high) : (low))
#define CLAMP_UPPER(x, low, high) ((x) <= (high) - (low) ? (x) + (low) : (high))

#define STRINGIFY_DETAIL(x) #x
#define STRINGIFY(x) STRINGIFY_DETAIL(x)

#define try_bool(X) if (!X) return false

#define k_tag_string_length 32
#define k_tag_long_string_length 256

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long CHAR_BYTES = sizeof(char);
const long SHORT_BYTES = sizeof(short);
const long LONG_BYTES = sizeof(long);
const long QWORD_BYTES = sizeof(qword);
const long CHAR_BITS = SIZEOF_BITS(char);
const long SHORT_BITS = SIZEOF_BITS(short);
const long LONG_BITS = SIZEOF_BITS(long);
const long QWORD_BITS = SIZEOF_BITS(qword);

#define FLAG(bit) (1ULL << (unsigned)(bit))
#define RANGE(bit) (FLAG((bit) - 1))
#define MASK(bit) ((RANGE((bit))) | ((bit) <= 1 ? 0 : ((RANGE((bit)) - 1))))
#define TEST_BIT(flags, bit) (((flags) & FLAG((bit))) != 0)
#define TEST_RANGE(flags, start_bit, end_bit) (((flags) & (((1 << ((end_bit) - (start_bit) + 1)) - 1) << (start_bit))) != 0)
#define TEST_FLAG(flags, bit) (flags.test((bit)))
#define TEST_MASK(flags, mask) (((flags) & mask) != 0)
#define ALIGN(value, bit) (((value) & ~(FLAG((bit)) - 1)) + FLAG((bit)))
#define ALIGN_UP(value, bit) ((((value) & (FLAG((bit)) - 1)) == 0) ? (value) : ((value) | ((1 << (bit)) - 1)) + 1)
#define SET_BIT(flags, bit, enable) { if ((enable)) { (flags) |= FLAG((bit)); } else { (flags) &= ~FLAG((bit)); } }
#define SET_MASK(flags, mask, enable) { if ((enable)) { (flags) |= (mask); } else { (flags) &= ~(mask); } }
#define VALID_BITS(flags, max_bits) ((flags) & ~((1 << (max_bits)) - 1))

#define ENTITY_INDEX_TO_SEED(value) ((ulong)(value >> 28))
#define SEED_TO_ENTITY_INDEX(value) ((ulong)(value << 28))

#define NONE -1

extern void* offset_pointer(void* pointer, long offset);
extern void const* offset_pointer(void const* pointer, long offset);
extern unsigned int address_from_pointer(void const* pointer);
extern void* pointer_from_address(unsigned int address);
extern unsigned int align_address(unsigned int address, long alignment_bits);
extern void* align_pointer(void* pointer, long alignment_bits);
extern long pointer_distance(void const* pointer_a, void const* pointer_b);
extern long pointer_difference(void const* pointer_a, void const* pointer_b);

constexpr bool pointer_is_aligned(void* pointer, long alignment_bits)
{
	return ((unsigned long)pointer & ((1 << alignment_bits) - 1)) == 0;
}

extern bool g_catch_exceptions;

#if defined(ASSERTS_ENABLED)

#define ASSERT(STATEMENT, ...) do { if (!(STATEMENT)) ASSERT_EXCEPTION(STATEMENT, true, __VA_ARGS__); } while (false)
#define ASSERT_EXCEPTION(STATEMENT, IS_EXCEPTION, ...) \
do { \
	if (!(STATEMENT) && !handle_assert_as_exception(#STATEMENT, __FILE__, __LINE__, IS_EXCEPTION)) \
	{ \
	    display_assert(#STATEMENT, __FILE__, __LINE__, IS_EXCEPTION); \
	    if (is_debugger_present() || !g_catch_exceptions) \
	        __debugbreak(); \
	    else \
	        system_abort(); \
	} \
} while (false)

#define VASSERT(STATEMENT, MESSAGE, ...) VASSERT_EXCEPTION(STATEMENT, MESSAGE, true)
#define VASSERT_EXCEPTION(STATEMENT, MESSAGE, IS_EXCEPTION, ...) \
do { \
	if (!(STATEMENT) && !handle_assert_as_exception(MESSAGE, __FILE__, __LINE__, IS_EXCEPTION)) \
	{ \
	    display_assert(MESSAGE, __FILE__, __LINE__, IS_EXCEPTION); \
	    if (is_debugger_present() || !g_catch_exceptions) \
	        __debugbreak(); \
	    else \
	        system_abort(); \
	} \
} while (false)

#define UNREACHABLE(...) VASSERT(0, "unreachable")
#define HALT(...) VASSERT(0, "halt()")

#else

#define ASSERT(STATEMENT, ...) do { } while (false)
#define ASSERT_EXCEPTION(STATEMENT, ...) do { } while (false)

#define VASSERT(STATEMENT, ...) do { } while (false)
#define VASSERT_EXCEPTION(STATEMENT, ...) do { } while (false)

#define UNREACHABLE(...) do { } while (false)
#define HALT(...) do { } while (false)

#endif

extern void display_assert(const char* statement, const char* file, long line, bool fatal);
extern bool handle_assert_as_exception(const char* statement, const char* file, long line, bool fatal);

extern int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size);
extern void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size);
extern void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size);

struct csstrtok_data
{
	char* unknown1;
	char* unknown2;
	char* unknown3;
};

extern long csstricmp(char const* s1, char const* s2);
extern long csstrcmp(char const* s1, char const* s2);
extern long csstrncmp(char const* s1, char const* s2, dword size);
extern long csstrnicmp(char const* s1, char const* s2, dword size);
extern char* csstristr(char const* s1, char const* s2);
extern char* csstrnzcpy(char* s1, char const* s2, dword size);
extern char* csstrnzcat(char* s1, char const* s2, dword size);
extern dword csstrnlen(char const* s, dword size);
extern char* csstrnupr(char* s, dword size);
extern char* csstrnlwr(char* s, dword size);
extern char const* csstrstr(char const* s1, char const* s2);
extern char* csstrtok(char* a1, char const* delimiters, bool a3, csstrtok_data* data);
extern long cvsnzprintf(char* buffer, dword size, char const* format, va_list list);
extern char* csnzprintf(char* buffer, dword size, char const* format, ...);
extern char* csnzappendf(char* buffer, dword size, char const* format, ...);
extern char* strncpy_debug(char* s1, char const* s2, dword size);
extern long strlen_debug(char const* s);
extern bool string_is_not_empty(char const* s);
extern void string_terminate_at_first_delimiter(char* s, const char* delimiter);

extern bool ascii_isupper(char C);
extern void ascii_strnlwr(char* string, long count);

template<typename t_type, long k_count>
void zero_array(t_type(&data)[k_count])
{
	csmemset(data, 0, sizeof(t_type) * k_count);
}

long __fastcall bit_vector_count_bits(const dword* bit_mask, int bit_count);

template<typename t_type>
class c_wrapped_array
{
public:
	c_wrapped_array()
	{
		set_elements(nullptr, 0);
	}

	template<long k_element_count>
	c_wrapped_array(t_type(&elements)[k_element_count])
	{
		set_elements(elements, k_element_count);
	}

	void set_elements(t_type* elements, long element_count)
	{
		m_count = element_count;
		m_elements = elements;
	}

	long count() const
	{
		return m_count;
	}

	t_type* begin()
	{
		return m_elements;
	}

	t_type* end()
	{
		return m_elements + m_count;
	}

	bool valid_index(long index)
	{
		return VALID_INDEX(index, count());
	}

	t_type& operator[](long index)
	{
		ASSERT(valid_index(index));

		return m_elements[index];
	}

	//protected:
	long m_count;
	t_type* m_elements;
};

template<typename t_type>
class c_basic_buffer
{
public:
	//c_basic_buffer() :
	//	m_buffer(nullptr),
	//	m_size(0)
	//{
	//}
	//
	//c_basic_buffer(void* start, ulong size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, ulong size) :
	//	m_buffer(start),
	//	m_size(size)
	//{
	//}
	//
	//c_basic_buffer(void* start, const void* end) :
	//	m_buffer(start),
	//	m_size(pointer_distance(start, end))
	//{
	//	ASSERT(start <= end);
	//}

	void clear()
	{
		m_buffer = nullptr;
		m_size = 0;
	}

	void set_buffer(t_type* start, ulong size)
	{
		ASSERT(start || size == 0);

		m_buffer = start;
		m_size = size;
	}

	void set_buffer(t_type* start, t_type* end)
	{
		ASSERT(start <= end);

		m_buffer = start;
		m_size = pointer_distance(start, end);
	}

	ulong size()
	{
		return m_size;
	}

	t_type* begin() const
	{
		return m_buffer;
	}

	t_type* end() const
	{
		return (t_type*)offset_pointer(m_buffer, m_size);
	}

	//protected:
	t_type* m_buffer;
	ulong m_size;
};

template<typename t_type, long k_count>
struct c_static_array
{
public:
	c_static_array()
	{
		clear();
	}

	c_static_array<t_type, k_count>& reverse()
	{
		_reverse(begin(), end() - 1);

		return *this;
	}

	t_type* begin()
	{
		return m_storage;
	}

	t_type* end()
	{
		return &m_storage[k_count];
	}

	t_type const* begin() const
	{
		return m_storage;
	}

	t_type const* end() const
	{
		return &m_storage[k_count];
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
	}

	long get_count() const
	{
		return k_count;
	}

	t_type* get_elements()
	{
		return m_storage;
	}

	bool valid(long index) const
	{
		return VALID_INDEX(index, k_count);
	}

	void set_all(t_type const& value)
	{
		for (long i = 0; i < k_count; i++)
		{
			m_storage[i] = value;
		}
	}

	long get_total_element_size() const
	{
		return sizeof(t_type) * k_count;
	}

	t_type const& operator[](long index) const
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

	t_type& operator[](long index)
	{
		ASSERT(valid(index));

		return m_storage[index];
	}

protected:
	t_type m_storage[k_count];
};

template<typename t_type, long k_count>
class c_static_sized_dynamic_array
{
public:
	c_static_sized_dynamic_array() :
		m_storage(),
		m_allocated_count(0)
	{
	}

	c_static_sized_dynamic_array<t_type, k_count>& reverse()
	{
		_reverse(begin(), end() - 1);

		return *this;
	}

	t_type* begin()
	{
		return m_storage.begin();
	}

	t_type* end()
	{
		return begin() + m_allocated_count;
	}

	const t_type* begin() const
	{
		return m_storage.begin();
	}

	const t_type* end() const
	{
		return begin() + m_allocated_count;
	}

	void clear()
	{
		csmemset(m_storage, 0, sizeof(m_storage));
		m_allocated_count = 0;
	}

	long count() const
	{
		return m_allocated_count;
	}

	bool full() const
	{
		return m_allocated_count == m_storage.get_count();
	}

	bool valid_index(long index) const
	{
		return VALID_INDEX(index, m_allocated_count);
	}

	long new_element_index()
	{
		long new_index = m_allocated_count;
		ASSERT(m_storage.valid(new_index));

		m_allocated_count++;

		return new_index;
	}

	void resize(long new_size)
	{
		ASSERT(new_size >= 0 && new_size <= m_storage.get_count());
		m_allocated_count = new_size;
	}

	t_type& operator[](long index)
	{
		ASSERT(valid_index(index));

		return m_storage[index];
	}

protected:
	c_static_array<t_type, k_count> m_storage;
	long m_allocated_count;
};

template<typename t_type, long k_count>
class c_static_stack
{
public:
	c_static_stack() :
		m_count(0),
		m_elements()
	{
	}

	t_type* get(long index)
	{
		ASSERT(valid(index));

		return &m_elements[index];
	}

	t_type* get_top()
	{
		return get(top());
	}

	t_type* get_elements()
	{
		return m_elements;
	}

	long push()
	{
		ASSERT(!full());

		return m_count++;
	}

	void pop()
	{
		ASSERT(!empty());

		m_count--;
	}

	void push_back(const t_type& cache)
	{
		*get(push()) = cache;
	}

	void clear()
	{
		m_count = 0;
	}

	long count() const
	{
		return m_count;
	}

	void resize(long count)
	{
		m_count = count;

		ASSERT(valid());
	}

	bool empty() const
	{
		ASSERT(valid());

		return m_count == 0;
	}

	bool full() const
	{
		ASSERT(valid());

		return m_count == k_count;
	}

	long top() const
	{
		ASSERT(!empty());

		return m_count - 1;
	}

	bool valid(long index) const
	{
		ASSERT(valid());

		return VALID_INDEX(index, m_count);
	}

	bool valid() const
	{
		return VALID_COUNT(m_count, k_count);
	}

	t_type& operator[](long index)
	{
		ASSERT(valid(index));

		return m_elements[index];
	}

protected:
	long m_count;
	t_type m_elements[k_count];
};

template<typename t_type, long k_type_size = sizeof(t_type), long k_alignment_mask = __alignof(t_type) - 1>
struct c_typed_opaque_data
{
	t_type* get()
	{
		ASSERT(((dword)m_opaque_storage & k_alignment_mask) == 0);
		return reinterpret_cast<t_type*>(((dword)m_opaque_storage + k_alignment_mask) & ~k_alignment_mask);
	}

	byte m_opaque_storage[k_type_size];
	//t_type* m_live_object;
};

template<typename t_type, long k_maximum_mapped, long k_a_hash_scalar, long k_b_hash_scalar, long k_c_hash_scalar>
class c_simple_hash_table
{
public:
	struct s_data
	{
		t_type entry;
		long key_a;
		long key_b;
		long key_c;
	};
	static_assert(sizeof(s_data) == sizeof(t_type) + (sizeof(long) * 3));

	long m_total_count;
	c_static_array<s_data, k_maximum_mapped> data;
	c_static_array<short, k_maximum_mapped> m_hash_entry_count;
	c_static_array<short, k_maximum_mapped> m_original_hash_entry;
};
static_assert(sizeof(c_simple_hash_table<long, 8192, 3307, 3, 337>) == 0x28004);

template<typename t_type>
class c_reference_count
{
public:
	t_type m_reference_count;
};
static_assert(sizeof(c_reference_count<long>) == 0x4);

class c_wrapped_flags
{
public:
	ulong* m_bit_storage;
	long m_bit_count;
};
static_assert(sizeof(c_wrapped_flags) == 0x8);

template<typename t_type, typename t_storage_type, t_storage_type k_count>
struct c_flags_no_init
{
public:
	static t_type const k_maximum_count = (t_type)k_count;
	//static_assert(k_maximum_count <= SIZEOF_BITS(t_storage_type));

	void clear()
	{
		m_flags = 0;
	}

	//void set_all()

	bool test(t_type bit) const
	{
		ASSERT(valid_bit(bit));

		return TEST_BIT(m_flags, bit);
	}

	bool test_range(t_type start_bit, t_type end_bit) const
	{
		ASSERT(valid_bit(start_bit) && valid_bit(end_bit) && (start_bit <= end_bit));
		return TEST_RANGE(m_flags, start_bit, end_bit);
	}

	void set(t_type bit, bool enable)
	{
		ASSERT(valid_bit(bit));

		if (bit < k_maximum_count)
		{
			SET_BIT(m_flags, bit, enable);
		}
	}

	void toggle(t_type bit)
	{
		ASSERT(valid_bit(bit));
		m_flags ^= FLAG(bit);
	}

	bool valid() const
	{
		static_assert((ushort)~MASK(11) == 0xF800);
		return !TEST_MASK(m_flags, ~MASK(k_count));
	}

	bool is_empty() const
	{
		static_assert(MASK(11) == 0x7FF);
		return !TEST_MASK(m_flags, MASK(k_count));
	}

	t_type count() const
	{
		return k_maximum_count;
	}

	bool operator==(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
	{
		return m_flags == rsa.m_flags;
	}

	bool operator!=(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
	{
		return m_flags != rsa.m_flags;
	}

	bool operator==(t_storage_type const& rsa) const
	{
		return m_flags == rsa;
	}

	bool operator!=(t_storage_type const& rsa) const
	{
		return m_flags != rsa;
	}

	t_storage_type get_unsafe() const
	{
		return m_flags;
	}

	void set_unsafe(t_storage_type new_flags)
	{
		m_flags = new_flags;
	}

	c_flags_no_init<t_type, t_storage_type, k_count>& operator|=(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa)
	{
		m_flags |= rsa.m_flags;
		ASSERT(valid());
		return *this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count>& operator&=(const c_flags_no_init<t_type, t_storage_type, k_count>& rsa)
	{
		m_flags &= rsa.m_flags;
		ASSERT(valid());
		return *this;
	}

	c_flags_no_init<t_type, t_storage_type, k_count> operator~() const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(~m_flags & MASK(k_count));
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator&(t_storage_type const rsa) const

	c_flags_no_init<t_type, t_storage_type, k_count> operator&(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa) const
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(m_flags & rsa.m_flags);
		return flags;
	}

	//c_flags_no_init<t_type, t_storage_type, k_count> operator|(t_storage_type const rsa) const

	c_flags_no_init<t_type, t_storage_type, k_count> operator|(c_flags_no_init<t_type, t_storage_type, k_count> const& rsa)
	{
		c_flags_no_init<t_type, t_storage_type, k_count> flags;
		flags.set_unsafe(m_flags | rsa.m_flags);
		return flags;
	}

	static bool valid_bit(t_type bit)
	{
		return VALID_INDEX(bit, k_maximum_count);
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_flags);
	}

protected:
	t_storage_type m_flags;
};

template<typename t_type, typename t_storage_type, t_storage_type k_count>
struct c_flags :
	public c_flags_no_init<t_type, t_storage_type, k_count>
{
public:
	c_flags(t_storage_type flags)
	{
		this->m_flags = flags;
	}

	c_flags()
	{
		this->m_flags = 0;
	}
};

template<long k_maximum_count>
struct c_static_flags_no_init
{
	static long const MAX_COUNT = k_maximum_count;

	void and_(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		ASSERT(vector_a);

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] &= vector_a[i];
	}

	void and_not_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		dword const* vector_a_bits = vector_a->get_bits_direct();
		dword const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & ~vector_b_bits[i];
	}

	void and_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		dword const* vector_a_bits = vector_a->get_bits_direct();
		dword const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = vector_a_bits[i] & vector_b_bits[i];
	}

	void clear()
	{
		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void clear_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void copy(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		csmemcpy(m_flags, vector_a, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	long count_bits_set() const
	{
		return bit_vector_count_bits(m_flags, k_maximum_count);
	}

	void fill(long count, byte fill_value)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, fill_value, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	dword const* get_bits_direct() const
	{
		return m_flags;
	}

	dword* get_writeable_bits_direct() const
	{
		return m_flags;
	}

	long highest_bit_set_in_range(long count) const
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		return bit_vector_highest_bit_set(m_flags, count);
	}

	void invert_bits()
	{
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] = ~m_flags[i];

		// no clue
		//m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1] = (byte)m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1];
	}

	bool is_clear() const
	{
		byte result = 1;
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			result &= m_flags[i] == 0;

		return result;
	}

	void keep_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = BIT_VECTOR_SIZE_IN_LONGS(count); i < BIT_VECTOR_SIZE_IN_LONGS(count); ++i)
			m_flags[i] = 0;

		m_flags[BIT_VECTOR_SIZE_IN_LONGS(count) - 1] &= ((count & (LONG_BITS - 1)) != 0) ? 0xFFFFFFFF >> (LONG_BITS - (count & (LONG_BITS - 1))) : 0xFFFFFFFF;
	}

	void or_bits(dword const* bits, long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_flags[i] |= bits[i];
	}

	void set(long index, bool enable)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		if (enable)
			BIT_VECTOR_OR_FLAG(m_flags, index);
		else
			BIT_VECTOR_AND_FLAG(m_flags, index);
	}

	void set_all()
	{
		csmemset(m_flags, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void set_bits_direct_destructive(long count, dword const* bits)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemcpy(m_flags, bits, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void set_range(long count)
	{
		ASSERT(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_flags, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	bool test(long index) const
	{
		ASSERT(VALID_INDEX(index, k_maximum_count));

		return BIT_VECTOR_TEST_FLAG(m_flags, index);
	}

	dword m_flags[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)];
};

template<long k_maximum_count>
struct c_static_flags :
	public c_static_flags_no_init<k_maximum_count>
{
};

template<typename t_type, typename t_storage_type, t_type k_minimum_value, t_type k_maximum_value_plus_one>
struct c_enum
{
public:
	c_enum() :
		m_storage(k_minimum_value)
	{
	}

	c_enum(t_type value) :
		m_storage(static_cast<t_storage_type>(value))
	{
	}

	c_enum(t_storage_type value) :
		m_storage(value)
	{
	}

	template<typename T>
	bool operator==(T value) const
	{
		return m_storage == static_cast<t_storage_type>(value);
	}

	template<typename T> const
		bool operator!=(T value)
	{
		return m_storage != static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<(T value) const
	{
		return m_storage < static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>(T value) const
	{
		return m_storage > static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator>=(T value) const
	{
		return m_storage >= static_cast<t_storage_type>(value);
	}

	template<typename T>
	bool operator<=(T value) const
	{
		return m_storage <= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator+=(T value)
	{
		m_storage += static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator-=(T value)
	{
		m_storage -= static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator= (T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template<typename T>
	void operator++()
	{
		m_storage++;
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

	t_type get()
	{
		return static_cast<t_type>(m_storage);
	}

	t_type const get() const
	{
		return static_cast<t_type>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

// #TODO: find an actual home
struct s_cluster_reference
{
	char bsp_index;
	char cluster_index;
};
static_assert(sizeof(s_cluster_reference) == 0x2);

// #TODO: find an actual home
struct s_location
{
	s_cluster_reference cluster_reference;
};
static_assert(sizeof(s_location) == sizeof(s_cluster_reference));

template<long k_maximum_count>
struct c_static_string
{
	using t_type = char[k_maximum_count];

public:
	static long const element_count = k_maximum_count;

	__forceinline t_type& get()
	{
		return m_string;
	}

	c_static_string() :
		m_string{}
	{
		clear();
	}

	c_static_string(char const* s) :
		m_string{}
	{
		clear();
		set(s);
	}

	void null_terminate_buffer()
	{
		m_string[k_maximum_count - 1] = 0;
	}

	void set_character(long index, char character)
	{
		ASSERT(VALID_INDEX(index, k_maximum_count - 1));

		//long initial_length = length();
		//ASSERT(VALID_COUNT(index, initial_length));
		//
		//if (index >= initial_length)
		//{
		//	if (index == initial_length && index < k_maximum_count - 1)
		//	{
		//		m_string[index] = character;
		//		m_string[index + 1] = 0;
		//	}
		//}
		//else
		//{
		//	m_string[index] = character;
		//}

		if (!m_string[index])
		{
			m_string[index + 1] = 0;
		}

		m_string[index] = character;
	}

	void set_length(long length)
	{
		if (VALID_COUNT(length, k_maximum_count - 1))
		{
			m_string[length] = 0;
		}
	}

	void set(char const* s)
	{
		csstrnzcpy(m_string, s, k_maximum_count);
	}

	//void set_wchar(const wchar_t* src);

	void append(char const* s)
	{
		csstrnzcat(m_string, s, k_maximum_count);
	}

	void append_line(char const* s = nullptr)
	{
		if (s != nullptr)
		{
			csstrnzcat(m_string, s, k_maximum_count);
		}
		csstrnzcat(m_string, "\r\n", k_maximum_count);
	}

	char const* print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		cvsnzprintf(m_string, k_maximum_count, format, list);

		va_end(list);

		return m_string;
	}

	char const* print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		cvsnzprintf(m_string, k_maximum_count, format, list);
		append_line();

		va_end(list);

		return m_string;
	}

	char const* print_va(char const* format, va_list list)
	{
		cvsnzprintf(m_string, k_maximum_count, format, list);

		return m_string;
	}

	char const* append_print(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_print_va(format, list);

		va_end(list);
		return result;
	}

	char const* append_print_line(char const* format, ...)
	{
		va_list list;
		va_start(list, format);

		char const* result = append_print_va(format, list);
		append_line();

		va_end(list);
		return result;
	}

	char const* append_print_va(char const* format, va_list list)
	{
		dword current_length = length();

		ASSERT(format);
		ASSERT(current_length >= 0 && current_length < k_maximum_count);

		cvsnzprintf(m_string + current_length, k_maximum_count - current_length, format, list);

		return m_string;
	}

	void clear()
	{
		csmemset(m_string, 0, sizeof(m_string));
	}

	bool is_empty() const
	{
		return !m_string[0];
	}

	char const* get_string() const
	{
		return m_string;
	}

	char const* get_offset(long offset) const
	{
		if (VALID_INDEX(offset, length()))
			return &m_string[offset];

		return "";
	}

	char* get_buffer()
	{
		return m_string;
	}

	long length() const
	{
		return csstrnlen(m_string, k_maximum_count);
	}

	bool is_equal(char const* _string) const
	{
		ASSERT(_string);

		return csstrnlen(_string, k_maximum_count) == length() && csmemcmp(get_string(), _string, length()) == 0;
	}

	bool starts_with(char const* _string) const
	{
		ASSERT(_string);

		return csmemcmp(_string, get_string(), csstrnlen(_string, k_maximum_count)) == 0;
	}

	bool ends_with(char const* _string) const
	{
		ASSERT(_string);

		long _length = length();
		long suffix_length = csstrnlen(_string, k_maximum_count);

		if (suffix_length > _length)
			return false;

		char const* suffix = get_string() + (_length - suffix_length);

		bool result = csmemcmp(suffix, _string, suffix_length) == 0;
		return result;
	}

	long next_index_of(char const* _string, long index) const
	{
		ASSERT(_string);

		long result = NONE;

		if (index < length())
		{
			char const* s = csstrstr(m_string + index, _string);
			if (s)
				result = s - get_string();
		}

		return result;
	}

	long index_of(char const* _string) const
	{
		ASSERT(_string);

		return next_index_of(_string, 0);
	}

	void set_bounded(char const* _string, long _length)
	{
		if (_length + 1 < k_maximum_count)
			_length++;
		else
			_length = k_maximum_count;

		csstrnzcpy(m_string, _string, _length);
	}

	bool substring(long index, long _length, c_static_string<k_maximum_count>& s) const
	{
		if (index < 0 || _length <= 0 || index + _length > length())
			return false;

		s.set_bounded(get_offset(index), _length);

		return true;
	}

	char* copy_to(char* s, unsigned int size)const
	{
		if (size > k_maximum_count)
			size = k_maximum_count;

		return csstrnzcpy(s, m_string, size);
	}

protected:
	char m_string[k_maximum_count];
};

class c_string_builder :
	public c_static_string<1024>
{
public:
	c_string_builder();
	c_string_builder(const char* format, ...);
	~c_string_builder();
};

class c_debug_output_path
{
public:
	c_debug_output_path() :
		m_root(),
		m_path()
	{
		m_root.clear();
		m_path.clear();
	}

	const char* get_path(const char* file_path)
	{
		m_path.print("%s%s", c_debug_output_path::get_root(), file_path);

		return m_path.get_string();
	}

	const char* get_root()
	{
		build_root_if_necessary();

		return m_root.get_string();
	}

private:
	void build_root_if_necessary()
	{
		if (m_root.is_empty())
		{
			m_root.set("");
		}
	}

	c_static_string<260> m_root;
	c_static_string<260> m_path;
};
static_assert(sizeof(c_debug_output_path) == 260 * 2);

extern char* tag_to_string(tag _tag, char* buffer);

struct c_string_id
{
public:
	//char const* get_string();
	//char const* get_string() const;
	long get_value() { return m_value; }

protected:
	string_id m_value;
};

constexpr __int64 make_int64(__int64 a, __int64 b)
{
	return ((a << 0) | (b << 32));
}

// IDA
template<class T>
T rotate_left(T value, int count)
{
	const dword nbits = sizeof(T) * 8;

	if (count > 0)
	{
		count %= nbits;
		T high = value >> (nbits - count);
		if (T(-1) < 0) // signed value
			high &= ~((T(-1) << count));
		value <<= count;
		value |= high;
	}
	else
	{
		count = -count % nbits;
		T low = value << (nbits - count);
		value >>= count;
		value |= low;
	}
	return value;
}

#define __ROL1__(value, count) rotate_left(static_cast<byte>(value), count)
#define __ROL2__(value, count) rotate_left(static_cast<word>(value), count)
#define __ROL4__(value, count) rotate_left(static_cast<dword>(value), count)
#define __ROL8__(value, count) rotate_left(static_cast<qword>(value), count)
#define __ROR1__(value, count) rotate_left(static_cast<byte>(value), -count)
#define __ROR2__(value, count) rotate_left(static_cast<word>(value), -count)
#define __ROR4__(value, count) rotate_left(static_cast<dword>(value), -count)
#define __ROR8__(value, count) rotate_left(static_cast<qword>(value), -count)

extern const real_argb_color* const global_real_argb_white;
extern const real_argb_color* const global_real_argb_grey;
extern const real_argb_color* const global_real_argb_black;
extern const real_argb_color* const global_real_argb_red;
extern const real_argb_color* const global_real_argb_green;
extern const real_argb_color* const global_real_argb_blue;
extern const real_argb_color* const global_real_argb_yellow;
extern const real_argb_color* const global_real_argb_cyan;
extern const real_argb_color* const global_real_argb_magenta;
extern const real_argb_color* const global_real_argb_pink;
extern const real_argb_color* const global_real_argb_lightblue;
extern const real_argb_color* const global_real_argb_orange;
extern const real_argb_color* const global_real_argb_purple;
extern const real_argb_color* const global_real_argb_aqua;
extern const real_argb_color* const global_real_argb_darkgreen;
extern const real_argb_color* const global_real_argb_salmon;
extern const real_argb_color* const global_real_argb_violet;
extern const real_rgb_color* const global_real_rgb_white;
extern const real_rgb_color* const global_real_rgb_grey;
extern const real_rgb_color* const global_real_rgb_black;
extern const real_rgb_color* const global_real_rgb_red;
extern const real_rgb_color* const global_real_rgb_green;
extern const real_rgb_color* const global_real_rgb_blue;
extern const real_rgb_color* const global_real_rgb_yellow;
extern const real_rgb_color* const global_real_rgb_cyan;
extern const real_rgb_color* const global_real_rgb_magenta;
extern const real_rgb_color* const global_real_rgb_pink;
extern const real_rgb_color* const global_real_rgb_lightblue;
extern const real_rgb_color* const global_real_rgb_orange;
extern const real_rgb_color* const global_real_rgb_purple;
extern const real_rgb_color* const global_real_rgb_aqua;
extern const real_rgb_color* const global_real_rgb_darkgreen;
extern const real_rgb_color* const global_real_rgb_salmon;
extern const real_rgb_color* const global_real_rgb_violet;

long bit_count(long val);
long __fastcall index_from_mask(const dword* mask, long bit_count); // first index?

struct c_allocation_base
{
public:
	c_allocation_base();

	virtual void* allocate(dword allocation, char const* name) = 0;
	virtual void deallocate(void* buffer) = 0;
};

struct c_system_allocation :
	public c_allocation_base
{
public:
};

struct c_normal_allocation :
	public c_allocation_base
{
public:
};

struct c_no_allocation :
	public c_allocation_base
{
public:
};

extern c_normal_allocation*& g_normal_allocation;
