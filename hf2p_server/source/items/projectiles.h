#pragma once
#include <items\items.h>

struct _projectile_datum
{
	// TODO
};

struct projectile_datum
{
	long definition_index;
	_object_datum object;
	_item_datum item;
};
static_assert(sizeof(projectile_datum) == 0x190); // TODO: VERIFY SIZE