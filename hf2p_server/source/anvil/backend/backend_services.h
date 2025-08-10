#pragma once
#include <boost\beast\core.hpp>
#include <boost\beast\http.hpp>
#include <boost\beast\version.hpp>
#include <boost\asio\connect.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\json.hpp>
#include <functional>
#include <cseries\cseries.h>

// $TODO: https

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using tcp = net::ip::tcp;

// 5 second cooldown between failed requests
constexpr ulong SERVICE_REQUEST_REFRESH_INTERVAL = 5000;

// Fail async task if nothing is heard back within 15 seconds
constexpr ulong SERVICE_REQUEST_TIMEOUT_INTERVAL = 15000;

// refresh every 2 minutes
constexpr ulong BACKEND_TOKEN_REFRESH_INTERVAL = 120000;

enum e_request_status
{
    _request_status_none,
    _request_status_waiting,
    _request_status_received,
    _request_status_failed,
    _request_status_timeout,

    k_request_status_count
};

enum e_backend_return_codes
{
    _backend_missing_params = -2,
    _backend_unhandled_error = -1,
    _backend_success = 0,

    k_backend_return_codes_count
};

struct s_request_info
{
    s_request_info();

    // if send_request returns true, a request was sent
    void update_request(std::function<bool()> send_request, std::function<void()> received_response = NULL);

    std::atomic<e_request_status> status;
    ulong failure_time;
};

struct s_backend_response
{
    e_backend_return_codes retCode = _backend_unhandled_error;
    json::object data;
};

struct s_backend_request
{
    virtual std::string to_json() = 0;
};

struct s_request_get_authorization_endpoints_and_date : s_backend_request
{
    // $TODO: Anvil's API doesn't actually read the request body

    struct s_versions
    {
        std::string ServiceName;
        int Version;
        int MinorVersion;
    };
    
    std::string provider;
    std::vector<s_versions> versions;
    
    std::string to_json() override;
};

struct s_authorization_endpoint
{
    std::string Name;
    std::string IP;
    long Port;
    long Protocol;
    bool IsDefault;
};

struct s_response_get_authorization_endpoints_and_date
{
    std::vector<s_authorization_endpoint> Endpoints;
    qword DateTime;
};

struct s_request_register_game_server : s_backend_request
{
    std::string secureAddr;

    std::string to_json() override;
};
struct s_register_game_server_response
{
    std::string lobbyId;
};

struct s_request_update_game_server : s_backend_request
{
    std::string secureAddr;
    std::string serverAddr;
    long serverPort;
    std::string playlistId;

    std::string to_json() override;
};

struct s_request_retrieve_lobby_members : s_backend_request
{
    std::string lobbyId;

    std::string to_json() override;
};

enum e_resolved_endpoints
{
    _endpoint_eds,
    _endpoint_authorization,
    _endpoint_private,

    k_resolved_endpoints_count
};

struct s_resolved_endpoint
{
    s_resolved_endpoint();

    bool resolved;
    std::string host;
    std::string port;
    tcp::resolver::results_type resolver_results;
};

struct s_backend_request_data
{
    s_backend_request_data(net::io_context& ioc);

    beast::tcp_stream stream;
    http::request<http::string_body> request;
    http::response<http::string_body> response;
    beast::flat_buffer buffer;
    std::function<void(s_backend_response*)> response_handler;
    std::string endpoint; // only exists so we can print the request endpoint, retrieving from target results in a corrupted string
    const s_resolved_endpoint* resolved;
};

class c_backend_services : public std::enable_shared_from_this<c_backend_services>
{
public:
    ~c_backend_services();

    static bool initialised();
    static bool ready();
    static void initialise();

    static void update();

    static void request_get_authorization_endpoints_and_date();
    static void request_register_game_server(s_request_register_game_server& request_body);
    static void request_unregister_game_server(s_request_register_game_server& request_body);
    static void request_update_game_server(s_request_update_game_server& request_body);
    static void request_retrieve_lobby_members(s_request_retrieve_lobby_members& request_body);

private:
    c_backend_services();

    void resolve(e_resolved_endpoints endpoint_type);
    void on_resolve(e_resolved_endpoints endpoint_type, beast::error_code ec, tcp::resolver::results_type results);
    
    void on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint);
    void on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);
    void on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);
    void make_request(s_backend_request& request_body, http::verb http_verb, std::string_view endpoint, std::function<void(s_backend_response*)> response_handler, e_resolved_endpoints endpoint_type, bool use_auth_token = true);

    void handle_response_get_authorization_endpoints_and_date(s_backend_response* response);
    void handle_response_register_game_server(s_backend_response* response);
    void handle_response_unregister_game_server(s_backend_response* response);
    void handle_response_update_game_server(s_backend_response* response);
    void handle_response_retrieve_lobby_members(s_backend_response* response);

    bool m_started;
    net::io_context m_ioc;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work_guard;
    std::thread m_thread;
    tcp::resolver m_resolver;

    s_resolved_endpoint m_endpoints[k_resolved_endpoints_count];
    s_request_info m_eds_request_info;
    bool m_authorization_endpoint_valid;

    ulong m_last_token_refresh;
    std::string m_authorisation_token;
    std::string m_diagnostics_token;
};
