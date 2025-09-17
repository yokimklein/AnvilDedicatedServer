#include "simulation_queue_global_events.h"
#include <memory\tls.h>
#include <simulation\game_interface\simulation_game_engine_player.h>
#include <game\game_engine.h>

void __fastcall simulation_queue_player_event_apply_set_activation(datum_index player_index, bool player_active)
{
    TLS_DATA_GET_VALUE_REFERENCE(players);
    player_datum* player = (player_datum*)datum_get(players, player_index);
    if (player->flags.test(_player_active_in_game_bit) != player_active)
    {
        if (player_active)
        {
            player->flags.set(_player_active_in_game_bit, true);
            game_engine_player_activated(player_index);
        }
        else
        {
            player->flags.set(_player_active_in_game_bit, false);
        }
        simulation_action_game_engine_player_update(player_index, _simulation_player_update_active_in_game);
    }
}