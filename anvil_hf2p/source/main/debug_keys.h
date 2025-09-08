#pragma once

struct debug_key
{
	const char* name;
	//c_enum<e_input_key_code, int16, _key_escape, NUMBER_OF_KEYS> key_code;
	short key_code;
	short modifier;
	void(__cdecl* function)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* variable;
};
static_assert(sizeof(debug_key) == 0x14);

struct s_debug_button
{
	const char* name;
	//c_enum<e_controller_button, int16, _controller_button_left_trigger, k_controller_button_count> gamepad_abstract_button;
	short gamepad_abstract_button;
	void(__cdecl* function)(bool);
	bool allow_out_of_game;
	bool allow_in_editor;
	bool toggle_variable;
	bool* variable;
};
static_assert(sizeof(s_debug_button) == 0x14);

void debug_keys_initialize();
void debug_keys_dispose();
bool debug_key_update(long key_index, debug_key* key, bool* modifier_down, long force_key_down);
void debug_keys_update();

extern debug_key global_debug_key_list[];
extern dword g_debug_button_down_flags;
extern s_debug_button g_debug_button_list[];