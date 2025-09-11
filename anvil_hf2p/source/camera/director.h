#pragma once
#include <cseries\cseries.h>
#include <camera\camera.h>
//#include <camera\dead_camera.h>
//#include <camera\first_person_camera.h>
//#include <camera\flying_camera.h>
//#include <camera\following_camera.h>
#include <camera\observer.h>
//#include <camera\orbiting_camera.h>
//#include <camera\static_camera.h>
#include <memory\data.h>

enum e_director_mode
{
	_director_mode_game = 0,
	_director_mode_saved_film,
	_director_mode_observer,
	_director_mode_debug,
	_director_mode_unused,
	_director_mode_editor,

	k_number_of_director_modes
};

enum e_director_perspective
{
	_director_perspective_first_person = 0,
	_director_perspective_third_person,
	_director_perspective_scripted,

	// c_null_camera, (c_authored_camera default)
	_director_perspective_3,

	k_number_of_director_perspectives,
};

struct s_director_info
{
	e_director_mode director_mode;
	e_director_perspective director_perspective;
	e_camera_mode camera_mode;
};
static_assert(sizeof(s_director_info) == 0xC);

enum e_controller_index;
enum e_output_user_index;
class c_director
{
	virtual e_director_mode get_type() const;
	virtual void update(real);
	virtual bool should_draw_hud();
	virtual bool should_draw_hud_saved_film();
	virtual bool inhibits_facing();
	virtual bool inhibits_input();
	virtual void handle_deleted_player(long);
	virtual void handle_deleted_object(long);
	virtual bool can_use_camera_mode(e_camera_mode);
	//virtual void select_fallback_target(); // c_observer_director, c_saved_film_director

	//bool in_free_camera_mode() const;
	//c_camera* get_camera();
	//c_camera const* get_camera() const;
	//s_observer_command const* get_last_observer_command() const;
	//e_director_perspective get_perspective() const;
	//bool set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update);
	//bool set_camera_mode(e_camera_mode camera_mode, real transition_time);

	byte m_camera[0x4C];

	s_observer_command m_observer_command;
	real m_transition_time;
	e_output_user_index m_output_user_index;
	long m_user_index;
	bool __unknown148;

	byte pad[3];
};
static_assert(sizeof(c_director) == 0x14C);