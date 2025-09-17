#include "director.h"
#include "input\input_windows.h"
#include "camera\debug_director.h"
#include "main\console.h"
#include "memory\tls.h"
#include "editor\editor_stubs.h"
#include "game\game.h"
#include "game\player_mapping.h"

c_camera* c_director::get_camera()
{
	if (!m_camera_storage[0])
	{
		return NULL;
	}

	return (c_camera*)&m_camera_storage;
}

c_camera const* c_director::get_camera() const
{
	if (!m_camera_storage[0])
	{
		return NULL;
	}

	return (c_camera*)&m_camera_storage;
}

bool c_director::set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update)
{
	// $TODO: the invoked call has force_update optimised out as it was never used
	ASSERT(force_update == false);

	auto set_camera_mode_internal_call = DECLFUNC(0xE3460, bool, __vectorcall, c_director*, e_camera_mode, real, bool);
	bool result;
	__asm
	{
		push 0 // force_update
		movss xmm2, transition_time
		push camera_mode
		mov ecx, this
		call set_camera_mode_internal_call
		mov result, al
	}
	return result;
}

bool c_director::set_camera_mode(e_camera_mode camera_mode, real transition_time)
{
	return set_camera_mode_internal(camera_mode, transition_time, false);
}

void c_director::player_switch_gui_create_or_update()
{
	// $TODO: what is this for? player switching?
}

void survival_mode_update_flying_camera(long user_index)
{
	// $TODO:
}

void control_toggling_of_debug_directors(long user_index)
{
	c_director* director = director_get(user_index);
	e_director_mode director_mode = director->get_type();

	if (director_mode == _director_mode_editor || input_key_frames_down(_key_backspace, _input_type_game) != 1)
	{
		return;
	}

	if (director_mode == _director_mode_debug)
	{
		if (static_cast<c_debug_director*>(director)->finished_cycle())
		{
			console_printf("exiting debug director");
			director_set_mode(user_index, choose_appropriate_director(user_index));
		}

	}
	else
	{
		input_suppress();
		console_printf("entering debug director");
		director_set_mode(user_index, _director_mode_debug);
	}
}

s_director_globals* director_globals_get()
{
	TLS_DATA_GET_VALUE_REFERENCE(director_globals);
	return (director_globals && director_globals->directors[0][0]) ? director_globals : NULL;
}

c_director* director_get(long user_index)
{
	if (!director_globals_get())
	{
		return NULL;
	}

	return (c_director*)&director_globals_get()->directors[user_index];
}

void __fastcall director_set_mode(long user_index, e_director_mode director_mode)
{
	INVOKE(0xE2E80, director_set_mode, user_index, director_mode);
}

e_director_mode choose_appropriate_director(long user_index)
{
	if (game_in_editor() && player_mapping_get_unit_by_output_user(user_index) == NONE)
	{
		return _director_mode_editor;
	}
	else
	{
		return game_is_playback() ? _director_mode_saved_film : _director_mode_game;
	}
}