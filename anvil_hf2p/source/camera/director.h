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

enum e_controller_index;
enum e_output_user_index;
class c_director
{
public:
	virtual e_director_mode get_type() const;
	virtual void update(real dt);
	virtual bool should_draw_hud();
	virtual bool should_draw_hud_saved_film();
	virtual bool inhibits_facing();
	virtual bool inhibits_input();
	virtual void handle_deleted_player(long);
	virtual void handle_deleted_object(long);
	virtual bool can_use_camera_mode(e_camera_mode);
	//virtual void select_fallback_target(); // c_observer_director, c_saved_film_director

	//bool in_free_camera_mode() const;
	c_camera* get_camera();
	c_camera const* get_camera() const;
	//s_observer_command const* get_last_observer_command() const;
	//e_director_perspective get_perspective() const;
	bool set_camera_mode_internal(e_camera_mode camera_mode, real transition_time, bool force_update);
	bool set_camera_mode(e_camera_mode camera_mode, real transition_time);
	void player_switch_gui_create_or_update();

protected:
	byte m_camera_storage[0x4C];
	s_observer_command m_last_observer_command;
	real m_change_camera_pause;
	long m_user_index;
	long m_watched_player_index;
	bool m_player_switch_gui_activated;
	byte pad[0x3];
};
static_assert(sizeof(c_director) == 0x14C);

struct s_director_info
{
	e_director_mode mode;
	e_director_perspective perspective;
	e_camera_mode camera_mode;
};
static_assert(sizeof(s_director_info) == 0xC);

struct s_director_globals
{
	c_static_array<byte[0x160], 4> directors;
	c_static_array<s_director_info, 4> director_info;
	real dtime;
	real fade_timer;
	real fade_maximum;
	bool debug_force_scripted_camera_disable;
};
static_assert(sizeof(s_director_globals) == 0x5C0);

struct s_observer_gamestate_globals
{
	long active_structure_bsp_index_mask;
	s_cluster_reference cluster_references[4];
};
static_assert(sizeof(s_observer_gamestate_globals) == 0xC);

void survival_mode_update_flying_camera(long user_index);
void control_toggling_of_debug_directors(long user_index);
s_director_globals* director_globals_get();
c_director* director_get(long user_index);
void __fastcall director_set_mode(long user_index, e_director_mode director_mode);
e_director_mode choose_appropriate_director(long user_index);