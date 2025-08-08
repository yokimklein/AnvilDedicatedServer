#include "private_service.h"
#include <networking\transport\transport_security.h>
#include <anvil\build_version.h>
#include <anvil\server_tools.h>
#include <anvil\backend\lobby.h>
#include <anvil\backend\user.h>
#include <combaseapi.h>
#include <iostream>

std::shared_ptr<c_backend_private_service> g_backend_private_service;

c_backend_private_service::c_backend_private_service(std::string_view host, std::string_view port)
    : m_initialised(false)
    , m_ioc()
    , m_work_guard(boost::asio::make_work_guard(m_ioc)) // keeps ioc alive when there's no immediate async work
    , m_resolver(m_ioc)
    , m_host(host)
    , m_port(port)
{
    m_thread = std::thread([this]()
    {
        m_ioc.run();
    });
}

c_backend_private_service::~c_backend_private_service()
{
    // allow run() to exit once all work is done
    m_work_guard.reset();
    m_ioc.stop();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void c_backend_private_service::initialise()
{
    if (g_backend_private_service)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": backend already initialised!\n");
        return;
    }

    // $TODO: pull endpoint from config file?
    g_backend_private_service = std::shared_ptr<c_backend_private_service>(new c_backend_private_service("127.0.0.1", "11001"));
    g_backend_private_service.get()->start_resolve();
}

void c_backend_private_service::start_resolve()
{
    if (m_initialised)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": already initialised - cannot start resolve again!\n");
        return;
    }
    g_backend_private_service.get()->m_resolver.async_resolve(m_host.data(), m_port.data(), beast::bind_front_handler(&c_backend_private_service::on_resolve, shared_from_this()));
}

void c_backend_private_service::on_resolve(beast::error_code ec, tcp::resolver::results_type results)
{
    if (ec)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to resolve host %s:%s [error: %d - %s]!\n", m_host.data(), m_port.data(), ec.value(), ec.message().c_str());
        return;
    }
    m_resolver_results = results;
    m_initialised = true;
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": successfully resolved host %s:%s\n", m_host.data(), m_port.data());
}

void c_backend_private_service::on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint)
{
    if (ec)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to connect to host %s:%s [error: %d - %s]!\n", m_host.data(), m_port.data(), ec.value(), ec.message().c_str());
        return;
    }
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": sending request to %s:%s%s\n", m_host.data(), m_port.data(), backend_data.get()->endpoint);
    
    http::async_write(backend_data.get()->stream, backend_data.get()->request, beast::bind_front_handler(&c_backend_private_service::on_write, shared_from_this(), backend_data));
}

void c_backend_private_service::on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
{
    if (ec)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to write! [error: %d - %s]!\n", ec.value(), ec.message().c_str());
        return;
    }

    http::async_read(backend_data.get()->stream, backend_data.get()->buffer, backend_data.get()->response, beast::bind_front_handler(&c_backend_private_service::on_read, shared_from_this(), backend_data));
}

void c_backend_private_service::on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
{
    if (ec)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to read response! [error: %d - %s]!\n", ec.value(), ec.message().c_str());
        return;
    }

    // parse json
    s_backend_response response;
    try
    {
        json::value jv = json::parse(backend_data.get()->response.body());
    
        if (jv.is_object())
        {
            const auto& obj = jv.as_object();
            if (obj.contains("retCode"))
            {
                response.retCode = (e_backend_return_codes)obj.at("retCode").as_int64();
                if (response.retCode != _backend_success)
                {
                    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": response returned failed retCode! [%d]!\n", response.retCode);
                }
                else
                {
                    if (obj.contains("data"))
                    {
                        response.data = obj.at("data").as_object();
                        backend_data.get()->response_handler(&response);
                    }
                }
            }
        }
    }
    catch (const std::exception& e)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to parse JSON! [%s]!\n", e.what());
    }

    // close connection
    backend_data.get()->stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    if (ec && ec != beast::errc::not_connected)
    {
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": failed to shutdown connection! [error: %d - %s]!\n", ec.value(), ec.message().c_str());
    }
}

void c_backend_private_service::make_request(s_backend_request& request_body, http::verb http_verb, const char* endpoint, std::function<void(s_backend_response*)> response_handler)
{
    if (!m_initialised)
    {
        return;
    }

    std::shared_ptr<s_backend_request_data> backend_data = std::make_shared<s_backend_request_data>(m_ioc);
    http::request<http::string_body>& request = backend_data.get()->request;

    backend_data.get()->endpoint = endpoint;
    request.target(backend_data.get()->endpoint);
    request.method(http_verb);
    request.version(11);
    request.set(http::field::host, m_host.data()/* + ":" + m_port*/);
    request.set(http::field::user_agent, anvil_get_build_name_string());
    request.set(http::field::content_type, "application/json");
    request.body() = request_body.to_json();
    request.prepare_payload();
    backend_data.get()->response_handler = response_handler;
    backend_data.get()->stream.async_connect(m_resolver_results, beast::bind_front_handler(&c_backend_private_service::on_connect, shared_from_this(), backend_data));
}

