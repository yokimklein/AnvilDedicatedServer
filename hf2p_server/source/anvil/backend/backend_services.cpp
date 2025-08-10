#include "backend_services.h"
#include <networking\transport\transport_security.h>
#include <anvil\build_version.h>
#include <anvil\server_tools.h>
#include <anvil\backend\lobby.h>
#include <anvil\backend\user.h>
#include <anvil\config.h>
#include <combaseapi.h>
#include <iostream>
#include <networking\network_time.h>

s_request_info::s_request_info()
    : status(_request_status_none)
    , failure_time(NONE)
{
}

std::shared_ptr<c_backend_services> g_backend_services;

s_backend_request_data::s_backend_request_data(net::io_context& ioc)
    : stream(ioc)
    , request()
    , response()
    , buffer()
    , endpoint()
    , resolved()
{
};

s_resolved_endpoint::s_resolved_endpoint()
    : resolved(false)
    , host()
    , port()
    , resolver_results()
{
};

c_backend_services::c_backend_services()
    : m_started(false)
    , m_ioc()
    // keeps ioc alive when there's no immediate async work
    , m_work_guard(boost::asio::make_work_guard(m_ioc))
    , m_resolver(m_ioc)
    , m_endpoints()
    , m_eds_request_info()
    , m_authorization_endpoint_valid(false)
    , m_last_token_refresh(NONE)
    , m_authorisation_token()
    , m_diagnostics_token()
{
    m_endpoints[_endpoint_eds].host = g_anvil_configuration["endpoints_dispatcher_domain"];
    m_endpoints[_endpoint_eds].port = g_anvil_configuration["endpoints_dispatcher_port"];
    m_endpoints[_endpoint_private].host = g_anvil_configuration["private_service_domain"];
    m_endpoints[_endpoint_private].port = g_anvil_configuration["private_service_port"];

    m_thread = std::thread([this]()
    {
        m_ioc.run();
    });
}

c_backend_services::~c_backend_services()
{
    // allow run() to exit once all work is done
    m_work_guard.reset();
    m_ioc.stop();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool c_backend_services::initialised()
{
    return g_backend_services != NULL;
};

bool c_backend_services::ready()
{
    return g_backend_services->m_endpoints[_endpoint_eds].resolved
        && g_backend_services->m_endpoints[_endpoint_authorization].resolved
        && g_backend_services->m_endpoints[_endpoint_private].resolved;
};

void c_backend_services::initialise()
{
    if (g_backend_services)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": backend already initialised!\n");
        return;
    }

    g_backend_services = std::shared_ptr<c_backend_services>(new c_backend_services());
    g_backend_services.get()->resolve(_endpoint_eds);
    g_backend_services.get()->resolve(_endpoint_private);
    g_backend_services->m_started = true;
}

void s_request_info::update_request(std::function<bool()> send_request, std::function<void()> received_response)
{
    switch (status)
    {
        case _request_status_none:
        {
            // if a request was sent, set the status to waiting
            if (send_request && send_request())
            {
                status = _request_status_waiting;
            }
            break;
        }
        case _request_status_received:
        {
            // allow overriding of received response
            if (received_response)
            {
                received_response();
            }
            else
            {
                status = _request_status_none;
            }
            break;
        }
        // on failure, wait 5 seconds before requesting again
        case _request_status_failed:
        {
            failure_time = network_time_get();
            status = _request_status_timeout;
            break;
        }
        case _request_status_timeout:
        {
            if (network_time_since(failure_time) >= SERVICE_REQUEST_REFRESH_INTERVAL)
            {
                failure_time = NONE;
                status = _request_status_none;
            }
            break;
        }
        case _request_status_waiting:
        default:
        {
            return;
        }
    }
}

void c_backend_services::update()
{
    // $TODO: Move this to an anvil_backend_initialize() function called from the game executable
    if (!c_backend_services::initialised())
    {
        c_backend_services::initialise();
    }

    // $TODO: handle this for when we're not running in dedicated server mode
    // These requests will only fire as the pragma tells them to, and if they fail they will try again every 5 seconds

    // Services below require a connection to the private service
    if (g_backend_services->m_endpoints[_endpoint_private].resolved)
    {
        g_lobby_session_data.update_request([]
        {
            // Request send is handled in network_join, this is just to update the timeout
            return false;
        },
        []
        {
            // empty to allow network join to handle received itself
        });
    }

    // Services below require a connection to the endpoint service
    if (!g_backend_services->m_endpoints[_endpoint_eds].resolved)
    {
        return;
    }

    // retrieve authorisation endpoint from API
    g_backend_services->m_eds_request_info.update_request([]
    {
        // Request EDS if we haven't yet
        if (g_backend_services->m_authorization_endpoint_valid)
        {
            return false;
        }
        c_backend_services::request_get_authorization_endpoints_and_date();
        return true;
    });

    // Services below require a connection to the authorization service
    if (!g_backend_services->m_endpoints[_endpoint_authorization].resolved)
    {
        if (g_backend_services->m_authorization_endpoint_valid)
        {
            // Resolve auth if it hasn't already been resolved
            g_backend_services->resolve(_endpoint_authorization);
        }
        return;
    }

    // Register game server with API once we're connected to the auth service
    g_lobby_info.update_request([]
    {
        // Don't request if we already have valid lobby info
        if (g_lobby_info.valid)
        {
            return false;
        }

        s_request_register_game_server register_request;
        s_transport_secure_address server_identifier;

        anvil_get_server_identifier(&server_identifier);
        register_request.secureAddr = transport_secure_address_get_string(&server_identifier);

        c_backend_services::request_register_game_server(register_request);
        return true;
    });
}

