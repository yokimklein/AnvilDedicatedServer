#include "config.h"
#include <cseries\cseries.h>
#include <format>
#include <game\game.h>

std::map<std::string, std::string> g_anvil_configuration =
{
    { "server_address", "127.0.0.1" },                              // The IPV4 address of this dedicated server instance
    { "playlist_id", "playlist_team_slayer_small" },                // The playlist this dedicated server instance is running
    { "endpoints_dispatcher_domain", "eds.svc.anvilstation.net" },  // The domain or IPV4 address for the Endpoints Dispatcher Service
    { "endpoints_dispatcher_port", "11705" },                       // The port for the Endpoints Dispatcher Service
    { "private_service_domain", "prv.svc.anvilstation.net" },       // The domain or IPV4 address for the Private Service
    { "private_service_port", "11001" },                            // The port for the Private Service
    { "account", "account" },                                       // The username for the API login
    { "sign-in-code", "sign-in-code" },                             // The password for the API login
    { "service-request-timeout-interval", "15000" },                // Time before a request is failed due to a timeout (15 seconds default)
    { "backend-token-refresh-interval", "120000" },                 // API token refresh interval (2 minutes default)
    { "service-request-refresh-interval", "5000" },                 // The time before a failed request is requested again (5 seconds defualt)
};

void anvil_load_configuration()
{
    if (!game_is_dedicated_server())
    {
        return;
    }

    std::ifstream file("server.cfg");
    std::string str;

    // create file if it doesn't exist
    if (!file.is_open())
    {
        std::ofstream out("server.cfg");
        for (const auto& [key, value] : g_anvil_configuration)
        {
            out << std::format("{} = {}\n", key, value);
        }
        out.close();
        return;
    }

    while (std::getline(file, str))
    {
        ulong pos = str.find(' ');
        if (str.length() <= pos + 3 && str.substr(pos, 3) != " = ")
        {
            continue;
        }
        std::string key = str.substr(0, pos);
        std::string value = str.substr(pos + 3);
        
        if (g_anvil_configuration.find(key) != g_anvil_configuration.end())
        {
            g_anvil_configuration[key] = value;
        }
    }
}