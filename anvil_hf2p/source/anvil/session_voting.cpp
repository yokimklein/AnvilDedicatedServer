#include "session_voting.h"
#include "scenario\scenario_map_variant.h"
#include "game\game_engine_variant.h"
#include "game\player_configuration.h"
#include "game\game_engine_simulation.h"
#include <stdlib.h>
#include "networking\session\network_session.h"
#include "game\game.h"
#include "anvil\session_control.h"
#include "anvil\config.h"
#include "anvil\backend\cache.h"

e_player_vote_selection g_anvil_vote_selections[k_maximum_multiplayer_players]{};

// TEMP FUNCTION: TODO blam _random_range
long rand_range(long min, long max)
{
    long range = max - min + 1;
    long num = rand() % range + min;
    return num;
};
void anvil_session_start_voting(c_network_session* session)
{
    s_cached_playlist& playlist = g_backend_data_cache.m_playlists[g_anvil_configuration["playlist_id"]];
    c_network_session_parameters* parameters = session->get_session_parameters();

    // reset player vote options
    csmemset(g_anvil_vote_selections, _player_vote_none, sizeof(g_anvil_vote_selections));
    
    // pick 2 random vote options from the playlist
    s_network_session_parameter_lobby_vote_set lobby_vote_set = {};
    std::vector<std::string> map_pool = playlist.maps;
    std::vector<std::string> gamemode_pool = playlist.gamemodes;
    for (long i = 0; i < NUMBEROF(s_network_session_parameter_lobby_vote_set::vote_options); i++)
    {
        ulong map_entry_index = rand_range(0, map_pool.size() - 1);
        ASSERT(VALID_INDEX(map_entry_index, map_pool.size()));
        std::string map_name = map_pool[map_entry_index];
        s_cached_map_info& map_info = g_backend_data_cache.m_map_infos[map_name];

        ulong gamemode_entry_index = rand_range(0, gamemode_pool.size() - 1);
        ASSERT(VALID_INDEX(gamemode_entry_index, gamemode_pool.size()));
        std::string gamemode_name = gamemode_pool[gamemode_entry_index];
        s_cached_gamemode& gamemode = g_backend_data_cache.m_gamemodes[gamemode_name];

        lobby_vote_set.vote_options[i].map = static_cast<byte>(map_info.title_instance_id);
        lobby_vote_set.vote_options[i].gamemode = static_cast<byte>(gamemode.title_instance_id);

        // remove chosen options from pool so they can't be selected again unless there is only a single entry remaining
        // this prevents running out of options which causes a crash in rand_range
        if (map_pool.size() > 1)
        {
            map_pool.erase(map_pool.begin() + map_entry_index);
        }
        if (gamemode_pool.size() > 1)
        {
            gamemode_pool.erase(gamemode_pool.begin() + gamemode_entry_index);
        }
    }
    parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);

    e_dedicated_server_session_state session_state = _dedicated_server_session_state_voting;
    parameters->m_parameters.dedicated_server_session_state.set(&session_state);
    parameters->m_parameters.countdown_timer.set(_network_game_countdown_delayed_reason_voting, 10);
}