void c_backend_services::resolve(e_resolved_endpoints endpoint_type)
{
    s_resolved_endpoint& endpoint = g_backend_services.get()->m_endpoints[endpoint_type];
    g_backend_services.get()->m_resolver.async_resolve
    (
        endpoint.host.c_str(),
        endpoint.port.c_str(),
        beast::bind_front_handler
        (
            &c_backend_services::on_resolve,
            shared_from_this(),
            endpoint_type
        )
    );
}

void c_backend_services::on_resolve(e_resolved_endpoints endpoint_type, beast::error_code ec, tcp::resolver::results_type results)
{
    // $TODO: handle resolution timeouts?
    s_resolved_endpoint& endpoint = m_endpoints[endpoint_type];
    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to resolve host %s:%s! [error: %d - %s]\n", endpoint.host.c_str(), endpoint.port.c_str(), ec.value(), ec.message().c_str());
        return;
    }
    endpoint.resolver_results = results;
    endpoint.resolved = true;
    printf("ONLINE/HTTP,STUB_LOG_FILTER: " __FUNCTION__ ": successfully resolved host %s:%s\n", endpoint.host.c_str(), endpoint.port.c_str());
}

void c_backend_services::on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint)
{
    backend_data.get()->stream.expires_never();

    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to connect to host %s:%s! [error: %d - %s]\n",
            backend_data.get()->resolved->host.c_str(),
            backend_data.get()->resolved->port.c_str(),
            ec.value(),
            ec.message().c_str());
        
        s_backend_response failure_response;
        failure_response.retCode = (e_backend_return_codes)ec.value();
        backend_data.get()->response_handler(&failure_response);

        return;
    }

    backend_data.get()->stream.expires_after(std::chrono::milliseconds(SERVICE_REQUEST_TIMEOUT_INTERVAL));
    http::async_write(backend_data.get()->stream, backend_data.get()->request, beast::bind_front_handler(&c_backend_services::on_write, shared_from_this(), backend_data));
}

void c_backend_services::on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
{
    backend_data.get()->stream.expires_never();

    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to write! [error: %d - %s]\n", ec.value(), ec.message().c_str());

        s_backend_response failure_response;
        failure_response.retCode = (e_backend_return_codes)ec.value();
        backend_data.get()->response_handler(&failure_response);

        return;
    }

    backend_data.get()->stream.expires_after(std::chrono::milliseconds(SERVICE_REQUEST_TIMEOUT_INTERVAL));
    http::async_read(backend_data.get()->stream, backend_data.get()->buffer, backend_data.get()->response, beast::bind_front_handler(&c_backend_services::on_read, shared_from_this(), backend_data));
}

void c_backend_services::on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
{
    backend_data.get()->stream.expires_never();

    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to read response! [error: %d - %s]\n", ec.value(), ec.message().c_str());

        s_backend_response failure_response;
        failure_response.retCode = (e_backend_return_codes)ec.value();
        backend_data.get()->response_handler(&failure_response);

        return;
    }

    // parse json
    s_backend_response response;
    const auto& response_body = backend_data.get()->response.body();
    try
    {
        json::value jv = json::parse(response_body);
    
        if (jv.is_object())
        {
            const auto& obj = jv.as_object();
            if (obj.contains("retCode"))
            {
                response.retCode = (e_backend_return_codes)obj.at("retCode").as_int64();
                if (response.retCode != _backend_success)
                {
                    printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": response returned failed retCode! [%d]!\n", response.retCode);
                    s_backend_response failure_response;
                    failure_response.retCode = response.retCode;
                    backend_data.get()->response_handler(&failure_response);
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
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": failed to parse JSON! [%s]\n%s\n", e.what(), response_body.c_str());
        s_backend_response failure_response;
        failure_response.retCode = _backend_unhandled_error;
        backend_data.get()->response_handler(&failure_response);
    }

    // close connection
    backend_data.get()->stream.socket().shutdown(tcp::socket::shutdown_both, ec);

    if (ec && ec != beast::errc::not_connected)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to shutdown connection! [error: %d - %s]!\n", ec.value(), ec.message().c_str());
    }
}

