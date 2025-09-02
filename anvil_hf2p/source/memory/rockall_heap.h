#pragma once

class ROCKALL_FRONT_END
{
public:
	virtual bool Delete(void*, int);
	virtual bool Details(void*, int*);
	virtual bool KnownArea(void*);
	virtual bool MultipleDelete(int, void** const, int);
	virtual bool MultipleNew(int*, void** const, int, int, int*, bool);
	virtual void* New(int, int*, bool);
	virtual void* Resize(void*, int, int, int*, bool, bool);
	virtual bool Verify(void*, int*);
	virtual void DeleteAll(bool);
	virtual void LockAll(void);
	virtual bool Truncate(int);
	virtual void UnlockAll(void);
	virtual bool Walk(bool*, void**, int*);
	virtual ~ROCKALL_FRONT_END();
	virtual void Exception(char*);
	virtual void* SpecialNew(int);
};

class c_fixed_memory_rockall_frontend : public ROCKALL_FRONT_END
{

};

class c_rockall_heap
{
public:
	void* allocate(long size);
	long get_allocation_size(void* block);
	void deallocate(void* block);

	c_fixed_memory_rockall_frontend* m_rockall_front_end;
	long count;
};