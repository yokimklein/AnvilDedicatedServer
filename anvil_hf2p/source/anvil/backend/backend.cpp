#include "backend.h"
#include <config\version.h>
#include <iostream>
#include <networking\network_time.h>
#include <anvil\config.h>
#include <anvil\backend\services\private_service.h>
#include <anvil\backend\services\endpoints_service.h>
#include <anvil\backend\services\authorization_service.h>
#include <anvil\backend\services\user_storage_service.h>
#include <anvil\backend\services\title_resource_service.h>
#include <anvil\backend\cache.h>

std::shared_ptr<c_backend> g_backend_services;

s_backend_request_data::s_backend_request_data(net::io_context& ioc)
    : stream(ioc)
    , request()
    , response()
    , buffer()
    , endpoint()
    , resolved()
{
};

c_backend::resolved_endpoint::resolved_endpoint()
    : m_resolved(false)
    , m_host()
    , m_port()
    , m_resolver_results()
{
};

void c_backend::resolved_endpoint::resolve(e_resolved_endpoints type, std::string host, std::string port)
{
    if (m_resolved)
    {
        return;
    }

    m_host = host;
    m_port = port;

    g_backend_services.get()->resolve(type);
}

void c_backend::resolve(e_resolved_endpoints endpoint_type, std::string host, std::string port)
{
    resolved_endpoint& endpoint = m_endpoint_storage[endpoint_type];
    endpoint.m_host = host;
    endpoint.m_port = port;
    resolve(endpoint_type);
}

void c_backend::resolve(e_resolved_endpoints endpoint_type)
{
    resolved_endpoint& endpoint = m_endpoint_storage[endpoint_type];

    g_backend_services.get()->m_resolver.async_resolve
    (
        endpoint.m_host.c_str(),
        endpoint.m_port.c_str(),
        beast::bind_front_handler
        (
            &c_backend::on_resolve,
            shared_from_this(),
            endpoint_type
        )
    );
}

void c_backend::on_resolve(e_resolved_endpoints endpoint_type, beast::error_code ec, tcp::resolver::results_type results)
{
    resolved_endpoint& endpoint = m_endpoint_storage[endpoint_type];

    // $TODO: handle resolution timeouts?
    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to resolve host %s:%s! [error: %d - %s]\n", endpoint.m_host.c_str(), endpoint.m_port.c_str(), ec.value(), ec.message().c_str());
        return;
    }
    endpoint.m_resolver_results = results;
    endpoint.m_resolved = true;
    printf("ONLINE/HTTP,STUB_LOG_FILTER: " __FUNCTION__ ": successfully resolved host %s:%s\n", endpoint.m_host.c_str(), endpoint.m_port.c_str());
}

c_backend::c_backend()
    : m_ioc()
    // keeps ioc alive when there's no immediate async work
    , m_work_guard(boost::asio::make_work_guard(m_ioc))
    , m_resolver(m_ioc)
    , m_endpoint_storage()
{
    m_thread = std::thread([this]()
    {
        m_ioc.run();
    });
}

c_backend::~c_backend()
{
    // allow run() to exit once all work is done
    m_work_guard.reset();
    m_ioc.stop();
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

bool c_backend::initialised()
{
    return g_backend_services != NULL;
};

bool c_backend::ready()
{
    // ready once all service endpoints are resolved
    bool ready = true;
    for (long endpoint_type = 0; endpoint_type < k_resolved_endpoints_count; endpoint_type++)
    {
        ready &= g_backend_services->m_endpoint_storage[endpoint_type].m_resolved;
    }
    return ready;
};

void c_backend::initialise()
{
    if (g_backend_services)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": backend already initialised!\n");
        return;
    }
    g_backend_services = std::shared_ptr<c_backend>(new c_backend());

    auto& endpoint_storage = g_backend_services.get()->m_endpoint_storage;
    c_backend::private_service::initialise(&endpoint_storage[_endpoint_private]);
    c_backend::endpoints_service::initialise(&endpoint_storage[_endpoint_eds]);
    c_backend::authorization_service::initialise(&endpoint_storage[_endpoint_authorization]);
    c_backend::user_storage_service::initialise(&endpoint_storage[_endpoint_title_server]);
    c_backend::title_resource_service::initialise(&endpoint_storage[_endpoint_title_server]);

    endpoints_service::endpoint().resolve(_endpoint_eds, g_anvil_configuration["endpoints_dispatcher_domain"], g_anvil_configuration["endpoints_dispatcher_port"]);
    private_service::endpoint().resolve(_endpoint_private, g_anvil_configuration["private_service_domain"], g_anvil_configuration["private_service_port"]);
}

