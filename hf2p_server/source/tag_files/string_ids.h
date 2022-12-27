#pragma once

// https://github.com/theTwist84/ManagedDonkey/blob/main/game/source/tag_files/string_ids.hpp

// halo 3, odst and halo online use a 16 bit namespace
// halo reach uses a 17 bit namespace
#define STRING_NAMESPACE_BITS 16

#define STRING_ID(NAMESPACE, STRING) (_string_namespace_##NAMESPACE << STRING_NAMESPACE_BITS) + _##NAMESPACE##_string_id_##STRING
#define STRING_NAMESPACE_FROM_STRING_ID(ID) (ID >> STRING_NAMESPACE_BITS)
#define STRING_INDEX_FROM_STRING_ID(ID) (ID & ((1 << STRING_NAMESPACE_BITS) - 1))

enum e_string_namespace
{
	_string_namespace_global = 0,
	_string_namespace_gui,
	_string_namespace_gui_alert,
	_string_namespace_gui_dialog,

	// Reach
	//_string_namespace_cui,
	//_string_namespace_cui_animation,
	//_string_namespace_cui_shader,
	//_string_namespace_properties,
	//_string_namespace_components,

	_string_namespace_game_engine,
	_string_namespace_game_start,
	_string_namespace_online,
	_string_namespace_saved_game,
	_string_namespace_gpu,

	// Reach
	//_string_namespace_ai_stimulus,
	//_string_namespace_incident,

	// MCC
	//_string_namespace_input,

	k_string_namespace_count
};

enum e_string_id
{
	// reach: k_string_id_invalid
	_string_id_invalid = 0xFFFFFFFF,

	// reach: k_string_id_empty_string
	_string_id_empty_string = 0,
};

// TODO: confirm ms29 stringids
enum e_global_string_id
{
};

enum e_gui_string_id
{
};

enum e_gui_alert_string_id
{
};

enum e_gui_dialog_string_id
{
};

enum e_game_engine_string_id
{
};

enum e_game_start_string_id
{
};

enum e_online_string_id
{
};

enum e_saved_game_string_id
{
};

enum e_gpu_string_id
{
};

enum e_input_string_id
{
};

//static_assert(0x611 == k_global_string_id_count);
//static_assert(0x684 == k_gui_string_id_count);
//static_assert(0x09B == k_gui_alert_string_id_count);
//static_assert(0x058 == k_gui_dialog_string_id_count);
//static_assert(0x0F7 == k_game_engine_string_id_count);
//static_assert(0x04C == k_game_start_string_id_count);
//static_assert(0x02F == k_online_string_id_count);
//static_assert(0x018 == k_saved_game_string_id_count);
//static_assert(0x00D == k_gpu_string_id_count);
//static_assert(0x073 == k_input_string_id_count);

//extern char const* string_id_get_string_const(long string_id);
//extern long string_id_retrieve(char const* string);
//
//extern void string_id_initialize();