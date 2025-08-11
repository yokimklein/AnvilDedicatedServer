#include "title_resource_service.h"

void c_backend::title_resource_service::initialise(c_backend::resolved_endpoint* endpoint)
{
    if (endpoint)
    {
        m_endpoint = endpoint;
    }
};
c_backend::resolved_endpoint& c_backend::title_resource_service::endpoint()
{
    return *m_endpoint;
}