// $TODO: split up into service update methods
void c_backend::update()
{
    // $TODO: Move this to an anvil_backend_initialize() function called from the game executable
    if (!initialised())
    {
        initialise();
    }

    // $TODO: handle this for when we're not running in dedicated server mode
    // These requests will only fire as the pragma tells them to, and if they fail they will try again every 5 seconds

    // Services below require a connection to the private service
    if (private_service::endpoint().m_resolved)
    {
        private_service::retrieve_lobby_members::m_status.update_request([]
        {
            // Request send is handled in network_join, this is just to update the timeout
            return false;
        },
        []
        {
            // Keep on received to allow network join to update this
            private_service::retrieve_lobby_members::m_status.status = _request_status_received;
        });

        private_service::unregister_game_server::m_status.update_request();
        private_service::update_game_server::m_status.update_request();
    }


    // Services below require a connection to the endpoint service
    if (!endpoints_service::endpoint().m_resolved)
    {
        return;
    }

    // retrieve authorisation endpoint from API
    endpoints_service::get_authorization_endpoints_and_date::m_status.update_request([]
    {
        // Request EDS if we haven't yet
        if (endpoints_service::m_authorization_endpoint_valid)
        {
            return false;
        }
        endpoints_service::get_authorization_endpoints_and_date::request();
        return true;
    });

    // Services below require a connection to the authorization service
    if (!authorization_service::endpoint().m_resolved)
    {
        return;
    }

    // Request enqueue if we're offline
    authorization_service::enqueue::m_status.update_request([]
    {
        if (authorization_service::m_session_state == _backend_session_offline)
        {
            authorization_service::enqueue::request();
            return true;
        }
        return false;
    },
    []
    {
        // On received, send dequeue
        authorization_service::dequeue::request();
    });
    // Update dequeue status, send is handled in enqueue response
    authorization_service::dequeue::m_status.update_request();

    // Services below require authentication
    if (authorization_service::m_session_state != _backend_session_online)
    {
        return;
    }

    // Refresh tokens every 2 minutes
    authorization_service::refresh_tokens::m_status.update_request([]
    {
        if (network_time_since(authorization_service::m_last_token_refresh) >= BACKEND_TOKEN_REFRESH_INTERVAL)
        {
            authorization_service::refresh_tokens::request();
            return true;
        }
        return false;
    });

    // Register game server with API once we're connected to the auth service
    private_service::register_game_server::m_status.update_request([]
    {
        // Don't request if we already have valid lobby info
        if (g_backend_data_cache.m_lobby_info.valid)
        {
            return false;
        }

        s_transport_secure_address server_identifier;
        anvil_get_server_identifier(&server_identifier);
        private_service::register_game_server::request(transport_secure_address_get_string(&server_identifier));
        return true;
    });

    // Request title instances if our cache is stale
    title_resource_service::get_title_configuration::m_status.update_request([]
    {
        // $TODO: re-request interval? check last update time?
        if (g_backend_data_cache.m_valid)
        {
            return false;
        }
        title_resource_service::get_title_configuration::request();
        return true;
    });
}