void c_backend_services::make_request(s_backend_request& request_body, http::verb http_verb, std::string_view endpoint, std::function<void(s_backend_response*)> response_handler, e_resolved_endpoints endpoint_type, bool use_auth_token)
{
    s_resolved_endpoint& resolved_endpoint = m_endpoints[endpoint_type];
    if (!resolved_endpoint.resolved)
    {
        return;
    }

    std::shared_ptr<s_backend_request_data> backend_data = std::make_shared<s_backend_request_data>(m_ioc);
    http::request<http::string_body>& request = backend_data.get()->request;

    backend_data.get()->endpoint = endpoint;
    backend_data.get()->resolved = &resolved_endpoint;
    request.target(backend_data.get()->endpoint);
    request.method(http_verb);
    request.version(11);
    request.set(http::field::host, resolved_endpoint.host + ":" + resolved_endpoint.port);
    request.set(http::field::user_agent, anvil_get_build_name_string());
    request.set(http::field::content_type, "application/json");
    if (use_auth_token)
    {
        request.set("USER_CONTEXT", m_authorisation_token);
    }
    request.body() = request_body.to_json();
    request.prepare_payload();
    backend_data.get()->response_handler = response_handler;

    printf("ONLINE/CLIENT/REQUEST,JSON: " __FUNCTION__ ": sending request to %s:%s%s\n", resolved_endpoint.host.c_str(), resolved_endpoint.port.c_str(), backend_data.get()->endpoint.c_str());

    backend_data.get()->stream.expires_after(std::chrono::milliseconds(SERVICE_REQUEST_TIMEOUT_INTERVAL));
    backend_data.get()->stream.async_connect(resolved_endpoint.resolver_results, beast::bind_front_handler(&c_backend_services::on_connect, shared_from_this(), backend_data));
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_request_get_authorization_endpoints_and_date::s_versions const& version)
{
    boost::json::object out;
    out["ServiceName"] = version.ServiceName;
    out["Version"] = version.Version;
    out["MinorVersion"] = version.MinorVersion;
    json_value = std::move(out);
}

s_authorization_endpoint tag_invoke(boost::json::value_to_tag<s_authorization_endpoint>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_authorization_endpoint endpoint
    {
        obj.at("Name").as_string().c_str(),
        obj.at("IP").as_string().c_str(),
        static_cast<int>(obj.at("Port").as_int64()),
        static_cast<int>(obj.at("Protocol").as_int64()),
        obj.at("IsDefault").as_bool()
    };
    return endpoint;
}

