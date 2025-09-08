#include "edit_text.h"
#include "cseries\cseries.h"
#include "input\input_windows.h"
#include <string.h>

#define valid_edit_text(edit) ((edit) && (edit)->buffer && (edit)->maximum_length > 0 && short(strlen((edit)->buffer)) <= (edit)->maximum_length)

void edit_text_new(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_selection_reset(edit);
}

void edit_text_fix_selection(edit_text* edit)
{
	short text_len = short(strlen(edit->buffer));

	if (edit->insertion_point_index <= 0)
	{
		edit->insertion_point_index = 0;
	}

	short text_len11 = text_len;
	if (edit->insertion_point_index < text_len)
	{
		text_len11 = edit->insertion_point_index;
	}

	word selection_start_index = edit->selection_start_index;
	bool v8 = edit->selection_start_index <= NONE;
	edit->insertion_point_index = text_len;

	short v5 = NONE;
	if (!v8)
	{
		v5 = selection_start_index;
	}
	if (v5 < text_len)
	{
		text_len = NONE;
		if (selection_start_index > NONE)
		{
			text_len = selection_start_index;
		}
	}

	edit->selection_start_index = text_len;
	if (text_len11 == text_len)
	{
		edit->selection_start_index = NONE;
	}
}

void edit_text_selection_reset(edit_text* edit)
{
	ASSERT(valid_edit_text(edit));

	edit_text_fix_selection(edit);
	edit->insertion_point_index = static_cast<word>(strlen(edit->buffer));
	edit->selection_start_index = NONE;
}

void edit_text_handle_key(edit_text* edit, const s_key_state* key)
{
	ASSERT(valid_edit_text(edit));

	// $IMPLEMENT

	edit_text_fix_selection(edit);

	e_key_type key_type = (e_key_type)key->key_type;
	e_input_key_code ascii_code = (e_input_key_code)key->ascii_code;
	if (key_type == _key_type_down && ascii_code == _key_backspace)
	{
		edit->buffer[edit->insertion_point_index - 1] = 0;
	}
}