void c_backend::on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint)
{
    backend_data.get()->stream.expires_never();

    if (ec)
    {
        printf("ONLINE/HTTP,ERR: " __FUNCTION__ ": failed to connect to host %s:%s! [error: %d - %s]\n",
            backend_data.get()->resolved->m_host.c_str(),
            backend_data.get()->resolved->m_port.c_str(),
            ec.value(),
            ec.message().c_str());
        
        s_backend_response failure_response;
        failure_response.retCode = (e_backend_return_codes)ec.value();
        backend_data.get()->response_handler(&failure_response);

        return;
    }

    backend_data.get()->stream.expires_after(std::chrono::milliseconds(SERVICE_REQUEST_TIMEOUT_INTERVAL));
    http::async_write(backend_data.get()->stream, backend_data.get()->request, beast::bind_front_handler(&c_backend::on_write, shared_from_this(), backend_data));
}

void c_backend::on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
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
    http::async_read(backend_data.get()->stream, backend_data.get()->buffer, backend_data.get()->response, beast::bind_front_handler(&c_backend::on_read, shared_from_this(), backend_data));
}

void c_backend::on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size)
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
    response.request_identifier = backend_data.get()->request_identifier;
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
                        response.data = obj.at("data");
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

ulong c_backend::make_request(s_backend_request& request_body, http::verb http_verb, std::string_view endpoint, std::function<void(s_backend_response*)> response_handler, resolved_endpoint& resolved_endpoint, bool use_auth_token)
{
    if (!resolved_endpoint.m_resolved)
    {
        return NONE;
    }

    std::shared_ptr<s_backend_request_data> backend_data = std::make_shared<s_backend_request_data>(g_backend_services->m_ioc);
    http::request<http::string_body>& request = backend_data.get()->request;

    backend_data.get()->endpoint = endpoint;
    backend_data.get()->resolved = &resolved_endpoint;
    request.target(backend_data.get()->endpoint);
    request.method(http_verb);
    request.version(11);
    request.set(http::field::host, resolved_endpoint.m_host + ":" + resolved_endpoint.m_port);
    request.set(http::field::user_agent, VERSION_TARGET_APPLICATION);
    request.set(http::field::content_type, "application/json");
    if (use_auth_token)
    {
        request.set("USER_CONTEXT", authorization_service::m_authorisation_token);
    }
    request.body() = request_body.to_json();
    request.prepare_payload();
    backend_data.get()->response_handler = response_handler;
    backend_data.get()->request_identifier = g_backend_services->increment_request_number();

    printf("ONLINE/CLIENT/REQUEST,JSON: " __FUNCTION__ ": sending request to %s:%s%s\n", resolved_endpoint.m_host.c_str(), resolved_endpoint.m_port.c_str(), backend_data.get()->endpoint.c_str());

    backend_data.get()->stream.expires_after(std::chrono::milliseconds(SERVICE_REQUEST_TIMEOUT_INTERVAL));
    backend_data.get()->stream.async_connect(resolved_endpoint.m_resolver_results, beast::bind_front_handler(&c_backend::on_connect, g_backend_services->shared_from_this(), backend_data));

    return backend_data.get()->request_identifier;
}

ulong c_backend::increment_request_number()
{
    // ensure increment doesn't ever become NONE as we treat this as a fail
    if (m_last_request_number == NONE)
    {
        m_last_request_number = 0;
    }
    else
    {
        m_last_request_number++;
    }
    return m_last_request_number;
}

s_endpoint_response tag_invoke(boost::json::value_to_tag<s_endpoint_response>, boost::json::value const& jv)
{
    auto const& obj = jv.as_object();
    s_endpoint_response endpoint
    {
        obj.at("Name").as_string().c_str(),
        obj.at("IP").as_string().c_str(),
        static_cast<int>(obj.at("Port").as_int64()),
        static_cast<int>(obj.at("Protocol").as_int64()),
        obj.at("IsDefault").as_bool()
    };
    return endpoint;
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_versions const& version)
{
    boost::json::object out;
    out["ServiceName"] = version.ServiceName;
    out["Version"] = version.Version;
    out["MinorVersion"] = version.MinorVersion;
    json_value = std::move(out);
}