#pragma once
#include <cseries\cseries.h>
#include <functional>
#include <atomic>

// 5 second cooldown between failed requests
constexpr ulong SERVICE_REQUEST_REFRESH_INTERVAL = 5000;

enum e_request_status
{
    _request_status_none,
    _request_status_waiting,
    _request_status_received,
    _request_status_failed,
    _request_status_timeout,

    k_request_status_count
};

struct s_request_info
{
    s_request_info();

    // if send_request returns true, a request was sent
    void update_request(std::function<bool()> send_request = NULL, std::function<void()> received_response = NULL);

    std::atomic<e_request_status> status;
    ulong failure_time;
};
