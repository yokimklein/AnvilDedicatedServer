#pragma once
#include <memory\data.h>
#include <objects\objects.h>

struct _crate_datum
{
	long self_destruct; // timer?
};
static_assert(sizeof(_crate_datum) == 0x4);

struct crate_datum
{
	long definition_index;
	_object_datum object;
	_crate_datum crate;
};
static_assert(sizeof(crate_datum) == 0x180);

bool crate_will_self_destruct(datum_index crate_index);