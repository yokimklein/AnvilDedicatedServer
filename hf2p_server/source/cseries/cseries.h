#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/cseries/cseries.hpp

#include <math\integer_math.h>
#include <math\real_math.h>

#include <stdarg.h> 
#include <windows.h>
#include <assert.h>

extern const size_t module_base;
#define BASE_ADDRESS(ADDR) (module_base + ADDR)
// module_base isn't instatiated in time for reference macros to use it, so we're calling GetModuleHandle directly for now
#define BASE_ADDRESS_REFERENCE(ADDR)((size_t)GetModuleHandle(NULL) + ADDR)

#define _STRCONCAT(x, y) x ## y
#define STRCONCAT(x, y) _STRCONCAT(x, y)

#define DECLFUNC(ADDR, R, CC, ...) reinterpret_cast<R(CC*)(__VA_ARGS__)>(BASE_ADDRESS(ADDR))
#define INVOKE(ADDR, TYPE, ...) reinterpret_cast<decltype(TYPE)*>(BASE_ADDRESS(ADDR))(__VA_ARGS__)

#define OFFSETOF(s,m) __builtin_offsetof(s,m)
#define NUMBEROF(_array) (sizeof(_array) / sizeof(_array[0]))
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

#define REFERENCE_DECLARE(address, type, name) type& name = *reinterpret_cast<type*>(BASE_ADDRESS_REFERENCE(address))
#define REFERENCE_DECLARE_ARRAY(address, type, name, count) type(&name)[count] = *reinterpret_cast<type(*)[count]>(BASE_ADDRESS_REFERENCE(address))
#define REFERENCE_DECLARE_STATIC_ARRAY(address, type, count, name) c_static_array<type, count> &name = *reinterpret_cast<c_static_array<type, count>*>(BASE_ADDRESS_REFERENCE(address))

#define FLOOR(a, b) ((a) <= (b) ? (b) : (a))
#define MIN(x, low) ((x) <= (low) ? (x) : (low))
#define MAX(x, high) ((x) >= (high) ? (x) : (high))
#define CLAMP(x, low, high) ((x) < (low) ? (low) : (x) > (high) ? (high) : (x))
#define CLAMP_LOWER(x, low, high) ((x) >= (high) - (low) ? (x) - (high) : (low))
#define CLAMP_UPPER(x, low, high) ((x) <= (high) - (low) ? (x) + (low) : (high))

#define try_bool(X) if (!X) return false

// 4-character tag group identifier
typedef unsigned long tag;
static_assert(sizeof(tag) == 0x4);

enum : tag
{
	_tag_none = 0xFFFFFFFF
};

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char uchar;
static_assert(sizeof(uchar) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short ushort;
static_assert(sizeof(ushort) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long ulong;
static_assert(sizeof(ulong) == 0x4);

// 64-bit signed integer ranging from -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef long long long64;
static_assert(sizeof(long64) == 0x8);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long ulong64;
static_assert(sizeof(ulong64) == 0x8);

// 32-character ascii string
typedef char string[32];
static_assert(sizeof(string) == 0x20);

// 256-character ascii string
typedef char long_string[256];
static_assert(sizeof(long_string) == 0x100);

// a 32-bit string identifier
typedef unsigned long string_id;
static_assert(sizeof(string_id) == 0x4);

// 8-bit unsigned integer ranging from 0 to 255
typedef unsigned char byte;
static_assert(sizeof(byte) == 0x1);

// 16-bit unsigned integer ranging from 0 to 65,535
typedef unsigned short word;
static_assert(sizeof(word) == 0x2);

// 32-bit unsigned integer ranging from 0 to 4,294,967,295
typedef unsigned long dword;
static_assert(sizeof(dword) == 0x4);

// 64-bit unsigned integer ranging from 0 to 18,446,744,073,709,551,615
typedef unsigned long long qword;
static_assert(sizeof(qword) == 0x8);

// 8-bit enumerator value
typedef char char_enum;
static_assert(sizeof(char_enum) == 0x1);

// 16-bit enumerator value
typedef short short_enum;
static_assert(sizeof(short_enum) == 0x2);

// 32-bit enumerator value
typedef long long_enum;
static_assert(sizeof(long_enum) == 0x4);

// 8-bit flags container
typedef byte byte_flags;
static_assert(sizeof(byte_flags) == 0x1);

// 16-bit flags container
typedef word word_flags;
static_assert(sizeof(word_flags) == 0x2);

// 32-bit flags container
typedef dword dword_flags;
static_assert(sizeof(dword_flags) == 0x4);

// 32-bit floating-point number ranging from 1.175494351e-38F to 3.402823466e+38F
typedef real real;
static_assert(sizeof(real) == 0x4);

typedef char utf8;

#define SIZEOF_BITS(value) 8 * sizeof(value)

const long LONG_BITS = SIZEOF_BITS(long);

#define FLAG(bit) (1 << (bit))
#define MASK(bit) ((1 << (bit)) - 1)
#define TEST_BIT(flags, bit) (((flags) & (1 << (bit))) != 0)
#define TEST_FLAG(flags, bit) (flags.test((bit)))
#define TEST_MASK(flags, mask) (((flags) & mask) != 0)
#define ALIGN(value, bit) (((value) & ~((1 << (bit)) - 1)) + (1 << (bit)))
#define ALIGN_UP(value, bit) ((((value) & ((1 << (bit)) - 1)) == 0) ? (value) : ((value) | ((1 << (bit)) - 1)) + 1)
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

extern int(__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size);
extern void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size);
extern void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size);

