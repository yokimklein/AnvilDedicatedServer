#pragma once
#include <cseries\cseries.h>

class c_synchronized_long
{
public:
	c_synchronized_long(long starting_value);
	c_synchronized_long();
	~c_synchronized_long();

	operator long() const;

	long set(long value);
	long set_if_equal(long value, long comperand);
	long peek() const;
	long add(long value);
	long increment();
	long decrement();
	long and_(long value);
	long or_(long value);
	long xor_(long value);
	long set_bit(long index, bool setting);
	bool test_bit(long index) const;

	c_synchronized_long& operator+=(long value);
	c_synchronized_long& operator-=(long value);
	c_synchronized_long& operator&=(long value);
	c_synchronized_long& operator|=(long value);
	c_synchronized_long& operator=(const c_synchronized_long& other);
	c_synchronized_long& operator=(bool value);
	c_synchronized_long& operator=(long value);

private:
	volatile long m_value;
};
static_assert(sizeof(c_synchronized_long) == 0x4);

//class c_synchronized_int64
//{
//public:
//	c_synchronized_int64(long64 starting_value);
//	c_synchronized_int64();
//	~c_synchronized_int64();
//
//	operator long64() const;
//
//	long64 set(long64 value);
//	long64 set_if_equal(long64 value, long64 comperand);
//	long64 peek() const;
//	long64 add(long64 value);
//	long64 increment();
//	long64 decrement();
//	long64 and_(long64 value);
//	long64 or_(long64 value);
//	long64 xor_(long64 value);
//
//	c_synchronized_int64& operator+=(long64 value);
//	c_synchronized_int64& operator-=(long64 value);
//	c_synchronized_int64& operator&=(long64 value);
//	c_synchronized_int64& operator|=(long64 value);
//	c_synchronized_int64& operator=(const c_synchronized_int64& other);
//	c_synchronized_int64& operator=(bool value);
//	c_synchronized_int64& operator=(long64 value);
//
//private:
//	volatile long64 m_value;
//};
//static_assert(sizeof(c_synchronized_int64) == 0x8);

class c_interlocked_long
{
public:
	c_interlocked_long(long starting_value);
	c_interlocked_long();
	~c_interlocked_long();

	operator long() const;

	long set(long value);
	long set_if_equal(long value, long comperand);
	long peek() const;
	long add(long value);
	long increment() volatile;
	long decrement() volatile;
	long and_(long value);
	long or_(long value);
	long xor_(long value);
	long set_bit(long index, bool setting);
	bool test_bit(long index) const;

	c_interlocked_long& operator+=(long value);
	c_interlocked_long& operator-=(long value);
	c_interlocked_long& operator&=(long value);
	c_interlocked_long& operator|=(long value);
	c_interlocked_long& operator=(const c_interlocked_long& other);
	c_interlocked_long& operator=(bool value);
	c_interlocked_long& operator=(long value);

private:
	volatile long m_value;
};
static_assert(sizeof(c_interlocked_long) == 0x4);

//class c_interlocked_int64
//{
//public:
//	c_interlocked_int64(long64 starting_value);
//	c_interlocked_int64();
//	~c_interlocked_int64();
//
//	operator long64() const;
//
//	long64 set(long64 value);
//	long64 set_if_equal(long64 value, long64 comperand);
//	long64 peek() const;
//	long64 add(long64 value);
//	long64 increment();
//	long64 decrement();
//	long64 and_(long64 value);
//	long64 or_(long64 value);
//	long64 xor_(long64 value);
//
//	c_interlocked_int64& operator+=(long64 value);
//	c_interlocked_int64& operator-=(long64 value);
//	c_interlocked_int64& operator&=(long64 value);
//	c_interlocked_int64& operator|=(long64 value);
//	c_interlocked_int64& operator=(const c_interlocked_int64& other);
//	c_interlocked_int64& operator=(bool value);
//	c_interlocked_int64& operator=(long64 value);
//
//private:
//	volatile long64 m_value;
//};
//
//template<long k_count>
//class c_synchronized_array
//{
//public:
////	c_synchronized_array();
////	~c_synchronized_array();
////
////	c_synchronized_long& get_member(long index);
////	c_synchronized_long& operator[](long index);
////	void set_array(long value);
////	c_synchronized_array& operator=(c_synchronized_array& other);
////
////private:
//	c_synchronized_long m_value[k_count];
//};
//static_assert(sizeof(c_synchronized_array<1>) == (sizeof(c_synchronized_long) * 1));
//
//template<long k_bit_count>
//class c_synchronized_bitvector :
//	public c_synchronized_array<((k_bit_count + (32 - 1)) >> 5)>
//{
//};
//static_assert(sizeof(c_synchronized_bitvector<256>) == sizeof(c_synchronized_array<8>));
