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

// Fail async task if nothing is heard back within 15 seconds
constexpr ulong SERVICE_REQUEST_TIMEOUT_INTERVAL = 15000;

// refresh every 2 minutes
constexpr ulong BACKEND_TOKEN_REFRESH_INTERVAL = 120000;

enum e_backend_return_codes
{
    _backend_missing_params = -2,
    _backend_unhandled_error = -1,
    _backend_success = 0,

    k_backend_return_codes_count
};

enum e_resolved_endpoints
{
    _endpoint_eds,
    _endpoint_authorization,
    _endpoint_private,
    _endpoint_title_server,

    k_resolved_endpoints_count
};

struct s_backend_response
{
    e_backend_return_codes retCode = _backend_unhandled_error;
    json::value data;
};

struct s_backend_request
{
    virtual std::string to_json() = 0;
};

struct s_backend_request_data;
class c_backend : public std::enable_shared_from_this<c_backend>
{
public:
    class resolved_endpoint
    {
    public:
        resolved_endpoint();

        void resolve(e_resolved_endpoints type, std::string host, std::string port);
        
        bool m_resolved;
        std::string m_host;
        std::string m_port;
        tcp::resolver::results_type m_resolver_results;
    };

    class private_service;
    class endpoints_service;
    class authorization_service;
    class user_storage_service;
    class title_resource_service;

    ~c_backend();

    static bool initialised();
    static bool ready();
    static void initialise();

    static void update();

    void resolve(e_resolved_endpoints endpoint_type, std::string host, std::string port);
    void resolve(e_resolved_endpoints endpoint_type);

private:
    c_backend();

    void on_resolve(e_resolved_endpoints endpoint_type, beast::error_code ec, tcp::resolver::results_type results);
    void on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint);
    void on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);
    void on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);
    static void make_request(s_backend_request& request_body, http::verb http_verb, std::string_view endpoint, std::function<void(s_backend_response*)> response_handler, resolved_endpoint& resolved_endpoint, bool use_auth_token = true);

    net::io_context m_ioc;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work_guard;
    std::thread m_thread;
    tcp::resolver m_resolver;

    // $TODO: find a better solution
    // Ideally I would store these as statics in the services, but I've been running into memory related crashes when I do this
    // seems to be an issue with the DLL memory mismatching with the EXE
    resolved_endpoint m_endpoint_storage[k_resolved_endpoints_count];
};
extern std::shared_ptr<c_backend> g_backend_services;

// $TODO: disconnect method, resets states and attempts again after timeout
// disconnect after multiple consecutive failures?

struct s_backend_request_data
{
    s_backend_request_data(net::io_context& ioc);

    beast::tcp_stream stream;
    http::request<http::string_body> request;
    http::response<http::string_body> response;
    beast::flat_buffer buffer;
    std::function<void(s_backend_response*)> response_handler;
    std::string endpoint; // only exists so we can print the request endpoint, retrieving from target results in a corrupted string
    const c_backend::resolved_endpoint* resolved;
};

struct s_endpoint_response
{
    std::string Name;
    std::string IP;
    long Port;
    long Protocol;
    bool IsDefault;
};
s_endpoint_response tag_invoke(boost::json::value_to_tag<s_endpoint_response>, boost::json::value const& jv);

// $TODO: Anvil's API ignores this struct
struct s_versions
{
    std::string ServiceName;
    int Version;
    int MinorVersion;
};
void tag_invoke(boost::json::value_from_tag, boost::json::value& json_value, s_versions const& version);
