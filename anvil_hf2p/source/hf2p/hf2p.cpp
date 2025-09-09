#include "hf2p.h"
#include <game\game.h>

void hf2p_initialize()
{
    if (game_is_client() || game_is_dedicated_server() || game_is_bot_client())
    {
        game_startup_main();

        if (game_is_client())
        {
            game_startup_client();
        }
        if (game_is_dedicated_server())
        {
            game_startup_dedicated_server();
        }
        if (game_is_bot_client())
        {
            game_startup_bot_client();
        }
    }
    else if (game_is_sapien())
    {
        game_startup_sapien();
    }

    // TODO: logging here, hook original function and return to this section?
}

void game_startup_main()
{
    INVOKE(0x2B0280, game_startup_main);
}

void game_startup_client()
{
    INVOKE(0x2B0560, game_startup_client);
}

void game_startup_dedicated_server()
{
    // Statistics are for challenges - likely to track progress for clients
    //game_statistics_reset();
    //game_statistics_initialize();
    
    // User data for requesting loadouts
    //user_data_initialize();
    
    // Clans for service tags?
    //clan_services_initialize();
    
    // User data cache
    //backend_user_base_data_initialize();
    
    // Clan data cache
    //backend_clan_base_data_initialize();
    
    // Requests session info from API
    //backend_session_basic_data_initialize();

    // Why is this called again?
    //backend_user_base_data_initialize();
}

void game_startup_bot_client()
{
    //    game_statistics_reset();
    //    game_statistics_initialize();
    //    messaging_initialize();
    //    user_data_initialize();
    //    clan_services_initialize();
    //    backend_user_base_data_initialize();
    //    backend_clan_base_data_initialize();
    //    user_states_initialize();
    //    presence_initialize();
    //    matchmaking_initialize();
    //    game_session_client_initialize();
    //    shop_initialize();
    //    heartbeat_initialize();
}

void game_startup_sapien()
{

}

bool __fastcall hf2p_session_invalid(s_transport_secure_identifier* session_id)
{
    return INVOKE(0x557EC0, hf2p_session_invalid, session_id);
}
