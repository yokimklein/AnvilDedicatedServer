#pragma once
#include <camera\director.h>
#include <camera\game_director.h>
#include <camera\saved_film_director.h>
#include <camera\debug_director.h>
#include <camera\editor_director.h>

// Not an original file, but I couldn't figure out how else to fix the circular references
struct s_director_globals
{
	union c_director_storage
	{
		c_director director;
		c_game_director game_director;
		c_saved_film_director saved_film_director;
		c_observer_director observer_director;
		c_debug_director debug_director;
		c_editor_director editor_director;
		char director_data[0x160];
	}
	directors[4];

	s_director_info infos[4];
	real timestep;
	real fade_timer5B4; // in?
	real fade_timer5B8; // out?
	bool debug_force_scripted_camera_disable;
};
static_assert(sizeof(s_director_globals) == 0x5C0);