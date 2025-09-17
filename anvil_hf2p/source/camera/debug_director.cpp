#include "debug_director.h"
#include "input\input_windows.h"
#include "main\console.h"
#include "camera\camera_globals.h"
#include "editor\editor_stubs.h"
#include "camera\flying_camera.h"
#include <cmath>

e_camera_mode debug_camera_modes[] = { _camera_mode_flying, _camera_mode_following };

void c_debug_director::update_(real dt)
{
	if (input_key_frames_down(_key_backspace, _input_type_ui) == 1)
	{
		cycle_camera();
	}

	mouse_state* state = input_get_mouse_state(_input_type_ui);
	if (debug_camera_modes[m_current_camera_mode_index] == _camera_mode_flying && state)
	{
		if (state->frames_down[_mouse_button_middle_click] == 1)
		{
			m_flying_camera_biped_control = !m_flying_camera_biped_control;
			console_printf(m_flying_camera_biped_control ? "Biped control mode" : "Flying camera control mode");
		}

		if (g_editor_director_mouse_wheel_speed_enabled && !state->frames_down[_mouse_button_middle_click])
		{
			//g_director_camera_speed_scale = int_pin(powf(1.3f, (real32)state->wheel_ticks) * g_director_camera_speed_scale, 0.0009765625f, 256.0f);

			real wheel_value = real(short(state->wheel_delta) / input_globals.mouse_wheel_delta);
			g_director_camera_speed_scale = int_pin(powf(1.3f, wheel_value) * g_director_camera_speed_scale, 0.0009765625f, 256.0f);

			//real32 wheel_value = real32(int16(state->wheel_delta) / input_globals.mouse_wheel_delta);
			//if (wheel_value < 0.0f)
			//	g_director_camera_speed_scale = int_pin(g_director_camera_speed_scale / 1.03f, 0.0009765625f, 256.0f);
			//if (wheel_value > 0.0f)
			//	g_director_camera_speed_scale = int_pin(g_director_camera_speed_scale * 1.03f, 0.0009765625f, 256.0f);
		}

		if (m_flying_camera_biped_control || !editor_input_inhibited())
		{
			m_input_inhibited = false;
			set_camera_mode(_camera_mode_static, 0.0f);
		}
		else
		{
			m_input_inhibited = true;
			set_camera_mode(_camera_mode_flying, 0.0f);
			static_cast<c_flying_camera*>(get_camera())->set_collision(false);
		}
	}

	INVOKE_CLASS_MEMBER(0xE3110, c_director, update, dt);
}

void c_debug_director::changed_camera()
{
	set_camera_mode(debug_camera_modes[m_current_camera_mode_index], 0.0f);
	console_printf("%s camera", global_camera_mode_names[debug_camera_modes[m_current_camera_mode_index]]);
	c_camera* camera = get_camera();
	if (camera->get_type() == _camera_mode_flying)
	{
		m_input_inhibited = true;
		static_cast<c_flying_camera*>(get_camera())->set_collision(false);
	}
	else
	{
		m_input_inhibited = false;
	}
}

void c_debug_director::cycle_camera()
{
	m_current_camera_mode_index = (m_current_camera_mode_index - 1) % NUMBEROF(debug_camera_modes);
	changed_camera();
}

bool c_debug_director::finished_cycle()
{
	return m_current_camera_mode_index == NUMBEROF(debug_camera_modes) - 1;
}