extern long csstricmp(char const* s1, char const* s2);
extern long csstrcmp(char const* s1, char const* s2);
extern long csstrncmp(char const* s1, char const* s2, dword size);
//extern long csstrnicmp(char const* s1, char const* s2, dword size);
//extern char* csstristr(char const* s1, char const* s2);
extern char* csstrnzcpy(char* s1, char const* s2, dword size);
extern char* csstrnzcat(char* s1, char const* s2, dword size);
extern dword csstrnlen(char const* s, dword size);
extern char* csstrnupr(char* s, dword size);
extern char* csstrnlwr(char* s, dword size);
extern char const* csstrstr(char const* s1, char const* s2);
//extern char* csstrtok(char*, char const*, bool, struct csstrtok_data* data);
extern long cvsnzprintf(char* buffer, dword size, char const* format, va_list list);
extern char* csnzprintf(char* buffer, dword size, char const* format, ...);
extern char* csnzappendf(char* buffer, dword size, char const* format, ...);
extern bool string_is_not_empty(char const* s);
extern char* strncpy_debug(char* s1, dword size1, char const* s2, dword size2);
extern long strlen_debug(char const* s);

template<typename t_type, long k_count>
void zero_array(t_type(&data)[k_count])
{
	csmemset(data, 0, sizeof(t_type) * k_count);
}

long __fastcall bit_vector_count_bits(const dword* bit_mask, int bit_count);

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
			m_storage[i] = value;
	}

	long get_total_element_size() const
	{
		return sizeof(t_type) * k_count;
	}

	t_type const& operator[](long index) const
	{
		assert(valid(index));

		return m_storage[index];
	}

	t_type& operator[](long index)
	{
		assert(valid(index));

		return m_storage[index];
	}

protected:
	t_type m_storage[k_count];
};

template<typename t_type, typename t_storage_type, long k_count>
struct c_flags //: public c_flags_no_init<t_type, t_storage_type, k_count>
{
	static t_type const k_maximum_count = (t_type)k_count;
	//static_assert(k_maximum_count <= SIZEOF_BITS(t_storage_type));

public:
	c_flags() :
		m_storage(0)
	{

	}

	c_flags(t_storage_type raw_bits) :
		m_storage(raw_bits)
	{

	}

	t_storage_type get_unsafe() const
	{
		return m_storage;
	}

	void set_unsafe(t_storage_type raw_bits)
	{
		m_storage = raw_bits;
	}

	void set(t_type bit, bool enable)
	{
		if (bit < k_maximum_count)
		{
			if (enable)
				m_storage |= (1LL << bit);
			else
				m_storage &= ~(1LL << bit);
		}
	}

	void clear()
	{
		m_storage = 0;
	}

	bool is_clear()
	{
		return m_storage == 0;
	}

	bool is_empty() const
	{
#pragma warning(push)
#pragma warning(disable : 4293)
		return (m_storage & (MASK(SIZEOF_BITS(t_storage_type)) >> (SIZEOF_BITS(t_storage_type) - k_maximum_count))) == 0;
#pragma warning(pop)
	}

