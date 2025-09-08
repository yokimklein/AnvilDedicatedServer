#pragma once

struct edit_text
{
	char* buffer;
	short maximum_length;
	short insertion_point_index;
	short selection_start_index;
};
static_assert(sizeof(edit_text) == 0xC);

struct s_key_state;
void edit_text_new(edit_text* edit);
void edit_text_fix_selection(edit_text* edit);
void edit_text_selection_reset(edit_text* edit);
void edit_text_handle_key(edit_text* edit, const s_key_state* key);