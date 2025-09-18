#include "input_windows.h"

REFERENCE_DECLARE(0x3EDD060, s_input_globals, input_globals);

bool __fastcall input_get_key(s_key_state* key, e_input_type input_type)
{
	if (input_globals.input_type_suppressed[input_type])
	{
		return false;
	}

	ASSERT(input_globals.buffered_key_read_index >= 0 && input_globals.buffered_key_read_index < MAXIMUM_BUFFERED_KEYSTROKES);

	// this ui check exists in ms23 but not ms29
	if (/*input_globals.input_type_suppressed[_input_type_ui] != input_globals.input_type_suppressed[input_type] ||*/
		input_globals.buffered_key_read_index >= input_globals.buffered_key_read_count)
	{
		return false;
	}

	*key = input_globals.buffered_keys[input_globals.buffered_key_read_index];
	ASSERT(input_globals.buffered_key_read_index >= 0 && input_globals.buffered_key_read_index < MAXIMUM_BUFFERED_KEYSTROKES);
	input_globals.buffered_key_read_index++;
	return true;
}

void input_suppress()
{
    input_globals.input_suppressed = true;
}

bool input_peek_key(s_key_state* key, e_input_type input_type)
{
	if (input_globals.input_type_suppressed[input_type] || input_globals.buffered_key_read_index >= input_globals.buffered_key_read_count)
	{
		return false;
	}
	
	ASSERT(input_globals.buffered_key_read_index >= 0 && input_globals.buffered_key_read_index < MAXIMUM_BUFFERED_KEYSTROKES);

	*key = input_globals.buffered_keys[input_globals.buffered_key_read_index];
	return true;
}

byte input_key_frames_down(e_input_key_code key_code, e_input_type input_type)
{
	if (input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed)
	{
		return 0;
	}
	
	switch (key_code)
	{
		case _key_left_windows:
		case _key_right_windows:
		case _key_windows:
		{
			return 0;
		}
		case _key_shift:
		{
			return MAX(input_globals.keys[_key_left_shift].frames, input_globals.keys[_key_right_shift].frames);
		}
		case _key_control:
		{
			return MAX(input_globals.keys[_key_left_control].frames, input_globals.keys[_key_right_control].frames);
		}
		case _key_alt:
		{
			return MAX(input_globals.keys[_key_left_alt].frames, input_globals.keys[_key_right_alt].frames);
		}
	}
	
	return input_globals.keys[key_code].frames;
}

mouse_state* input_get_mouse_state(e_input_type input_type)
{
	if (!input_globals.raw_input_mouse_state_update)
	{
		return NULL;
	}

	if (input_globals.input_type_suppressed[input_type] || input_globals.input_suppressed)
	{
		return &input_globals.suppressed_raw_mouse_state;
	}

	return &input_globals.raw_mouse_state;
}

void input_clear_all_rumblers()
{
	INVOKE(0x817E0, input_clear_all_rumblers);
}

void __fastcall input_update()
{
	INVOKE(0x80C10, input_update);
}