	bool valid_bit(t_type bit)
	{
		return VALID_INDEX(0, k_maximum_count);
	}

	bool valid_bit(t_type bit) const
	{
		return VALID_INDEX(0, k_maximum_count);
	}

	bool valid() const
	{
		return (m_storage & MASK(k_maximum_count)) == 0;
	}

	bool test(t_type bit)
	{
		assert(valid_bit(bit));

		return TEST_BIT(m_storage, static_cast<t_storage_type>(bit));
	}

	bool test(t_type bit) const
	{
		assert(valid_bit(bit));

		return TEST_BIT(m_storage, static_cast<t_storage_type>(bit));
	}

	bool operator==(c_flags<t_type, t_storage_type, k_maximum_count>& value)
	{
		return m_storage == value.m_storage;
	}

	bool operator==(t_type value)
	{
		return !!(m_storage & (1 << value));
	}

	bool operator!=(t_type value)
	{
		return m_storage != value;
	}

	void operator|=(t_storage_type raw_bits)
	{
		m_storage |= raw_bits;
	}

	void operator&=(t_storage_type raw_bits)
	{
		m_storage &= raw_bits;
	}

	c_flags<t_type, t_storage_type, k_maximum_count> operator|(c_flags<t_type, t_storage_type, k_maximum_count> const& other) const
	{
		return c_flags(m_storage | other.m_storage);
	}

	template <class T>
	void operator=(T value)
	{
		m_storage = static_cast<t_storage_type>(value);
	}

	template <class T>
	operator T () const
	{
		return static_cast<T>(m_storage);
	}

protected:
	t_storage_type m_storage;
};

template<long k_maximum_count>
struct c_static_flags_no_init
{
	static long const MAX_COUNT = k_maximum_count;

	void and_(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		assert(vector_a);

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_storage[i] &= vector_a[i];
	}

