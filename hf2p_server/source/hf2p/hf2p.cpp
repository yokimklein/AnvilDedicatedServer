#include "hf2p.h"
#include <memory\tls.h>

void hf2p_player_podium_initialize(long podium_biped_index, long player_index)
{
	s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
	s_player_podium* player_podium = &g_player_podiums[*g_player_podiums_count];
	*g_player_podiums_count = *g_player_podiums_count + 1;
	player_podium->current_action = _podium_action_base;
	player_podium->ticks = get_tls()->game_time_globals->elapsed_ticks;
	player_podium->player_index = player_index;
	player_podium->player_model_index = player_data->configuration.host.player_appearance.player_model_choice;
	player_podium->female = player_data->configuration.host.player_appearance.flags & 1;
	player_podium->loop_count = 0;
	player_podium->object_index = -1;
	player_podium->object_index2 = -1;
	player_podium->object_index3 = -1;
	player_podium->stance_index = 0;
	player_podium->move_index = 1;
}