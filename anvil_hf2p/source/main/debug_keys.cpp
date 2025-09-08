#include "debug_keys.h"
#include "input\input_windows.h"
#include "game\game.h"
#include "main\console.h"
#include "editor\editor_stubs.h"

debug_key global_debug_key_list[]
{
	{
		.name = NULL,
		.key_code = _key_not_a_key,
		.modifier = 0,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	}
};

c_static_flags<ALIGN(NUMBEROF(global_debug_key_list), 4)> global_debug_key_down; // 16 bit aligned
dword g_debug_button_down_flags;

void debug_keys_initialize()
{
	debug_key* key = global_debug_key_list;
	long key_down_count = 0;
	for (key_down_count = 0; key->name; key_down_count++)
	{
		if (key->variable)
			*key->variable = false;

		key++;
	}

	//if (!global_debug_key_down)
	//{
	//	int32 key_down_size = 4 * ((key_down_count + 31) >> 5);
	//	global_debug_key_down = (int32*)malloc(key_down_size /*, __FILE__, __LINE__ */);
	//	ASSERT(global_debug_key_down);
	//	csmemset(global_debug_key_down, 0, key_down_size);
	//}

	global_debug_key_down.clear();
	g_debug_button_down_flags = 0;
}

void debug_keys_dispose()
{

}

bool debug_key_update(long key_index, debug_key* key, bool* modifier_down, long force_key_down)
{
	bool result = false;
	if ((game_in_progress() || key->allow_out_of_game) && (!game_in_editor() || key->allow_in_editor) && input_globals.mouse_acquired)
	{
		ASSERT(VALID_INDEX(key->modifier, 6)); // NUMBEROF(modifier_down) == 6
		bool key_down = input_key_frames_down(static_cast<e_input_key_code>(key->key_code), _input_type_game) && modifier_down[key->modifier];

		if (force_key_down)
		{
			key_down = force_key_down == 1;
		}

		if (!key->toggle_variable)
		{
			if (key->variable)
			{
				*key->variable = key_down;
			}
		}

		if (global_debug_key_down.test(key_index))
		{
			if (!key_down)
			{
				result = true;
				global_debug_key_down.set(key_index, false);

				if (key->toggle_variable)
				{
					if (key->variable)
					{
						console_printf("%s = %s", key->name, *key->variable ? "OFF" : "ON");
						*key->variable = !*key->variable;
					}
				}

				if (key->function)
				{
					key->function(false);
				}
			}
		}
		else if (key_down)
		{
			result = true;
			global_debug_key_down.set(key_index, true);

			if (key->function)
			{
				key->function(true);
			}
		}
	}

	return result;
}

void debug_keys_update()
{
	bool shift_down = input_key_frames_down(_key_shift, _input_type_game);
	bool control_down = input_key_frames_down(_key_control, _input_type_game) != 0;
	bool alt_down = input_key_frames_down(_key_alt, _input_type_game) != 0;

	//random_seed_allow_use();
	bool modifier_down[6]{};

	modifier_down[0] = !shift_down && !control_down && !alt_down;
	modifier_down[1] = shift_down && !control_down && !alt_down;
	modifier_down[2] = !shift_down && control_down && !alt_down;
	modifier_down[3] = !control_down && alt_down;
	modifier_down[4] = shift_down && control_down && alt_down;
	modifier_down[5] = !shift_down && control_down && alt_down;

	debug_key* key = global_debug_key_list;
	if (key->name)
	{
		for (long key_index = 0; key->name; key_index++)
		{
			debug_key_update(key_index, key++, modifier_down, 0);
		}
	}

	// $IMPLEMENT `g_debug_button_list`

	//for (s_debug_button& button : g_debug_button_list)
	//{
	//	bool button_down = ?;
	//
	//	// Extra logic
	//
	//	if (button.callback)
	//		button.callback(button_down);
	//}

	//random_seed_disallow_use();
}
