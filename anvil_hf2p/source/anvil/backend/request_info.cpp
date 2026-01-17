#include "request_info.h"
#include "networking\network_time.h"
#include "anvil\backend\backend.h"

s_request_info::s_request_info()
    : status(_request_status_none)
    , failure_time(NONE)
{
}

void s_request_info::update_request(std::function<bool()> send_request, std::function<void()> received_response)
{
    // $TODO: max retry count, if exceeded mark request as failed (and restart service?)

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
            status = _request_status_none;
            if (received_response)
            {
                received_response();
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
            if (network_time_since(failure_time) >= g_backend_services.get()->service_request_refresh_interval())
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