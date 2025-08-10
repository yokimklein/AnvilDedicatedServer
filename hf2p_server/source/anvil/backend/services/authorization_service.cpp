#include "authorization_service.h"

void c_backend::authorization_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::authorization_service::endpoint()
{
    return *m_endpoint;
}