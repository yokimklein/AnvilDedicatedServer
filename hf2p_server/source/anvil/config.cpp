#include "config.h"
#include <cseries\cseries.h>
#include <format>

std::map<std::string, std::string> g_anvil_configuration =
{
    { "server_address", "127.0.0.1" },
    { "playlist_id", "playlist_team_slayer_small" },
    { "endpoints_dispatcher_domain", "eds.svc.anvilstation.net" },
    { "endpoints_dispatcher_port", "11705" },
    { "private_service_domain", "prv.svc.anvilstation.net" },
    { "private_service_port", "11001" },
};

void anvil_load_configuration()
{
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