void anvil_session_update_voting(c_network_session* session)
{
    const c_network_session_membership* membership = session->get_session_membership();
    c_network_session_parameters* parameters = session->get_session_parameters();

    const e_dedicated_server_session_state* dedicated_server_session_state = parameters->m_parameters.dedicated_server_session_state.get();
    if (!dedicated_server_session_state || !game_is_dedicated_server())
    {
        return;
    }

    if (*dedicated_server_session_state == _dedicated_server_session_state_voting)
    {
        // if voting has ended
        if (parameters->m_parameters.countdown_timer.get_countdown_timer() <= 0)
        {
            // loop through each player and gather their vote selections
            char option_votes[2] = {};
            for (long i = membership->get_first_player(); i != NONE; i = membership->get_next_player(i))
            {
                // if vote selection is valid
                if (g_anvil_vote_selections[i] > _player_vote_none && g_anvil_vote_selections[i] < k_player_vote_selection_count)
                {
                    option_votes[g_anvil_vote_selections[i] - 1]++;
                }
            }
            char winning_index = (option_votes[0] > option_votes[1] ? 0 : 1);
            printf("option %d wins the vote!\n", winning_index);
            s_network_session_parameter_lobby_vote_set lobby_vote_set;
            parameters->m_parameters.lobby_vote_set.get(&lobby_vote_set);
            lobby_vote_set.winning_vote_index = winning_index;
            parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);

            // load winning map & mode
            long map_title_instance_id = lobby_vote_set.vote_options[winning_index].map;
            e_map_id map_id = _map_id_none;
            for (auto iterator = g_backend_data_cache.m_map_infos.begin(); iterator != g_backend_data_cache.m_map_infos.end(); iterator++)
            {
                if (iterator->second.title_instance_id == map_title_instance_id)
                {
                    map_id = iterator->second.map_id;
                    break;
                }
            }
            char gamemode_title_instance_id = lobby_vote_set.vote_options[winning_index].gamemode;
            e_game_engine_type engine_index = k_game_engine_type_default;
            long variant_index = 0;
            long time_limit = 0;
            for (auto iterator = g_backend_data_cache.m_gamemodes.begin(); iterator != g_backend_data_cache.m_gamemodes.end(); iterator++)
            {
                if (iterator->second.title_instance_id == gamemode_title_instance_id)
                {
                    engine_index = (e_game_engine_type)iterator->second.gamemode_id;
                    ASSERT(VALID_INDEX(engine_index, k_game_engine_type_count));
                    variant_index = iterator->second.variant_id;
                    time_limit = iterator->second.time_limit;
                    break;
                }
            }

            anvil_session_set_gamemode(session, engine_index, variant_index, time_limit);
            anvil_session_set_map(map_id);

            e_dedicated_server_session_state session_state = _dedicated_server_session_state_game_start_countdown;
            parameters->m_parameters.dedicated_server_session_state.set(&session_state);

            return;
        }

        // if voting is active
        // update votes
        for (long i = membership->get_first_player(); i != NONE; i = membership->get_next_player(i))
        {
            const s_network_session_player* player = membership->get_player(i);
            e_player_vote_selection vote_selection_index = player->configuration.client.vote_selection_index;

            // if vote hasn't changed or is invalid
            if (!VALID_INDEX(vote_selection_index, k_player_vote_selection_count) || vote_selection_index == g_anvil_vote_selections[i])
            {
                continue;
            }

            s_network_session_parameter_lobby_vote_set lobby_vote_set;
            parameters->m_parameters.lobby_vote_set.get(&lobby_vote_set);

            // remove old vote if its valid
            if (g_anvil_vote_selections[i] != _player_vote_none && VALID_INDEX(g_anvil_vote_selections[i], k_player_vote_selection_count))
            {
                lobby_vote_set.vote_options[g_anvil_vote_selections[i] - 1].number_of_votes--;
            }

            // update vote selection
            g_anvil_vote_selections[i] = vote_selection_index;

            // add new vote
            lobby_vote_set.vote_options[vote_selection_index - 1].number_of_votes++;

            // update vote set parameter
            parameters->m_parameters.lobby_vote_set.set(&lobby_vote_set);
        }
    }
    // if dedi state is ready to start and the map has finished loading, start the countdown
    else if (*dedicated_server_session_state == _dedicated_server_session_state_game_start_countdown &&
        parameters->m_parameters.game_start_status.get()->game_start_status == _session_game_start_status_ready_leader &&
        parameters->m_parameters.game_start_status.get()->map_load_progress == 100)
    {
        parameters->m_parameters.countdown_timer.set(_network_game_countdown_delayed_reason_start, 5);
    }
    // dedi state is set to in game by c_life_cycle_state_handler_in_game::enter, and reset to waiting for players in c_life_cycle_state_handler_in_game::exit

    // when enough players are found JOINING SESSION
    // once connected, WAITING FOR PLAYERS until all lobby players have connected
    // VOTING ENDS IN for 10 seconds during vote phase
    // GAME STARTS 5 second countdown timer
    // wait 30 seconds once loaded back to mainmenu after game with 'PREPARE FOR BATTLE' text, kick all players from session back to matchmake
    // then return to MATCHMAKING QUEUE until enough players are found

    // TODO: what displays the 'PREPARE FOR BATTLE' text?
}