std::string s_request_register_game_server::to_json()
{
    boost::json::object out;
    out["secureAddr"] = secureAddr;
    return boost::json::serialize(out);
}

std::string s_request_update_game_server::to_json()
{
    boost::json::object out;
    out["secureAddr"] = secureAddr;
    out["serverAddr"] = serverAddr;
    out["serverPort"] = serverPort;
    out["playlistId"] = playlistId;
    return boost::json::serialize(out);
}

std::string s_request_retrieve_lobby_members::to_json()
{
    boost::json::object out;
    out["lobbyId"] = lobbyId;
    return boost::json::serialize(out);
}

void c_backend_private_service::request_register_game_server(s_request_register_game_server& request_body)
{
    if (!m_initialised)
    {
        return;
    }

    // remove old lobby ID and set status to waiting
    g_lobby_info.clear_lobby_identifier();
    g_lobby_info.status = _request_status_waiting;

    make_request(request_body, http::verb::put, "/PrivateService.svc/RegisterGameServer", handle_response_register_game_server);
}

void handle_response_register_game_server(s_backend_response* response)
{
    if (response->data.contains("lobbyId"))
    {
        std::string lobby_id_string = response->data.at("lobbyId").as_string().c_str();
        std::wstring lobby_id_braced = std::format(L"{{{}}}", std::wstring(lobby_id_string.begin(), lobby_id_string.end()));
        HRESULT result = CLSIDFromString(lobby_id_braced.c_str(), (LPCLSID)&g_lobby_info.lobby_identifier);
        ASSERT(result == S_OK);
        
        g_lobby_info.valid = true;
        g_lobby_info.status = _request_status_received;
        printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": received lobby identifier [%s]\n", transport_secure_identifier_get_string(&g_lobby_info.lobby_identifier));
    }
}

void c_backend_private_service::request_unregister_game_server(s_request_register_game_server& request_body)
{
    if (!m_initialised)
    {
        return;
    }

    // clear lobby info
    g_lobby_info.clear_lobby_identifier();
    g_lobby_info.status = _request_status_none;

    make_request(request_body, http::verb::delete_, "/PrivateService.svc/UnregisterGameServer", handle_response_unregister_game_server);
}

void handle_response_unregister_game_server(s_backend_response* response)
{

}

void c_backend_private_service::request_update_game_server(s_request_update_game_server& request_body)
{
    if (!m_initialised)
    {
        return;
    }

    make_request(request_body, http::verb::post, "/PrivateService.svc/UpdateGameServer", handle_response_update_game_server);
}

void handle_response_update_game_server(s_backend_response* response)
{

}

void c_backend_private_service::request_retrieve_lobby_members(s_request_retrieve_lobby_members& request_body)
{
    if (!m_initialised)
    {
        return;
    }
    g_lobby_session_data.status = _request_status_waiting;

    make_request(request_body, http::verb::post, "/PrivateService.svc/RetrieveLobbyMembers", handle_response_retrieve_lobby_members);
}

void handle_response_retrieve_lobby_members(s_backend_response* response)
{
    g_lobby_session_data.reset_user_data();

    auto members = response->data.at("members").as_array();
    long user_sessions_count = members.size();

    // ensure the API hasn't returned more players than we support
    ASSERT(user_sessions_count <= k_network_maximum_players_per_session);
    if (user_sessions_count > k_network_maximum_players_per_session)
    {
        g_lobby_session_data.status = _request_status_none;
        return;
    }
    
    printf("MP/NET/STUB_LOG_PATH,STUB_LOG_FILTER: " __FUNCTION__ ": received [%d] user sessions\n", user_sessions_count);

    for (ulong user_session_index = 0; user_session_index < members.size(); user_session_index++)
    {
        s_user_session& user_session = g_lobby_session_data.users[user_session_index];

        user_session.valid = true;
        user_session.user_id = members[user_session_index].at("userId").as_int64();

        std::string session_id_string = members[user_session_index].at("sessionId").as_string().c_str();
        std::wstring session_id_braced = std::format(L"{{{}}}", std::wstring(session_id_string.begin(), session_id_string.end()));

        HRESULT result = CLSIDFromString(session_id_braced.c_str(), (LPCLSID)&user_session.session_id);
        ASSERT(result == S_OK);
    }

    g_lobby_session_data.status = _request_status_received;
}