std::string s_request_get_authorization_endpoints_and_date::to_json()
{
    boost::json::object out;
    out["provider"] = provider;
    out["versions"] = boost::json::value_from(versions);
    return boost::json::serialize(out);
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

void c_backend_services::request_get_authorization_endpoints_and_date()
{
    if (!g_backend_services->m_endpoints[_endpoint_eds].resolved)
    {
        return;
    }

    s_request_get_authorization_endpoints_and_date request_body;
    request_body.provider = "";
    s_request_get_authorization_endpoints_and_date::s_versions versions[3];
    versions[0].ServiceName = "AuthorizationService";
    versions[0].Version = 4;
    versions[0].MinorVersion = 14;
    versions[1].ServiceName = "d7e31cf042024b22ac09d2a42c0403cb"; // UserStorageService
    versions[1].Version = 2;
    versions[1].MinorVersion = 0;
    versions[2].ServiceName = "PrivateService";
    versions[2].Version = 1;
    versions[2].MinorVersion = 0;
    for (long i = 0; i < NUMBEROF(versions); i++)
    {
        request_body.versions.push_back(versions[i]);
    }

    g_backend_services->make_request
    (
        request_body,
        http::verb::post,
        "/EndpointsDispatcherService.svc/GetAuthorizationEndpointsAndDate",
        std::bind(&c_backend_services::handle_response_get_authorization_endpoints_and_date, g_backend_services, std::placeholders::_1),
        _endpoint_eds,
        false
    );
}

void c_backend_services::handle_response_get_authorization_endpoints_and_date(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    if (!response->data.contains("Endpoints"))
    {
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": response returned no endpoints array!\n");
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    auto endpoints = boost::json::value_to<std::vector<s_authorization_endpoint>>(response->data.at("Endpoints"));
    long endpoints_count = endpoints.size();

    if (endpoints_count == 0)
    {
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received 0 endpoints!\n");
        m_eds_request_info.status = _request_status_failed;
        return;
    }

    // Use first default endpoint, or use the first endpoint
    s_authorization_endpoint& authorisation_endpoint = endpoints[0];
    for (auto& endpoint : endpoints)
    {
        if (endpoint.IsDefault)
        {
            authorisation_endpoint = endpoint;
            break;
        }
    }
    g_backend_services->m_endpoints[_endpoint_authorization].host = authorisation_endpoint.IP;
    g_backend_services->m_endpoints[_endpoint_authorization].port = std::format("{}", authorisation_endpoint.Port);
    m_authorization_endpoint_valid = true;
    m_eds_request_info.status = _request_status_received;

    printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received %d endpoints, using [%s]\n", endpoints_count, authorisation_endpoint.Name.c_str());

    // don't currently need this
    //if (response->data.contains("DateTime"))
    //{
    //    qword date_time = response->data.at("DateTime").as_uint64();
    //}
}

void c_backend_services::request_register_game_server(s_request_register_game_server& request_body)
{
    if (!g_backend_services->m_endpoints[_endpoint_private].resolved)
    {
        return;
    }

    // remove old lobby ID and set status to waiting
    g_lobby_info.clear_lobby_identifier();

    g_backend_services->make_request
    (
        request_body,
        http::verb::put,
        "/PrivateService.svc/RegisterGameServer",
        std::bind(&c_backend_services::handle_response_register_game_server, g_backend_services, std::placeholders::_1),
        _endpoint_private,
        false
    );
}

void c_backend_services::handle_response_register_game_server(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        g_lobby_info.status = _request_status_failed;
        return;
    }

    if (response->data.contains("lobbyId"))
    {
        std::string lobby_id_string = response->data.at("lobbyId").as_string().c_str();
        std::wstring lobby_id_braced = std::format(L"{{{}}}", std::wstring(lobby_id_string.begin(), lobby_id_string.end()));
        HRESULT result = CLSIDFromString(lobby_id_braced.c_str(), (LPCLSID)&g_lobby_info.lobby_identifier);
        ASSERT(result == S_OK);
        
        g_lobby_info.status = _request_status_received;
        g_lobby_info.valid = true;
        printf("ONLINE/CLIENT/RESPONSE,JSON: " __FUNCTION__ ": received lobby identifier [%s]\n", transport_secure_identifier_get_string(&g_lobby_info.lobby_identifier));
    }
}

void c_backend_services::request_unregister_game_server(s_request_register_game_server& request_body)
{
    if (!g_backend_services->m_endpoints[_endpoint_private].resolved)
    {
        return;
    }

    // clear lobby info
    g_lobby_info.clear_lobby_identifier();
    g_lobby_info.status = _request_status_none;

    g_backend_services->make_request
    (
        request_body,
        http::verb::delete_,
        "/PrivateService.svc/UnregisterGameServer",
        std::bind(&c_backend_services::handle_response_unregister_game_server, g_backend_services, std::placeholders::_1),
        _endpoint_private,
        false
    );
}

void c_backend_services::handle_response_unregister_game_server(s_backend_response* response)
{

}

void c_backend_services::request_update_game_server(s_request_update_game_server& request_body)
{
    if (!g_backend_services->m_endpoints[_endpoint_private].resolved)
    {
        return;
    }

    g_backend_services->make_request
    (
        request_body,
        http::verb::post,
        "/PrivateService.svc/UpdateGameServer",
        std::bind(&c_backend_services::handle_response_update_game_server, g_backend_services, std::placeholders::_1),
        _endpoint_private,
        false
    );
}

void c_backend_services::handle_response_update_game_server(s_backend_response* response)
{

}

void c_backend_services::request_retrieve_lobby_members(s_request_retrieve_lobby_members& request_body)
{
    if (!g_backend_services->m_endpoints[_endpoint_private].resolved)
    {
        return;
    }
    g_lobby_session_data.status = _request_status_waiting;

    g_backend_services->make_request
    (
        request_body,
        http::verb::post,
        "/PrivateService.svc/RetrieveLobbyMembers",
        std::bind(&c_backend_services::handle_response_retrieve_lobby_members, g_backend_services, std::placeholders::_1),
        _endpoint_private,
        false
    );
}

void c_backend_services::handle_response_retrieve_lobby_members(s_backend_response* response)
{
    if (response->retCode != _backend_success)
    {
        g_lobby_session_data.status = _request_status_failed;
        g_lobby_session_data.reset_user_data();
        return;
    }

    // $TODO: json fails to parse sometimes?
    auto members = response->data.at("members").as_array();
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