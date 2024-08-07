#pragma once
#include <cseries\cseries.h>
#include <networking\session\network_session_parameters_chunked.h>
#include <game\game_options.h>
#include <shell\shell.h>
#include <text\unicode.h>

struct s_saved_film_description
{
	long film_category;
	long campaign_id;
	long map_id;
	short campaign_insertion_point;
	bool campaign_survival_enabled;
	byte : 8;
	long difficulty;
	c_static_wchar_string<256> film_path;
	c_static_wchar_string<128> film_name;
	c_enum<e_controller_index, long, _controller_index0, k_number_of_controllers> controller_index;
	long length_seconds;
};
static_assert(sizeof(s_saved_film_description) == 0x31C);

struct s_network_session_parameter_saved_film_game_options
{
	long length_in_ticks;
	long start_ticks;
	game_options game_options;
};
static_assert(sizeof(s_network_session_parameter_saved_film_game_options) == 0x1A050);

class c_network_session_parameter_saved_film_game_options : public c_network_session_parameter_chunked<s_network_session_parameter_saved_film_game_options, 0x8800>
{
};
static_assert(sizeof(c_network_session_parameter_saved_film_game_options) == 0x45140);