	void and_not_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		dword const* vector_a_bits = vector_a->get_bits_direct();
		dword const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_storage[i] = vector_a_bits[i] & ~vector_b_bits[i];
	}

	void and_range(c_static_flags_no_init<k_maximum_count> const* vector_a, c_static_flags_no_init<k_maximum_count> const* vector_b, long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		dword const* vector_a_bits = vector_a->get_bits_direct();
		dword const* vector_b_bits = vector_b->get_bits_direct();

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_storage[i] = vector_a_bits[i] & vector_b_bits[i];
	}

	void clear()
	{
		csmemset(m_storage, 0, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void clear_range(long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_storage, 0, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void copy(c_static_flags_no_init<k_maximum_count> const* vector_a)
	{
		csmemcpy(m_storage, vector_a, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	long count_bits_set() const
	{
		return bit_vector_count_bits(m_storage, k_maximum_count);
	}

	void fill(long count, byte fill_value)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_storage, fill_value, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	dword const* get_bits_direct() const
	{
		return m_storage;
	}

	dword* get_writeable_bits_direct()
	{
		return m_storage;
	}

	long highest_bit_set_in_range(long count) const
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		return bit_vector_highest_bit_set(m_storage, count);
	}

	void invert_bits()
	{
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_storage[i] = ~m_storage[i];

		// no clue
		//m_storage[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1] = (byte)m_storage[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)-1];
	}

	bool is_clear() const
	{
		byte result = 1;
		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			result &= m_storage[i] == 0;

		return result;
	}

	void keep_range(long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = BIT_VECTOR_SIZE_IN_LONGS(count); i < BIT_VECTOR_SIZE_IN_LONGS(count); ++i)
			m_storage[i] = 0;

		m_storage[BIT_VECTOR_SIZE_IN_LONGS(count) - 1] &= ((count & (LONG_BITS - 1)) != 0) ? 0xFFFFFFFF >> (LONG_BITS - (count & (LONG_BITS - 1))) : 0xFFFFFFFF;
	}

	void or_bits(dword const* bits, long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		for (long i = 0; i < BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count); i++)
			m_storage[i] |= bits[i];
	}

	void set(long index, bool enable)
	{
		assert(VALID_INDEX(index, k_maximum_count));

		if (enable)
			BIT_VECTOR_OR_FLAG(m_storage, index);
		else
			BIT_VECTOR_AND_FLAG(m_storage, index);
	}

	void set_all()
	{
		csmemset(m_storage, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(k_maximum_count));
	}

	void set_bits_direct_destructive(long count, dword const* bits)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemcpy(m_storage, bits, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	void set_range(long count)
	{
		assert(IN_RANGE_INCLUSIVE(count, 0, k_maximum_count));

		csmemset(m_storage, 0xFFFFFFFF, BIT_VECTOR_SIZE_IN_BYTES(count));
	}

	bool test(long index) const
	{
		assert(VALID_INDEX(index, k_maximum_count));

		return BIT_VECTOR_TEST_FLAG(m_storage, index);
	}

	dword m_storage[BIT_VECTOR_SIZE_IN_LONGS(k_maximum_count)];
};

template<long k_maximum_count>
struct c_static_flags : public c_static_flags_no_init<k_maximum_count>
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
		assert(VALID_INDEX(index, k_maximum_count - 1));

		//long initial_length = length();
		//assert(VALID_COUNT(index, initial_length));
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
			m_string[index + 1] = 0;

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

	void append(char const* s)
	{
		csstrnzcat(m_string, s, k_maximum_count);
	}

	void append_line(char const* s = nullptr)
	{
		if (s != nullptr)
			csstrnzcat(m_string, s, k_maximum_count);
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

		assert(format);
		assert(current_length >= 0 && current_length < k_maximum_count);

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
		assert(_string);

		return csstrnlen(_string, k_maximum_count) == length() && csmemcmp(get_string(), _string, length()) == 0;
	}

	bool starts_with(char const* _string) const
	{
		assert(_string);

		return csmemcmp(_string, get_string(), csstrnlen(_string, k_maximum_count)) == 0;
	}

	bool ends_with(char const* _string) const
	{
		assert(_string);

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
		assert(_string);

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
		assert(_string);

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

extern __int64 make_int64(__int64 a, __int64 b);

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

extern real_argb_color const* const& global_real_argb_white;
extern real_argb_color const* const& global_real_argb_grey;
extern real_argb_color const* const& global_real_argb_black;
extern real_argb_color const* const& global_real_argb_red;
extern real_argb_color const* const& global_real_argb_green;
extern real_argb_color const* const& global_real_argb_blue;
extern real_argb_color const* const& global_real_argb_yellow;
extern real_argb_color const* const& global_real_argb_cyan;
extern real_argb_color const* const& global_real_argb_magenta;
extern real_argb_color const* const& global_real_argb_pink;
extern real_argb_color const* const& global_real_argb_lightblue;
extern real_argb_color const* const& global_real_argb_orange;
extern real_argb_color const* const& global_real_argb_purple;
extern real_argb_color const* const& global_real_argb_aqua;
extern real_argb_color const* const& global_real_argb_darkgreen;
extern real_argb_color const* const& global_real_argb_salmon;
extern real_argb_color const* const& global_real_argb_violet;
extern real_rgb_color const* const& global_real_rgb_white;
extern real_rgb_color const* const& global_real_rgb_grey;
extern real_rgb_color const* const& global_real_rgb_black;
extern real_rgb_color const* const& global_real_rgb_red;
extern real_rgb_color const* const& global_real_rgb_green;
extern real_rgb_color const* const& global_real_rgb_blue;
extern real_rgb_color const* const& global_real_rgb_yellow;
extern real_rgb_color const* const& global_real_rgb_cyan;
extern real_rgb_color const* const& global_real_rgb_magenta;
extern real_rgb_color const* const& global_real_rgb_pink;
extern real_rgb_color const* const& global_real_rgb_lightblue;
extern real_rgb_color const* const& global_real_rgb_orange;
extern real_rgb_color const* const& global_real_rgb_purple;
extern real_rgb_color const* const& global_real_rgb_aqua;
extern real_rgb_color const* const& global_real_rgb_darkgreen;
extern real_rgb_color const* const& global_real_rgb_salmon;
extern real_rgb_color const* const& global_real_rgb_violet;

long bit_count(long val);
long __fastcall index_from_mask(dword* mask, long bit_count); // first index?

struct c_allocation_base
{
public:
	virtual void* allocate(dword allocation, char const* name);
	virtual void deallocate(void* buffer);
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