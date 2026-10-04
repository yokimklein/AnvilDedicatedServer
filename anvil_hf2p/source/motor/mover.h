#pragma once
#include <objects\objects.h>

struct _motor_datum
{
	object_header_block_reference motor_tasks;
	object_header_block_reference motor_state;
	object_header_block_reference action_state_storage;
#if ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
	long unknownC; // new in ms30, exact owner (motor or unit) unverified
#endif
};
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
static_assert(sizeof(_motor_datum) == 0xC);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
static_assert(sizeof(_motor_datum) == 0x10);
#endif

struct motor_datum
{
	long definition_index;
	_object_datum object;
	_motor_datum motor;
};
#if ENGINE_VERSION == ENGINE_VERSION_ID(11, 1, 604673)
static_assert(sizeof(motor_datum) == 0x188);
#elif ENGINE_VERSION == ENGINE_VERSION_ID(12, 1, 700255)
static_assert(sizeof(motor_datum) == 0x180);
#endif