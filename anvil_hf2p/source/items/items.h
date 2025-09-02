#pragma once
#include <objects\objects.h>

enum e_item_inventory_state
{
	_item_inventory_state_free = 0,
	_item_inventory_state_readied,
	_item_inventory_state_hidden,
	_item_inventory_state_stowed,

	k_item_inventory_state_count
};

struct _item_datum
{
	byte_flags flags;
	c_enum<e_item_inventory_state, byte, _item_inventory_state_free, k_item_inventory_state_count> inventory_state;
	short detonation_ticks;
	datum_index ignore_object_index;
	long last_relevant_time;
	datum_index inventory_unit_index;
	long last_inventory_unit_index;
};
static_assert(sizeof(_item_datum) == 0x14);
static_assert(0x00 == OFFSETOF(_item_datum, flags));
static_assert(0x01 == OFFSETOF(_item_datum, inventory_state));
static_assert(0x02 == OFFSETOF(_item_datum, detonation_ticks));
static_assert(0x08 == OFFSETOF(_item_datum, last_relevant_time));
static_assert(0x0C == OFFSETOF(_item_datum, inventory_unit_index));
static_assert(0x10 == OFFSETOF(_item_datum, last_inventory_unit_index));

struct item_datum
{
	long definition_index;
	_object_datum object;
	_item_datum item;
};
static_assert(sizeof(item_datum) == 0x190);