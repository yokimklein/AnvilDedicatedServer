#include "private_service.h"
#include <anvil\backend\lobby.h>
#include <combaseapi.h>
#include <anvil\backend\user.h>

void c_backend::private_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::private_service::endpoint()
{
    return *m_endpoint;
}

std::string c_backend::private_service::register_game_server::s_request::to_json()
{
    boost::json::object out;
    out["secureAddr"] = secureAddr;
    return boost::json::serialize(out);
}

void c_backend::private_service::register_game_server::request(std::string secureAddr)
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    // remove old lobby ID and set status to waiting
    g_lobby_info.clear_lobby_identifier();

    s_request request_body;
    request_body.secureAddr = secureAddr;

    g_backend_services->make_request
    (
        request_body,
        http::verb::put,
        "/PrivateService.svc/RegisterGameServer",
        &response,
        *m_endpoint,
        false
    );
};

void c_backend::private_service::register_game_server::response(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        g_lobby_info.status = _request_status_failed;
        return;
    }

    const auto& data = response->data.as_object();
    if (data.contains("lobbyId"))
    {
        std::string lobby_id_string = data.at("lobbyId").as_string().c_str();
        std::wstring lobby_id_braced = std::format(L"{{{}}}", std::wstring(lobby_id_string.begin(), lobby_id_string.end()));
        HRESULT result = CLSIDFromString(lobby_id_braced.c_str(), (LPCLSID)&g_lobby_info.lobby_identifier);
        ASSERT(result == S_OK);

        g_lobby_info.status = _request_status_received;
        g_lobby_info.valid = true;
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received lobby identifier [%s]\n", transport_secure_identifier_get_string(&g_lobby_info.lobby_identifier));
    }
}

std::string c_backend::private_service::unregister_game_server::s_request::to_json()
{
    boost::json::object out;
    out["secureAddr"] = secureAddr;
    return boost::json::serialize(out);
}

void c_backend::private_service::unregister_game_server::request(std::string secureAddr)
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    // clear lobby info
    g_lobby_info.clear_lobby_identifier();
    g_lobby_info.status = _request_status_none;

    s_request request_body;
    request_body.secureAddr = secureAddr;

    g_backend_services->make_request
    (
        request_body,
        http::verb::delete_,
        "/PrivateService.svc/UnregisterGameServer",
        &response,
        *m_endpoint,
        false
    );
};

void c_backend::private_service::unregister_game_server::response(s_backend_response* response)
{

}

std::string c_backend::private_service::update_game_server::s_request::to_json()
{
    boost::json::object out;
    out["secureAddr"] = secureAddr;
    out["serverAddr"] = serverAddr;
    out["serverPort"] = serverPort;
    out["playlistId"] = playlistId;
    return boost::json::serialize(out);
}

void c_backend::private_service::update_game_server::request(std::string secureAddr, std::string serverAddr, long serverPort, std::string playlistId)
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }

    s_request request_body;
    request_body.secureAddr = secureAddr;
    request_body.serverAddr = serverAddr;
    request_body.serverPort = serverPort;
    request_body.playlistId = playlistId;

    g_backend_services->make_request
    (
        request_body,
        http::verb::post,
        "/PrivateService.svc/UpdateGameServer",
        &response,
        *m_endpoint,
        false
    );
}

void c_backend::private_service::update_game_server::response(s_backend_response* response)
{
}

std::string c_backend::private_service::retrieve_lobby_members::s_request::to_json()
{
    boost::json::object out;
    out["lobbyId"] = lobbyId;
    return boost::json::serialize(out);
}

void c_backend::private_service::retrieve_lobby_members::request(std::string lobbyId)
{
    if (!m_endpoint->m_resolved)
    {
        return;
    }
    g_lobby_session_data.status = _request_status_waiting;

    s_request request_body;
    request_body.lobbyId = lobbyId;

    g_backend_services->make_request
    (
        request_body,
        http::verb::post,
        "/PrivateService.svc/RetrieveLobbyMembers",
        &response,
        *m_endpoint,
        false
    );
}

void c_backend::private_service::retrieve_lobby_members::response(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        g_lobby_session_data.status = _request_status_failed;
        g_lobby_session_data.reset_user_data();
        return;
    }

    // $TODO: json fails to parse sometimes?
    const auto& data = response->data.as_object();
    auto members = data.at("members").as_array();
    ulong user_sessions_count = members.size();

    // ensure the API hasn't returned more players than we support
    if (!VALID_INDEX(user_sessions_count, k_network_maximum_players_per_session))
    {
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received too many user sessions! [%d out of max %d]\n", user_sessions_count, k_network_maximum_players_per_session);
        g_lobby_session_data.status = _request_status_none;
        g_lobby_session_data.reset_user_data();
        return;
    }

    printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received [%d] user sessions\n", user_sessions_count);

    s_user_session users[k_network_maximum_players_per_session];
    csmemset(users, 0, sizeof(users));
    for (ulong user_session_index = 0; user_session_index < members.size(); user_session_index++)
    {
        s_user_session& user_session = users[user_session_index];

        user_session.valid = true;
        user_session.user_id = members[user_session_index].at("userId").as_int64();

        std::string session_id_string = members[user_session_index].at("sessionId").as_string().c_str();
        std::wstring session_id_braced = std::format(L"{{{}}}", std::wstring(session_id_string.begin(), session_id_string.end()));

        HRESULT result = CLSIDFromString(session_id_braced.c_str(), (LPCLSID)&user_session.session_id);
        ASSERT(result == S_OK);
    }

    g_lobby_session_data.valid = true;
    csmemcpy(g_lobby_session_data.users, users, sizeof(g_lobby_session_data.users));
    g_lobby_session_data.status = _request_status_received;
}
