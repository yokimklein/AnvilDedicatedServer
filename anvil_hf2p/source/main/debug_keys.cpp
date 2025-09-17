#include "debug_keys.h"
#include "input\input_windows.h"
#include "game\game.h"
#include "main\console.h"
#include "editor\editor_stubs.h"
#include "main\main_time.h"
#include "anvil\session_voting.h"
#include "networking\session\network_session.h"
#include "networking\logic\network_life_cycle.h"
#include "networking\session\network_session_parameters.h"
#include "anvil\session_control.h"

// Modifier Table
// 
// Index, Shift,  Control, Alt
// 0,     No,     No,      No
// 1,     Yes,    No,      No
// 2,     No,     Yes,     No
// 3,     Ignore, No,      Yes
// 4,     Yes,    Yes,     Yes
// 5,     No,     Yes,     Yes

debug_key global_debug_key_list[]
{
	{
		.name = "display framerate",
		.key_code = _key_f,
		.modifier = 2,
		.function = NULL,
		.allow_out_of_game = false,
		.allow_in_editor = false,
		.toggle_variable = true,
		.variable = base_address<bool*>(0x103E7C4)
	},
	{
		.name = "anvil session begin vote",
		.key_code = _key_page_down,
		.modifier = 0,
		.function = debug_key_begin_vote,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "anvil session launch",
		.key_code = _key_home,
		.modifier = 0,
		.function = debug_key_session_launch,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "anvil session disconnect",
		.key_code = _key_page_up,
		.modifier = 0,
		.function = debug_key_session_disconnect,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "anvil session end",
		.key_code = _key_end,
		.modifier = 0,
		.function = debug_key_session_end,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
	{
		.name = "anvil session test mode",
		.key_code = _key_insert,
		.modifier = 0,
		.function = debug_key_session_test_mode,
		.allow_out_of_game = true,
		.allow_in_editor = false,
		.toggle_variable = false,
		.variable = NULL
	},
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

void __cdecl debug_key_begin_vote(bool key_is_down)
{
	if (!key_is_down)
	{
		return;
	}

	console_printf("starting session vote...");
	anvil_session_begin_vote();
}

void __cdecl debug_key_session_launch(bool key_is_down)
{
	if (!key_is_down)
	{
		return;
	}

	console_printf("launching session...");

	c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
	if (!session)
	{
		return;
	}

	c_network_session_parameters* parameters = session->get_session_parameters();
	if (!parameters)
	{
		return;
	}

	parameters->m_parameters.session_mode.set(_network_session_mode_setup);
}

void __cdecl debug_key_session_disconnect(bool key_is_down)
{
	if (!key_is_down)
	{
		return;
	}

	console_printf("disconnecting session...");

	c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
	if (!session)
	{
		return;
	}

	session->disconnect();
}

void __cdecl debug_key_session_end(bool key_is_down)
{
	if (!key_is_down)
	{
		return;
	}

	console_printf("ending session game...");

	c_network_session* session = life_cycle_globals.state_manager.get_active_squad_session();
	if (!session)
	{
		return;
	}

	c_network_session_parameters* parameters = session->get_session_parameters();
	if (!parameters)
	{
		return;
	}

	parameters->m_parameters.session_mode.set(_network_session_mode_end_game);
}

void __cdecl debug_key_session_test_mode(bool key_is_down)
{
	if (!key_is_down)
	{
		return;
	}

	console_printf("setting session test mode...");

	anvil_session_set_gamemode(_game_engine_type_slayer, 0, 0);
	anvil_session_set_map(_riverworld);
}