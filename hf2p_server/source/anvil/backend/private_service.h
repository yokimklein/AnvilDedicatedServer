#pragma once
#include <boost\beast\core.hpp>
#include <boost\beast\http.hpp>
#include <boost\beast\version.hpp>
#include <boost\asio\connect.hpp>
#include <boost\asio\ip\tcp.hpp>
#include <boost\json.hpp>
#include <functional>

namespace net = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
namespace json = boost::json;
using tcp = net::ip::tcp;

enum e_request_status
{
    _request_status_none,
    _request_status_waiting,
    _request_status_received,

    k_request_status_count
};

enum e_backend_return_codes
{
    _backend_missing_params = -2,
    _backend_unhandled_error = -1,
    _backend_success = 0,

    k_backend_return_codes_count
};

struct s_backend_response
{
    e_backend_return_codes retCode = _backend_unhandled_error;
    json::object data;
};

struct s_register_game_server_response
{
    const char* lobbyId;
};
void handle_register_game_server_response(s_backend_response* response);

struct s_register_game_server_request
{
    const char* secureAddr;

    std::string to_json();
};

struct s_backend_request_data
{
    s_backend_request_data(net::io_context& ioc)
        : stream(ioc)
        , request()
        , response()
        , buffer()
    {
    };

    beast::tcp_stream stream;
    http::request<http::string_body> request;
    http::response<http::string_body> response;
    beast::flat_buffer buffer;
    std::function<void(s_backend_response*)> response_handler;
};

class c_backend_private_service : public std::enable_shared_from_this<c_backend_private_service>
{
public:
    ~c_backend_private_service();

    static void initialise();
    void start_resolve();

    bool initialised() const { return m_initialised; };

    void request_register_game_server(s_register_game_server_request& request_body);

private:
    c_backend_private_service(std::string_view host, std::string_view port);

    void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
    void on_connect(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, tcp::resolver::results_type::endpoint_type endpoint);
    void on_write(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);
    void on_read(std::shared_ptr<s_backend_request_data> backend_data, beast::error_code ec, std::size_t size);

    bool m_initialised;
    net::io_context m_ioc;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work_guard;
    std::thread m_thread;

    tcp::resolver m_resolver;
    std::string_view m_host;
    std::string_view m_port;
    tcp::resolver::results_type m_resolver_results;

    //beast::tcp_stream m_stream; // $TODO: Create one of these per request

    //http::request<http::string_body> m_request;
    //http::response<http::string_body> m_response;
    //beast::flat_buffer m_buffer;
};
extern std::shared_ptr<c_backend_private_service> g_backend_private_service;
