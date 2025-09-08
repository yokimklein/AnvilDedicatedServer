#include "synchronized_value.h"
#include <windows.h>

c_synchronized_long::c_synchronized_long(long starting_value)
	: m_value(starting_value)
{
}

c_synchronized_long::c_synchronized_long()
	: m_value(0)
{
}

c_synchronized_long::~c_synchronized_long()
{
}

c_synchronized_long::operator long() const
{
	return m_value;
}

long c_synchronized_long::set(long value)
{
	long result = InterlockedExchange(&m_value, value);
	return result;
}

long c_synchronized_long::set_if_equal(long value, long comperand)
{
	long result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

long c_synchronized_long::peek() const
{
	return m_value;
}

long c_synchronized_long::add(long value)
{
	long result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

long c_synchronized_long::increment()
{
	long result = InterlockedIncrement(&m_value);
	return result;
}

long c_synchronized_long::decrement()
{
	long result = InterlockedDecrement(&m_value);
	return result;
}

long c_synchronized_long::and_(long value)
{
	InterlockedAdd(&m_value, value);
	m_value &= value;
	return m_value;
}

long c_synchronized_long::or_(long value)
{
	m_value |= value;
	return m_value;
}

long c_synchronized_long::xor_(long value)
{
	m_value ^= value;
	return m_value;
}

long c_synchronized_long::set_bit(long index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_synchronized_long::test_bit(long index) const
{
	return TEST_BIT(m_value, index);
}

c_synchronized_long& c_synchronized_long::operator+=(long value)
{
	add(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator-=(long value)
{
	add(-value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator&=(long value)
{
	and_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator|=(long value)
{
	or_(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(const c_synchronized_long& other)
{
	set(other.m_value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(bool value)
{
	set(value);
	return *this;
}

c_synchronized_long& c_synchronized_long::operator=(long value)
{
	InterlockedExchange(&m_value, value);
	return *this;
}

c_interlocked_long::c_interlocked_long(long starting_value) :
	m_value(starting_value)
{
}

c_interlocked_long::c_interlocked_long() :
	m_value(0)
{
}

c_interlocked_long::~c_interlocked_long()
{
}

c_interlocked_long::operator long() const
{
	return peek();
}

long c_interlocked_long::set(long value)
{
	long result = InterlockedExchange(&m_value, value);
	return result;
}

long c_interlocked_long::set_if_equal(long value, long comperand)
{
	long result = InterlockedCompareExchange(&m_value, value, comperand);
	return result;
}

long c_interlocked_long::peek() const
{
	return m_value;
}

long c_interlocked_long::add(long value)
{
	long result = InterlockedExchangeAdd(&m_value, value);
	return result;
}

long c_interlocked_long::increment() volatile
{
	long result = InterlockedIncrement(&m_value);
	return result;
}

long c_interlocked_long::decrement() volatile
{
	long result = InterlockedDecrement(&m_value);
	return result;
}

long c_interlocked_long::and_(long value)
{
	m_value &= value;
	return m_value;
}

long c_interlocked_long::or_(long value)
{
	m_value |= value;
	return m_value;
}

long c_interlocked_long::xor_(long value)
{
	m_value ^= value;
	return m_value;
}

long c_interlocked_long::set_bit(long index, bool setting)
{
	SET_BIT(m_value, index, setting);
	return m_value;
}

bool c_interlocked_long::test_bit(long index) const
{
	bool result = TEST_BIT(m_value, index);
	return result;
}

c_interlocked_long& c_interlocked_long::operator+=(long value)
{
	long result = add(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator-=(long value)
{
	long result = add(-value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator&=(long value)
{
	long result = and_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator|=(long value)
{
	long result = or_(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(const c_interlocked_long& other)
{
	long result = set(other.m_value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(bool value)
{
	long result = set(value);
	return *this;
}

c_interlocked_long& c_interlocked_long::operator=(long value)
{
	long result = set(value);
	return *this;
}