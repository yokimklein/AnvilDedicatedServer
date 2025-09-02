#pragma once
#include <cseries\cseries.h>

enum e_overlapped_task_state
{
	_overlapped_task_state_none = 0,
	_overlapped_task_state_starting,
	_overlapped_task_state_pending,
	_overlapped_task_state_completing,
	_overlapped_task_state_succeeded,
	_overlapped_task_state_failed,

	k_overlapped_task_state_count
};

struct c_overlapped_task
{
	enum e_overlapped_task_flags
	{
		_unused_bit = 0,
		_restarted_during_completion_bit,

		k_number_of_overlapped_task_flags
	};

public:
	virtual ~c_overlapped_task();
	virtual const char* get_context_string() const = 0;
	virtual dword start(void* overlapped) = 0;
	virtual void update(dword return_result);
	virtual void success(dword return_result);
	virtual void failure(dword calling_result, dword overlapped_error, dword overlapped_extended_error);
	virtual void complete();
	virtual void reset();
	virtual bool is_result_successful(dword calling_result, dword overlapped_error, dword overlapped_extended_error);

	//e_overlapped_task_state get_task_state() const;
	//void set_task_state_internal(e_overlapped_task_state task_state);
	//void task_recycled_during_completion(bool recycled_during_completion);
	//bool task_was_recycled_during_completion() const;

	//c_overlapped_task* constructor(char const* file, long line);

	//bool busy() const;

	//char const* get_file() const;
	//void set_file(char const* file);
	//long get_line() const;
	//void set_line(long line);

protected:
	c_flags<e_overlapped_task_flags, short, k_number_of_overlapped_task_flags> m_task_flags;
	c_enum<e_overlapped_task_state, short, _overlapped_task_state_none, k_overlapped_task_state_count> m_task_state;
	char const* m_file;
	long m_line;
};
static_assert(sizeof(c_overlapped_task) == 0x10);
